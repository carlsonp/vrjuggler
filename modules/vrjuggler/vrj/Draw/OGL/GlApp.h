/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_GL_APP_H_
#define _VRJ_GL_APP_H_

#include <vrj/vrjConfig.h>

#include <vrj/Kernel/App.h>

#include <vrj/Kernel/User.h>
#include <vrj/Display/Projection.h>

namespace vrj
{
   class Kernel;

/** \class GlApp GlApp.h vrj/Draw/OGL/GlApp.h
 *
 * vrj::GlApp encapulates an actual OpenGL application object.
 * This defines the base class from which OpenGL-based application classes
 * should be derived.  The interface given is what the kernel and the OpenGL
 * Draw Manager expect in order to interact with the application.
 *
 * The control loop will look similar to this:
 *
 * \code
 * glapp_obj->contextInit();        // called for each context
 * while (drawing)
 * {
 *    glapp_obj->preFrame();
 *    glapp_obj->latePreFrame();
 *    glapp_obj->bufferPreDraw();   // called for each draw buffer
 *    glapp_obj->contextPreDraw();  // called for each context
 *    glapp_obj->draw();            // called for each viewport
 *    glapp_obj->contextPostDraw(); // called for each context
 *    glapp_obj->intraFrame();      // called in parallel to the draw functions
 *    sync();
 *    glapp_obj->postFrame();
 *
 *    updateAllDevices();
 * }
 *
 * glapp_obj->contextClose();       // called for each context
 * \endcode
 *
 * @note One time through the loop is a Juggler Frame.
 *
 * @see vrj::App, vrj::Kernel
 */
class VJ_CLASS_API GlApp : public App
{
public:
   GlApp(Kernel* kern);

   GlApp();

   virtual ~GlApp();

   /**
    * Function that renders the scene.
    * Override this function with the user rendering routine.
    *
    * @pre OpenGL state has correct transformation and buffer selected.
    * @post The current scene has been drawn.
    */
   virtual void draw() = 0;

   /**
    * Function that is called immediately after a new context is created.
    * Use this function to create context-specific data structures such as
    * display lists and texture objects that are known to be required when
    * the context is created.
    *
    * @pre The OpenGL context has been set to the new context.
    * @post The application has completed context-specific initialization.
    */
   virtual void contextInit()
   {;}

   /**
    * Function that is called immediately before a context is closed.
    * Use this function to clean up any context-specific data structures.
    */
   virtual void contextClose()
   {;}

   /**
    * Function that is called upon entry into the context before rendering.
    * This can be used to allocate context-specific data dynamically.
    *
    * @pre The OpenGL context has been set to the context for drawing.
    * @post The application object has executed any commands that need to be
    *       executed only once per context, per frame.
    *
    * @note This function can be used for things that need to happen
    *       every frame but only once per context.
    */
   virtual void contextPreDraw()
   {;}

   /**
    * Function that is called upon exit of the context after rendering.
    *
    * @pre The OpenGL context has been set to the context for drawing.
    */
   virtual void contextPostDraw()
   {;}

   /**
    * Function that is called once for each frame buffer of an OpenGL context.
    * This function is executed after contextInit() (if needed) but before
    * contextPreDraw().  It is called once per frame buffer (see note).
    *
    * @pre The OpenGL context has been set to the context for drawing.
    * @post The application object has executed any commands that need to be
    *        executed once per context, per buffer, per frame.
    *
    * @note This function is designed to be used when some task must be
    *       performed only once per frame buffer (i.e., once for the left
    *       buffer, once for the right buffer).  For example, the OpenGL clear
    *       color should be defined and glClear(GL_COLOR_BUFFER_BIT) should be
    *       called in this method.
    */
   virtual void bufferPreDraw()
   {;}

   /**
    * Function that is called at the beginning of the drawing of each pipe.
    *
    * @pre The library is preparing to render all windows on a given pipe.
    * @post Any pre-pipe user calls have been done.
    *
    * @note Currently the OpenGL context is not set when this function is
    *       called.  This is a TEST function.  USE AT YOUR OWN RISK!!!
    */
   virtual void pipePreDraw()
   {;}

public:
   /** @name Factory functions
    * @{
    */

   /** Get the DrawManager to use.
    * Returns the OpenGL Draw Manager.
    */
   virtual DrawManager* getDrawManager();

   /** @} */
};

} // End of vrj namespace


#endif
