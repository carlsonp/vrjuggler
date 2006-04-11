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


#ifndef _VJ_GLOVE_INTERFACE_H_
#define _VJ_GLOVE_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to an glove device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjGloveInterface : public vjDeviceInterface
{
public:
   vjGloveInterface() : mGloveProxy(NULL)
   {;}

   vjGloveProxy* operator->()
   { return mGloveProxy; }

   vjGloveProxy& operator*()
   { return *(mGloveProxy); }

   virtual void refresh()
   {
      vjDeviceInterface::refresh();
      if(mProxyIndex != -1)
         mGloveProxy = vjKernel::instance()->getInputManager()->getGloveProxy(mProxyIndex);
      else
         mGloveProxy = vjKernel::instance()->getInputManager()->getDummyGloveProxy();
   }

private:
   vjGloveProxy* mGloveProxy;     // The proxy that is being wrapped
};

#endif