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

#include <vrj/vrjConfig.h>
//#include <sys/types.h>
#include <string>

#include <jccl/Config/ConfigChunk.h>

#include <vrj/Kernel/Kernel.h>

#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>
#include <gmtl/Xforms.h>

//#include <vrj/Math/Coord.h>
#include <vrj/Util/Debug.h>

#include <vrj/Display/SurfaceProjection.h>

namespace vrj
{


// Just call the base class constructor
void SurfaceProjection::config(jccl::ConfigChunkPtr chunk)
{
   vprASSERT( (chunk->getDescToken() == std::string("surfaceDisplay")) ||
              (chunk->getDescToken() == std::string("surfaceViewport")) );

   Projection::config(chunk);        // Call base class config first
}

/**
 * Recalculate the projection matrix.
 * Uses a method that needs to know the distance in the screen plane
 * from the origin (determined by the normal to the plane throught the
 * origin) to the edges of the screen.
 * This method can be used for any rectangular planar screen.
 * By adjusting the wall rotation matrix, this method can be used for
 * the general case of a rectangular screen in 3-space.
 *
 * @pre WallRotation matrix must be set correctly.
 * @pre mOrigin*'s must all be set correctly.
 * @pre eyePos is scaled by position factor.
 * @pre scaleFactor is the scale current used
 */
void SurfaceProjection::calcViewMatrix( gmtl::Matrix44f& eyePos, const float scaleFactor )
{
   calcViewFrustum(eyePos, scaleFactor);

   //Coord eye_coord(eyePos);
   gmtl::Vec3f   eye_pos( gmtl::makeTrans<gmtl::Vec3f>(eyePos) );             // Non-xformed pos

   // Need to post translate to get the view matrix at the position of the eye
   // View mat = base_M_
   mViewMat = m_surface_M_base * gmtl::makeTrans<gmtl::Matrix44f>( -eye_pos );
}


/**
 * Recalculate the view frustum.
 * Uses a method that needs to know the distance in the screen plane
 * from the origin (determined by the normal to the plane throught the
 * origin) to the edges of the screen.
 * This method can be used for any rectangular planar screen.
 * By adjusting the wall rotation matrix, this method can be used for
 * the general case of a rectangular screen in 3-space.
 *
 * @pre m_base_M_surface and m_surface_M_base matrices must be set correctly.
 * @pre mOrigin*'s must all be set correctly.
 */
void SurfaceProjection::calcViewFrustum(gmtl::Matrix44f& eyePos, const float scaleFactor)
{
   float near_dist, far_dist;
   near_dist = mNearDist;
   far_dist = mFarDist;

   // Distance measurements from eye to screen/edges
   // Distance to edges is from the point on the screen plane
   // where a normal line would go through the origin.
   float eye_to_screen, eye_to_right, eye_to_left, eye_to_top, eye_to_bottom;

   // Distances in near plane, in near plane from origin.  (Similar to above)
   float n_eye_to_right, n_eye_to_left, n_eye_to_top, n_eye_to_bottom;

   // Compute transformed eye position
   // - Converts eye coords into the surface's coord system
   gmtl::Point3f   eye_surface;         // Xformed position of eyes
   eye_surface = m_surface_M_base * gmtl::makeTrans<gmtl::Point3f>(eyePos);

   vprDEBUG(vrjDBG_DISP_MGR,7)
      << "SurfaceProjection::calcviewFrustum:    Base eye:" << gmtl::makeTrans<gmtl::Point3f>(eyePos)
      << "  Xformed eye:" << eye_surface
      << std::endl << vprDEBUG_FLUSH;

   // Compute dist from eye to screen/edges
   // Take into account scale since all origin to anythings are in meters
   eye_to_screen = (scaleFactor * mOriginToScreen) + eye_surface[gmtl::Zelt];
   eye_to_right =  (scaleFactor * mOriginToRight) - eye_surface[gmtl::Xelt];
   eye_to_left =   (scaleFactor * mOriginToLeft) + eye_surface[gmtl::Xelt];
   eye_to_top =    (scaleFactor * mOriginToTop) - eye_surface[gmtl::Yelt];
   eye_to_bottom = (scaleFactor * mOriginToBottom) + eye_surface[gmtl::Yelt];

   // Find dists on near plane using similar triangles
   float near_distFront = near_dist/eye_to_screen;      // constant factor
   n_eye_to_left = eye_to_left*near_distFront;
   n_eye_to_right = eye_to_right*near_distFront;
   n_eye_to_top = eye_to_top*near_distFront;
   n_eye_to_bottom = eye_to_bottom*near_distFront;

   // Set frustum and calulcate the matrix
   mFrustum.set(-n_eye_to_left, n_eye_to_right,
                -n_eye_to_bottom, n_eye_to_top,
                near_dist, far_dist);

   mFocusPlaneDist = eye_to_screen;    // Needed for drawing

   vprDEBUG(vrjDBG_DISP_MGR,7)
      << "SurfaceProjection::calcWallProjection: \n\tFrustum: " << mFrustum
      << std::endl << vprDEBUG_FLUSH;

}

std::ostream& SurfaceProjection::outStream(std::ostream& out,
                                        const unsigned int indentLevel)
{
   const int pad_width_dot(20 - indentLevel);
   out.setf(std::ios::left);

   const std::string indent_text(indentLevel, ' ');

   out << indent_text << std::setw(pad_width_dot)
       << "Type " << " vrj::SurfaceProjection\n";

   return Projection::outStream(out, indentLevel);
}

}