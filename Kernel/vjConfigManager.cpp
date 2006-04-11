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
#include <Kernel/vjConfigManager.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkFactory.h>
#include <Kernel/vjDependencyManager.h>
#include <Kernel/vjDebug.h>
#include <stdlib.h>

vjConfigManager* vjConfigManager::_instance = NULL;

//: Do we need to check the pending list
//! CONCURRENCY: concurrent
// The routine counts the number of pending chunks
// each time it is called.
// if it goes pending_repeat_limit calls without
// changing size, then it returns false until mLastPendingSize changes
bool vjConfigManager::pendingNeedsChecked()
{ 
   const int pending_repeat_limit(4);
   int cur_pending_size(0);
   bool ret_val(false);

   mPendingCountMutex.acquire();
   {
      cur_pending_size = mPendingConfig.size();
      if(cur_pending_size != mLastPendingSize)
      {
         ret_val = true;                           // Flag it for a check
         mPendingCheckCount=0;                     // Reset the counter
         mLastPendingSize = cur_pending_size;      // Keep track of size
      }
      else if(mPendingCheckCount < pending_repeat_limit)
      {
         ret_val = true;         // Less than count, so do a check
         mPendingCheckCount++;   // Increment it
         if(mPendingCheckCount == pending_repeat_limit)
         {
            vjDEBUG_BEGIN(vjDBG_ALL,0) << "vjConfigManager::pendingNeedsChecked: Pending list is now STALE."
                                       << cur_pending_size << " items still in pending\n" << vjDEBUG_FLUSH;
            lockPending();
            debugDumpPending();     // Output the stale pending list
            unlockPending();              
         }
      }
      else
      {
         ret_val = false;
      }         
   }
   mPendingCountMutex.release();

   return ret_val;
}


// Add the given chunk db to the pending list as adds
void vjConfigManager::addChunkDB(vjConfigChunkDB* db)
{
   vjASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
   lockPending();
   {
      vjPendingChunk pending;
      pending.mType = vjPendingChunk::ADD;
   
      for(std::vector<vjConfigChunk*>::iterator i=db->begin();i!=db->end();i++)
      {
         pending.mChunk = (*i);
         mPendingConfig.push_back(pending);
      }
   }
   unlockPending();

   // Reset pending count
   mPendingCountMutex.acquire();
   mPendingCheckCount = 0;
   mPendingCountMutex.release();

}

void vjConfigManager::removeChunkDB(vjConfigChunkDB* db)
{
   vjASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
   lockPending();
   {
      vjPendingChunk pending;
      pending.mType = vjPendingChunk::REMOVE;
   
      for(std::vector<vjConfigChunk*>::iterator i=db->begin();i!=db->end();i++)
      {
         pending.mChunk = (*i);
         mPendingConfig.push_back(pending);
      }
   }
   unlockPending();

   // Reset pending count
   mPendingCountMutex.acquire();
   mPendingCheckCount = 0;
   mPendingCountMutex.release();

}

// Look for items in the active list that don't have their dependencies filled anymore
//
//! POST: Any chunks in active with dependencies not filled are added to the 
//+       the pending list. (A remove and an add are added to the pending)
//! RETURNS: The number of lost dependencies found
int vjConfigManager::scanForLostDependencies()
{
   vjASSERT(0 == mActiveLock.test());        // We can't hold the lock upon entry

   vjDEBUG_BEGIN(vjDBG_ALL,1) << "vjConfigManager::scanForLostDependencies: Entered: \n" << vjDEBUG_FLUSH;

   vjDependencyManager* dep_mgr = vjDependencyManager::instance();
   std::vector<vjConfigChunk*> chunks;
   int num_lost_deps(0);

   // NOTE: can't hold this lock because the depSatisfied routines make use of the activeLock as well
   // NOTE: Make the copy of the chunks so that we can iterate without fear of active changing
   mActiveLock.acquire();
      chunks = mActiveConfig.getChunks();   // Get a copy of the chunks
   mActiveLock.release();

   // Now test them
   for(unsigned int i=0;i<chunks.size();i++)
   {
      if(!dep_mgr->depSatisfied(chunks[i]))      // We are not satisfied
      {
         vjDEBUGlg(vjDBG_ALL,1,false,true) << chunks[i]->getProperty("name")
                                            << " type: " << ((std::string)chunks[i]->getType()).c_str()
                                            << " has lost dependencies.\n"
                                            << vjDEBUG_FLUSH;
         
         num_lost_deps++;              // Keep a count of the number lost deps found

         // Add the pending removal
         vjPendingChunk pending;
         pending.mType = vjPendingChunk::REMOVE;
         pending.mChunk = chunks[i];
         addPending(pending);
         
         // Add the pending re-addition
         vjConfigChunk* copy_of_chunk;          // Need a copy so that the remove can delete the chunk
         (*copy_of_chunk) = (*chunks[i]);       // Make the copy
         pending.mType = vjPendingChunk::ADD;
         pending.mChunk = copy_of_chunk;
         addPending(pending);                   // Add the add item
      }
   }
   
   vjDEBUG_END(vjDBG_ALL,1) << "vjConfigManager::scanForLostDependencies: Exiting: \n" << vjDEBUG_FLUSH;

   return num_lost_deps;
}

