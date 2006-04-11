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


#ifndef _VJ_TS_OBJECT_PROXY_H_
#define _VJ_TS_OBJECT_PROXY_H_
//#pragma once

#include <Threads/vjTSObject.h>
#include <Threads/vjThreadManager.h>

//-----------------------------------------------------------------
//: This is a smart pointer to a thread specific object.
//
//  This allows users to have an object that has a seperate copy
// for each thread.
//
//! NOTE: The object used for type T must have a default constructor
//+       This class creates each instance of the real objects
//+       using this default constructor.
//-----------------------------------------------------------------
//! PUBLIC_API:

template <class T>
class vjTSObjectProxy
{
public:
   //-----------------------------------------------------------------
   //: Constructor for proxy.
   // The "real" object is actually create in this routine.
   //-----------------------------------------------------------------
   vjTSObjectProxy() : mObjectKey(-1)
   {
      vjTSObject<T>* new_object = new vjTSObject<T>;  // create object to add
      mObjectKey = vjThreadManager::instance()->addTSObject(new_object);  // Tell tables to add it
   }

   //-----------------------------------------------------------------
   //: Destructor.
   //  When proxy is destroyed, we want to delete the object from the
   //  global tables.
   //-----------------------------------------------------------------
   ~vjTSObjectProxy()
   {
      vjThreadManager::instance()->removeTSObject(mObjectKey);
   }

   T* operator->()
   { return getSpecific(); }

   T& operator*()
   { return *getSpecific();}

private:
   //-----------------------------------------------------------------
   //: Get the correct version for current thread
   // - Find the correct table
   // - Get the obj pointer
   // - Attempts a dynamic cast
   //-----------------------------------------------------------------
   T* getSpecific()
   {
      vjTSTable* table =
         vjThreadManager::instance()->getCurrentTSTable();        // get table for current thread
      vjTSBaseObject* object = table->getObject(mObjectKey);      // get the specific object
      vjTSObject<T>* real_object = dynamic_cast< vjTSObject<T>* >(object); // try dynamic casting it

      vjASSERT(real_object != NULL);      // If fails, it means that "real" object was different type than the proxy
      if(real_object == NULL)
         return NULL;
      else
         return real_object->getObject();                                   // return the ptr;
   }

   // Don't allow copy construction
   vjTSObjectProxy(vjTSObjectProxy& proxy)
   {;}
private:
   long  mObjectKey;    // The key to find the object
};

#endif