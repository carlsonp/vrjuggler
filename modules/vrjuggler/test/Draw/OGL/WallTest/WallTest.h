/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

/* This Application draws a test pattern in 3D on all the walls in the application
 * It also draws a cube on the left eye, and a tetrahedron on the right eye
 * Button 0 moves the light (off initially)
 * Button 1 toggles lighting
 * Button 2 toggles a line of cubes through the screen on and off (Cubes are 1/2 foot across)
 * Button 3 toggles the cube/tetrahedron on off
 */

#ifndef WALLTEST_H
#define WALLTEST_H

#include <vrj/vrjConfig.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Display/SurfaceViewport.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <vector>

#include <TestMode.h>

using namespace vrj;

/** Test program for wall settings.
 *
 */
class WallTest : public GlApp
{
public:
   WallTest()
   {
      mUseLights=false;      
      mCurMode = 0;
      mLightPosition=gmtl::Point4f(0,5,0,1);
   }

   virtual ~WallTest(){;}

public:
   virtual void init();

   virtual void apiInit(){;}

public:
   virtual void bufferPreDraw();

   virtual void preFrame();
   virtual void contextInit();
   virtual void draw();   
    
	
public:
   bool mUseLights;      
   gmtl::Point4f mLightPosition;
   
   gadget::PositionInterface  mWand;    /**< Positional interface for Wand position */
   gadget::PositionInterface  mHead;    /**< Positional interface for Head position */
   gadget::DigitalInterface   mButton0; /**< Digital interface for button 0 */
   gadget::DigitalInterface   mButton1; /**< Digital interface for button 1 */
   gadget::DigitalInterface   mButton2;
   gadget::DigitalInterface   mButton3;

   std::vector<TestModePtr>   mTestModes;    /** List of test modes. */
   unsigned                   mCurMode;      /** Current test mode to use. */
};


#endif