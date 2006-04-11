/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _GADGET_SIM_RELATIVE_POSITION_H
#define _GADGET_SIM_RELATIVE_POSITION_H

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/InputMixer.h>
#include <jccl/Config/ConfigChunkPtr.h>


namespace gadget
{
/**
 * Simulated a position device by making.
 * An existing device behave relative to another one.
 *
 * One device is the base coordinate system frame of reference.
 * The other device has it's position information transformed
 * to be relative to that frame.
 * ie. There is a matrix multiply.
 *
 * This class should not be used directly by the user.
 */
//class SimRelativePosition : public Input, public Position
class SimRelativePosition : public InputMixer<Input,Position>
{
public:
   SimRelativePosition() {;}
   virtual ~SimRelativePosition() {;}

   virtual bool config(jccl::ConfigChunkPtr chunk);

   /** These functions don't do anything. */
   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   /** Updates the data. */
   virtual void updateData();

   static std::string getChunkType() { return std::string( "SimRelativePosition" ); }

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   PositionData      mPos;                   /**< The current position being simulated */
   PositionInterface mBaseFrame;             /**< The base frame of reference */
   PositionInterface mRelativePos;           /**< the relative position */

};

} // End of gadget namespace

#endif