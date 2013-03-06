
#include <gadget/Devices/DriverConfig.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include "Vrpn.h"
#include "VRPNText.h"


extern "C"
{
   GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
   {
      new gadget::DeviceConstructor<gadget::Vrpn>(inputMgr);
      new gadget::DeviceConstructor<gadget::VRPNText>(inputMgr);
   }
}
