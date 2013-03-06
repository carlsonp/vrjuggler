/** @file
	@brief Implementation

	@date 2012

	@author
	Patrick Carlson <carlsonp@iastate.edu>
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "VRPNText.h"

// Library/third-party includes
#include <boost/bind.hpp>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/Devices/DriverConfig.h>


// Standard includes
// - none


namespace gadget
{
   namespace
   {
      void receiverDeleter(void * data, vrpn_TEXTHANDLER handler, vrpn_Text_Receiver * t)
      {
         t->unregister_message_handler(data, handler);
         delete t;
      }

      boost::shared_ptr<vrpn_Text_Receiver> createReceiverWithHandler(std::string const& name, void * data, vrpn_TEXTHANDLER handler)
      {
         using namespace boost;
         boost::shared_ptr<vrpn_Text_Receiver> ret(new vrpn_Text_Receiver(name.c_str()), boost::bind(receiverDeleter, data, handler, _1));
         //setup VRPN callback
         //http://www.cs.unc.edu/Research/vrpn/Connection_interest.html
         ret->register_message_handler(data, handler);
         return ret;
      }

   }
   void VRPN_CALLBACK VRPNText::handle_text(void *userdata, const vrpn_TEXTCB t)
   {
      if (t.type == vrpn_TEXT_NORMAL)
      {
         //Cast void pointer to the appropriate type
         VRPNText *self = static_cast<VRPNText*>(userdata);
         self->setStoredText(t.message);
      }
   }

   VRPNText::VRPNText() {}

   VRPNText::~VRPNText()
   {
      //Destructor
      stopSampling();
   }

   bool VRPNText::config(jccl::ConfigElementPtr c)
   {
      if (!(Input::config(c) && String::config(c)))
      {
         return false;
      }

      //Pull in tracker name and IP address from JCCL
      //e.g. mytracker@127.0.0.1
      name = c->getProperty<std::string>("device");
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            <<  "Device: " << name << std::endl
            << vprDEBUG_FLUSH;

      return true;
   }

   bool VRPNText::startSampling()
   {

      if (text)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "gadget::VRPNText was already started.\n" << std::endl
               << vprDEBUG_FLUSH;
         return false;
      }
      try
      {

         //Setup VRPN connection
         std::cout << "Starting up VRPN Text connection: " << name << std::endl;
         text = createReceiverWithHandler(name, this, handle_text);
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": Failed to start VRPNText driver!\n"
               << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << ex.what() << std::endl << vprDEBUG_FLUSH;
      }

      return text;
   }

   bool VRPNText::sample()
   {
      if (!text)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
               << "gadget::VRPNText not active, returning false.\n"
               << std::endl << vprDEBUG_FLUSH;
         return false;
      }

      //run VRPN mainloop
      text->mainloop();

      std::vector<StringData> temp;
      temp.push_back(storedText);
      if (!storedText.empty())
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "gadget::VRPNText got something: " << storedText
               << std::endl << vprDEBUG_FLUSH;
      }
      addStringSample(temp);

      storedText.clear();

      return true;
   }

   bool VRPNText::stopSampling()
   {
      bool ret = text;
      text.reset();

      return ret;
   }

   void VRPNText::updateData()
   {
      if (!text)
      {
         return;
      }
      sample();
      swapStringBuffers();
   }
}
