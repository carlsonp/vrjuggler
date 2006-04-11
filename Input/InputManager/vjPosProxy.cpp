/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#include <vjConfig.h>
#include <Input/InputManager/vjPosProxy.h>
#include <Kernel/vjKernel.h>


//: Set the transform for this vjPosProxy
// Sets the transformation matrix to
//    mMatrixTransform = M<sub>trans</sub>.post(M<sub>rot</sub>)
//! NOTE: This means that to set transform, you specific the translation
//+       followed by rotation that takes the device from where it physically
//+       is in space to where you want it to be.
void vjPosProxy::setTransform( float xoff, float yoff, float zoff,    // Translate
                   float xrot, float yrot, float zrot)   // Rotate
{
   etrans = true;
   m_matrixTransform.makeIdent();
   vjMatrix trans_mat; trans_mat.makeIdent();
   vjMatrix rot_mat; rot_mat.makeIdent();

   if((xoff != 0.0f) || (yoff != 0.0f) || (zoff != 0.0f))
      trans_mat.makeTrans(xoff, yoff, zoff);
   if((xrot != 0.0f) || (yrot != 0.0f) || (zrot != 0.0f))
      rot_mat.makeXYZEuler(xrot, yrot, zrot);

   m_matrixTransform.mult(trans_mat, rot_mat);
}

//: Set the vjPosProxy to now point to another device and subDevicenumber
void vjPosProxy::set(vjPosition* posPtr, int unitNum)
{
   vjASSERT( posPtr->fDeviceSupport(DEVICE_POSITION) );
   vjDEBUG(vjDBG_INPUT_MGR,1) << "posPtr: " << posPtr << endl
              << "unit  : " << unitNum << endl << endl << vjDEBUG_FLUSH;
   m_posPtr = posPtr;
   m_unitNum = unitNum;
}


bool vjPosProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,3) << "------------------ POS PROXY config() -----------------\n" << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "PosProxy");

   int unitNum = chunk->getProperty("unit");
   std::string proxy_name = chunk->getProperty("name");
   std::string dev_name = chunk->getProperty("device");

   if (true == (bool)chunk->getProperty("etrans") )
   {
      vjDEBUG(vjDBG_INPUT_MGR,3) << "Position Transform enabled..." << endl << vjDEBUG_FLUSH;
      setTransform
      ( chunk->getProperty("translate",0) , // xtrans
        chunk->getProperty("translate",1) , // ytrans
        chunk->getProperty("translate",2) , // ztrans
        chunk->getProperty("rotate",0) , // xrot
        chunk->getProperty("rotate",1) , // yrot
        chunk->getProperty("rotate",2) );// zrot
      vjDEBUG(vjDBG_INPUT_MGR,4) << "Transform Matrix: " << endl << getTransform() << endl << vjDEBUG_FLUSH;
   }

   // --- SETUP PROXY with INPUT MGR ---- //
   int proxy_num = vjKernel::instance()->getInputManager()->addPosProxy(dev_name,unitNum,proxy_name,this);

   if ( proxy_num != -1)
   {
      vjDEBUG_END(vjDBG_INPUT_MGR,3) << "   PosProxy config()'ed" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG_END(vjDBG_INPUT_MGR,0) << "   PosProxy config() failed" << endl << vjDEBUG_FLUSH;
      vjDEBUG_END(vjDBG_INPUT_MGR,3) << endl << vjDEBUG_FLUSH;
      return false;
   }
}