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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.config;

import java.util.EventListener;

/**
 * Interface for objects that wish to be notified when a ChunkDesc changes in
 * some way.
 */
public interface ChunkDescListener
   extends EventListener
{
   public void nameChanged(ChunkDescEvent evt);
   public void tokenChanged(ChunkDescEvent evt);
   public void helpChanged(ChunkDescEvent evt);

   public void categoryAdded(ChunkDescEvent evt);
   public void categoryRemoved(ChunkDescEvent evt);

   public void propertyDescChanged(ChunkDescEvent evt);
   public void propertyDescAdded(ChunkDescEvent evt);
   public void propertyDescRemoved(ChunkDescEvent evt);
}
