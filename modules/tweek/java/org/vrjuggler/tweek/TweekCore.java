/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.tweek;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import javax.swing.UIManager;
import javax.swing.tree.DefaultMutableTreeNode;
import com.incors.plaf.kunststoff.KunststoffLookAndFeel;
import com.incors.plaf.kunststoff.mini.KunststoffMiniLookAndFeel;
import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.tweek.beans.loader.BeanInstantiationException;
import org.vrjuggler.tweek.gui.TweekFrame;
import org.vrjuggler.tweek.services.*;
import org.vrjuggler.tweek.text.MessageDocument;
import org.vrjuggler.tweek.net.corba.*;


/**
 * @since 1.0
 */
public class TweekCore
   implements BeanInstantiationListener
{
   // ========================================================================
   // Public methods.
   // ========================================================================

   public static TweekCore instance ()
   {
      if ( m_instance == null )
      {
         m_instance = new TweekCore();
      }

      return m_instance;
   }

   public void init (String[] args) throws Exception
   {
      // Register the internal static beans
      registerStaticBeans();

      // This needs to be the first step to ensure that all the basic services
      // and viewers get loaded.
      String default_path = System.getProperty("TWEEK_BASE_DIR") +
                            File.separator + "bin" + File.separator + "beans";

      mBeanDirs.add(default_path);

      // As a side effect, the following may add more paths to mBeanDirs.
      String[] new_args = parseTweekArgs(args);

      // Explicitly load the global preferences now.  The
      // GlobalPreferencesService class does not load them automatically.  This
      // must happen after the command line arguments have been parsed so that
      // the user can specify an alternate preferences file.
      GlobalPreferencesService global_prefs =
         (GlobalPreferencesService) BeanRegistry.instance().getBean("GlobalPreferences");
      global_prefs.load();

      // Set the look and feel now so that any GUI components that are
      // instantiated hereafter will have the correct look and feel.
      // XXX: If there are GUI components loaded statically (see above), they
      // will need to be updated.
      setLookAndFeel(global_prefs);

      // Loop over all the known Bean directories to search for and load any
      // Beans that are found.  This must occur after the global preferences
      // have been loaded so that the user can enable or disable lazy Panel
      // Bean instantiation.
      Iterator i = mBeanDirs.iterator();
      while ( i.hasNext() )
      {
         String path = (String) i.next();

         try
         {
            findAndLoadBeans(path);
         }
         catch (BeanPathException e)
         {
            System.out.println("WARNING: Invalid path " + path);
         }
      }

      // Register the command-line arguments with the Environment Service (if
      // it is available).
      try
      {
         EnvironmentService service = (EnvironmentService)
            BeanRegistry.instance().getBean( EnvironmentService.class.getName() );

         if ( service != null )
         {
            service.setCommandLineArgs(new_args);
         }
      }
      catch (ClassCastException e)
      {
         // Use System.err here because the GUI has not been displayed yet.
         System.err.println("WARNING: Failed to register command-line arguments");
      }

      m_gui = new TweekFrame(mMsgDocument);

      // Now we need to register the TweekFrame instance as a listener for
      // BeanFocusChangeEvents.
      List viewer_beans = BeanRegistry.instance().getBeansOfType( ViewerBean.class.getName() );
      for ( Iterator itr = viewer_beans.iterator(); itr.hasNext(); )
      {
         BeanModelViewer v = ((ViewerBean)itr.next()).getViewer();
         v.addBeanFocusChangeListener(m_gui);
      }

      m_gui.initGUI();

      // Now select the default bean if necessary
      if (defaultBean != null)
      {
         mMsgDocument.printStatusnl("Trying to focus default bean: " + defaultBean);
         TweekBean bean = BeanRegistry.instance().getBean(defaultBean);

         // Valid the bean registered under the default bean's name
         if (bean == null)
         {
            mMsgDocument.printWarningnl("WARNING: Default Bean doesn't exist");
         }
         else if (! (bean instanceof PanelBean))
         {
            mMsgDocument.printWarningnl("WARNING: Default Bean is not a Panel Bean");
         }
         else
         {
            ViewerBean viewer = m_gui.getBeanViewer();
            if (viewer != null)
            {
               viewer.getViewer().focusBean((PanelBean)bean);
            }
         }
      }
   }

   /**
    * Registers the beans that are internal to Tweek that are required to exist
    * before the bean loading can begin.
    */
   protected void registerStaticBeans()
   {
      BeanRegistry registry = BeanRegistry.instance();

      // environment service
      registry.registerBean( new EnvironmentService(
            new BeanAttributes( "Environment" ) ) );

      // global preferences service
      registry.registerBean( new GlobalPreferencesService(
            new BeanAttributes( "GlobalPreferences" ) ) );

   }

   /**
    * Called by the BeanInstantiationCommunicator singleton whenever a new bean
    * is instantiated.
    */
   public void beanInstantiated (BeanInstantiationEvent evt)
   {
      // If the bean created is a viewer bean, initialize it with tweek
      Object bean = evt.getBean();
      if ( bean instanceof ViewerBean ) {
         BeanModelViewer viewer = ((ViewerBean)bean).getViewer();
         viewer.setModel(panelTreeModel);
         viewer.initGUI();
      }
   }


   /**
    * Look for TweekBeans in standard locations and register them in the
    * registry.
    *
    * @param path    the path in which to search for TweekBeans
    */
   public void findAndLoadBeans( String path )
      throws BeanPathException
   {
      BeanRegistry registry = BeanRegistry.instance();

      // This service is loaded statically, so we do not have to worry about
      // finding the Bean first.
      GlobalPreferencesService prefs =
         (GlobalPreferencesService) registry.getBean("GlobalPreferences");

      // Get the beans in the given path and add them to the dependency manager
      XMLBeanFinder finder = new XMLBeanFinder(mValidateXML);
      List beans = finder.find( path );
      for ( Iterator itr = beans.iterator(); itr.hasNext(); )
      {
         TweekBean bean = (TweekBean)itr.next();
         mDepManager.add(bean);
      }

      // Instantiate the beans pending in the dependency manager
      while (mDepManager.hasBeansPending())
      {
         TweekBean bean = (TweekBean)mDepManager.pop();
         if (bean == null)
         {
            // There are more beans pending, but they all have unsatisfied
            // dependencies so we can instantiate them ...
            break;
         }

         // Try to instantiate the Bean.
         try
         {
            // If the current Bean is not a Panel Bean or the user has disabled
            // lazy Panel Bean instantiation, we can instantiate the Bean.
            if ( ! (bean instanceof org.vrjuggler.tweek.beans.PanelBean) ||
                 ! prefs.getLazyPanelBeanInstantiation() )
            {
               bean.instantiate();
            }

            registry.registerBean(bean);
         }
         catch (BeanInstantiationException e)
         {
            mMsgDocument.printWarningnl("WARNING: Failed to instantiate Bean'" +
                                       bean.getName() + "': " +
                                       e.getMessage());
         }
      }
   }

   public BeanTreeModel getPanelTreeModel()
   {
      return panelTreeModel;
   }

   /**
    * Gets the name of the panel bean to select/instantiate by default when
    * Tweek is started.
    *
    * @return  the name of the default bean; null if there is no default
    */
   public String getDefaultBean()
   {
      return defaultBean;
   }

   /**
    * Default constructor.
    */
   protected TweekCore ()
   {
      BeanInstantiationCommunicator.instance().addBeanInstantiationListener( this );
   }

   /**
    * Looks through the given array of arguments for any that are specific to
    * the Tweek Java GUI.  Those that are recognized are removed and handled
    * here.  Unrecognized options are left in the array.  The remaining
    * arguments are returned to the caller in a new array.
    *
    * @post Any Tweek-specific arguments are removed and a new array without
    *       those arguments is returned.
    */
   protected String[] parseTweekArgs (String[] args)
   {
      Vector save_args = new Vector();

      for ( int i = 0; i < args.length; i++ )
      {
         if ( args[i].startsWith("--beanpath=") )
         {
            int start = args[i].indexOf('=') + 1;
            String path = args[i].substring(start);
            mBeanDirs.add(path);
         }
         else if ( args[i].startsWith("--prefs=") )
         {
            int start   = args[i].indexOf('=') + 1;
            String path = args[i].substring(start);
            GlobalPreferencesService prefs =
               (GlobalPreferencesService) BeanRegistry.instance().getBean("GlobalPreferences");
            prefs.setFileName(path);
         }
         else if ( args[i].startsWith("--defaultbean=") )
         {
            int start = args[i].indexOf('=') + 1;
            defaultBean = args[i].substring(start);
         }
         else if ( args[i].startsWith("--validate") )
         {
            mValidateXML = true;
         }
         else
         {
            save_args.add(args[i]);
         }
      }

      String[] new_args = null;

      if ( save_args.size() > 0 )
      {
         new_args = new String[save_args.size()];

         for ( int i = 0; i < save_args.size(); i++ )
         {
            new_args[i] = (String) save_args.elementAt(i);
         }
      }

      return new_args;
   }

   // ========================================================================
   // Protected data members.
   // ========================================================================

   protected static TweekCore m_instance = null;

   // ========================================================================
   // Private methods.
   // ========================================================================

   /**
    * Sets the look and feel for the GUI.  This assumes that the GUI will be
    * based on Swing and that Swing is available.
    */
   private void setLookAndFeel(GlobalPreferencesService prefs)
   {
      // Install extra look and feels.
      UIManager.installLookAndFeel("Kunststoff",
                                   KunststoffLookAndFeel.class.getName());
      KunststoffLookAndFeel.setIsInstalled(true);

      UIManager.installLookAndFeel("Kunststoff Mini",
                                   KunststoffMiniLookAndFeel.class.getName());
      KunststoffMiniLookAndFeel.setIsInstalled(true);

      // This class installs itself with the UI Manager automatically.
      new net.sourceforge.mlf.metouia.MetouiaLookAndFeel();

     try
      {
         UIManager.setLookAndFeel(prefs.getLookAndFeel());
      }
      catch (Exception e)
      {
         this.mMsgDocument.printWarningnl("Failed to set look and feel to " +
                                          prefs.getLookAndFeel());
      }
   }

   // ========================================================================
   // Private data members.
   // ========================================================================

   /**
    * The name of the panel bean to select by default when Tweek is started.
    */
   private String defaultBean = null;

   /**
    * The bean dependency manager used to load beans in a stable order.
    */
   private BeanDependencyManager mDepManager = new BeanDependencyManager();

   private boolean mValidateXML = false;
   private List    mBeanDirs    = new ArrayList();

   private MessageDocument mMsgDocument = new MessageDocument();
   private TweekFrame m_gui = null;

   private BeanTreeModel panelTreeModel =
      new BeanTreeModel(new DefaultMutableTreeNode());
}
