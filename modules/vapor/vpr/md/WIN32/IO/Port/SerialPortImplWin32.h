/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SERIAL_PORT_IMPL_WIN32_H_
#define _VPR_SERIAL_PORT_IMPL_WIN32_H_

#include <vpr/vprConfig.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include <vpr/IO/IOSys.h>
#include <vpr/IO/Port/SerialTypes.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

/**
 * vpr::SerialPort implementation for Win32.
 */
class VPR_CLASS_API SerialPortImplWin32
{
public:
   /**
    * Constructor.  This creates a file handle object connected to the given
    * port name and sets the update action to happen immediately.
    *
    * @post mHandle is assigned a value and connected to port_name, and the
    *       update action is set to vpr::SerialTypes::NOW.
    *
    * @param port_name The name of the serial port that will be accessed.
    */
   SerialPortImplWin32(const std::string& port_name);

   /**
    * Destructor.  If the serial port is still open, it is closed.
    *
    * @post The serial port is closed.
    */
   ~SerialPortImplWin32();

   // ========================================================================
   // vpr::BlockIO basics.
   // ========================================================================

   /**
    * Gets the name of this serial port.
    *
    * @post
    *
    * @return An object containing the name of the serial port.
    */
   const std::string& getName()
   {
      return mName;
   }

   /**
    * Sets the open flags so that the serial port is opened in read-only
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in read-only mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenReadOnly()
   {
      mOpenFlag = GENERIC_READ;
   }

   /**
    * Sets the open flags so that the serial port is opened in write-only
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in write-only mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenWriteOnly()
   {
      mOpenFlag = GENERIC_WRITE;
   }

   /**
    * Sets the open flags so that the serial port is opened in read/write
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in read/write mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenReadWrite()
   {
      mOpenFlag = GENERIC_READ | GENERIC_WRITE;
   }

   /**
    * Sets the blocking flags so that the serial port is opened in blocking
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it
    *       is opened in blocking mode.  If the port is already open, this
    *       has no effect.
    */
   void setOpenBlocking()
   {
      mBlocking &= ~FILE_FLAG_OVERLAPPED;
   }

   /**
    * Sets the blocking flags so that the serial port is opened in
    * non-blocking mode.
    *
    * @post The open flags are updated so that when the port is opened, it
    *       is opened in non-blocking mode.  If the port is already open,
    *       this has no effect.
    */
   void setOpenNonBlocking()
   {
	   //Still not sure why this is not supported
      std::cout << "non-Blocking not currently supported in win32." << std::endl;
//      mBlocking |= FILE_FLAG_OVERLAPPED;
   }

   /**
    * Opens the serial port and initializes its flags.
    *
    * @pre The serial port is not already open.
    * @post An attempt is made to open the port.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.  If the port is opened, mOpen is set to
    *       true.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port was
    *         opened successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus open();

   /**
    * Closes the serial port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to close the port.  The resulting status is
    *       returned to the caller.  If the port is closed, mOpen is set to
    *       false.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port was
    *         closed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus close();

   /**
    * Reconfigures the serial port so that it is in blocking or non-blocking
    * mode.
    *
    * @pre The serial port is open.
    * @post Processes will block when accessing the port.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the blocking mode was
    *         changed successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus setBlocking(const bool& flag)
   {
      std::cout << "Enabling blocking mode after port open is unsuported in Win32." << std::endl;
      return vpr::ReturnStatus::Fail;
   }

   /**
    * Get the current blocking state for the serial port.
    *
    * @pre mBlocking is set correctly
    *
    * @return true is returned if the port is in blocking mode.<br>
    *         false is returned if the port is in non-blocking mode.
    */
   bool isBlocking() const
   {
      return ! (mBlocking & FILE_FLAG_OVERLAPPED);
   }

   /**
    * Returns the contained handle.
    */
   vpr::IOSys::Handle getHandle()
   {
#ifdef VPR_USE_NSPR
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "ERROR: Cannot get handle for Win32 file descriptor with NSPR!\n";
      return vpr::IOSys::NullHandle;
#else
      return mHandle;
#endif
   }

   /**
    * Tests if the serial port is read-only.
    *
    * @pre The serial port is open.
    * @post The access mode is tested for read-only mode, and the result is
    *       returned to the caller.
    *
    * @return <code>true</code> is returned if the device is in read-only
    *         mode; <code>false</code> otherwise.
    */
   bool isReadOnly()
   {
      return GENERIC_READ == mOpenFlag;
   }

   /**
    * Tests if the serial port is write-only.
    *
    * @pre The serial port is open.
    * @post The access mode is tested for write-only mode, and the result is
    *       returned to the caller.
    *
    * @return <code>true</code> is returned if the device is in write-only
    *         mode; <code>false</code> otherwise.
    */
   bool isWriteOnly()
   {
      return GENERIC_WRITE == mOpenFlag;
   }

   /**
    * Tests if the serial port is read/write.
    *
    * @pre The serial port is open.
    * @post The access mode is tested for read/write mode, and the result is
    *       returned to the caller.
    *
    * @return <code>true</code> is returned if the device is in read/write
    *         mode; <code>false</code> otherwise.
    */
   bool isReadWrite()
   {
      return(GENERIC_READ | GENERIC_WRITE) == mOpenFlag;
   }

   // ========================================================================
   // VPR serial port interface implementation.
   // ========================================================================

   /**
    * Gets the current update action.  This tells when updates to the serial
    * device take effect.
    *
    * @post The current update action is returned to the caller.
    *
    * RETURNS: A vpr::SerialTypes::UpdateActionOption value stating when
    *          updates take effect.
    */
   vpr::SerialTypes::UpdateActionOption getUpdateAction();

   /**
    * Changes the current update action to take place as described by the
    * given value.
    *
    * @post The update action is modified to use the new value.  The results
    *       will be seen with the next device operation.
    *
    * @param action The new update action value.
    */
   void setUpdateAction (vpr::SerialTypes::UpdateActionOption action);

   /**
    * Clears all of the serial port's flags
    *
    * @pre This serial port is already open.
    * @post An attempt is made to change the flags.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port was opened
    *         successfully.
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus clearAll();

   /**
    * Queries the serial port for the maximum buffer size.
    *
    * @pre The serial port is open.
    * @post The maximum buffer size is returned to the caller through the
    *       by-reference argument.
    *
    * @param size A reference to a vpr::Uint8 where the buffer size is
    *             stored for return to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the buffer size was
    *         retrieved successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus getBufferSize(vpr::Uint16& size);

   /**
    * Attempts to change the buffer size to the given argument.
    *
    * @pre The serial port is open.
    * @post If the buffer size is usable on the port, the port attributes
    *       are updated and success is returned.  Otherwise, a failure
    *       status is returned.
    *
    * @param size The new size for the buffer.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the buffer size was set
    *         successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus setBufferSize(const vpr::Uint8 size);

   /**
    * Gets the value of the timeout (in tenths of a second) to wait for data
    * to arrive.  This is only applicable in non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The current timeout setting is returned to the caller in the
    *       by-reference argument.
    *
    * @param timeout A reference to a vpr::Uint8 to be used as storage for
    *                the timeout value.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the timeout was
    *         requested successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus getTimeout(vpr::Uint8& timeout);

   /**
    * Sets the value of the timeout to wait for data to arrive.  The value
    * given must be in tenths of a second.  This is only applicable in
    * non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The timeout interval is updated to use the given value.
    *
    * @param timeout_val The new timeout value measured in tenths of a
    *                    second.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the timeout was set
    *         successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus setTimeout(const vpr::Uint8 timeout);

   /**
    *  Get the character size (the bits per byte).
    *
    * @pre The serial port is open.
    * @post The current character size is returned to the caller in the
    *        by-reference argument.
    *
    * @param size  A reference to a vpr::SerialTypes::CharacterSizeOption
    *               object to be used as storage for the character size
    *               value.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getCharacterSize(vpr::SerialTypes::CharacterSizeOption& size);

   /**
    *  Set the current character size (the bits per byte) to the size in the
    *  given value.  This is used for both reding and writing, and the size
    *  does not include the parity bit (if any).
    *
    * @pre The serial port is open.
    * @post The bits per byte for the serial port is updated.
    *
    * @param bpb  The new character size.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setCharacterSize(const vpr::SerialTypes::CharacterSizeOption bpb);

   /**
    * Gets the current read state for the port.
    *
    * @pre The serial port is open.
    * @post The read state (either enabled or disabled) is returne to the
    *       caller.
    *
    * @return <code>true</code> is returned if bytes can be read from the
    *         device.
    */
   bool getReadState()
   {
      return true;
   }

   /**
    * Enables the receiver so that bytes can be read from the port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to make the device readable.
    *
    * @return vpr::ReturnStatus::Succeed is returned if reading is enabled
    *         successfully.
    */
   vpr::ReturnStatus setRead(const bool& flag)
   {
      return vpr::ReturnStatus();
   }

   /**
    * Get the local attach state.  That is, test if the device is attached
    * locally.
    *
    * @pre The serial port is open.
    * @post The local attach state is returned to the caller.
    * @return true  - The device is attached locally.
    *         false - The device is not attached locally, and opening the
    *                 device will usually block until there is a response.
    */
   bool getLocalAttachState();

   /**
    * Mark the device as locally attached or not.
    *
    * @pre This serial port is open.
    * @post The device has its local attachment state enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setLocalAttach(const bool& flag);

   /**
    * Get the current state of ignoring BREAK bytes.
    *
    *
    * @pre This serial port is open.
    * @post The BREAK byte ignore state is returned to the caller.
    *
    * @return true  - Bad bytes are ignored.
    * @return false - Bad bytes are not ignored.
    */
   bool getBreakByteIgnoreState();

   /**
    * Enable ignoring of received BREAK bytes or not.
    *
    *
    * @pre This serial port is open.
    * @post BREAK byte ignoring is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setBreakByteIgnore(const bool& flag);

   /**
    * Gets the number of stop bits in use.  This will be either 1 or 2.
    *
    * @pre The serial port is open.
    * @post The number of stop bits currently in use is returned via the
    *        by-reference argument.
    *
    * @param num_bits  A reference to a vpr::Uint8 that will be used to
    *                   store the number of stop bits.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getStopBits(vpr::Uint8& num_bits);

   /**
    * Sets the number of stop bits to use.  The value must be either 1 or 2.
    *
    * @pre The serial port is open, and the given number of stop bits is
    *       either 1 or 2.
    * @post The number of stop bits is updated to use the given value.
    *
    * @param num_bits  The number of stop bits to use.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setStopBits(const vpr::Uint8 num_bits);

   /**
    * Queries the canonical input state of the serial port.  If canonical mode
    * is enabled, the characters EOF, EOL, EOL2, ERASE, KILL, REPRINT,
    * STATUS, and WERASE are enabled, and the input characters are
    * assembled into lines.  Otherwise, read requests are satisfied directly
    * from the input queue, and a read will not return until the buffer is
    * at its minimum capacity or the timeout has expired.  See
    * getBufferSize() and getTimeout() for more information.
    *
    * @pre The serial port is open.
    * @post The current state of canonical input is returnd to the caller.
    *
    * @return true if canonical input is enabled; false otherwise
    */
   bool getCanonicalState(void);

   /**
    * Enables or disables canonical input.  See getCanonicalState() for more
    * information about what this means.
    *
    * @pre The serial port is open.
    * @post Canonical input is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see getCanonicalState
    */
   vpr::ReturnStatus setCanonicalInput(const bool& flag);

   /**
    * Gets the current state of ignoring bytes with framing errors (other
    * than a BREAK) or parity errors.
    *
    * @pre The serial port is open.
    * @post The erroneous byte ignore state is returned to the caller.
    *
    * @return true if bad bytes are ignored; false otherwise.
    */
   bool getBadByteIgnoreState();

   /**
    * Enables or disables ignoring of received bytes with framing errors or
    * parity errors.
    *
    * @pre The serial port is open.
    * @post Erroneous byte ignoring is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setBadByteIgnore(const bool& flag);

   /**
    * Gets the state of parity checking for input.
    *
    * @pre The serial port is open.
    * @post The state of input parity checking is returned to the caller.
    *
    * @return true if input parity checking is enabled; false otherwise.
    */
   bool getInputParityCheckState();

   /**
    * Enables or disables input parity checking.
    *
    * @pre The serial port is open.
    * @post Input parity checking is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setInputParityCheck(const bool& flag);

   /**
    * Gets the current state of bit stripping.  When enabled, input bytes are
    * stripped to seven bits.  Otherwise, all eight bits are processed.
    *
    * @pre The serial port is open.
    * @post The current bit stripping state is returned to the caller.
    *
    * @return true if input bytes are stripped to seven bits.  false is
    *         returned if input bytes are not stripped.
    */
   bool getBitStripState();

   /**
    * Enables or disables stripping of input bytes to seven bits.
    *
    * @pre The serial port is open.
    * @post Input bit stripping is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setBitStripping(const bool& flag);

   /**
    * Gets the state of start-stop input control.  When enabled, if the
    * terminal driver sees that the receive buffer is getting full, a STOP
    * command is sent to the serial device.  The device should respond by
    * halting its data flow.  When the input queue has been processed, a
    * START command is sent to the serial device which should cause it to
    * resume putting bytes onto the input queue.
    *
    * @pre The serial port is open.
    * @post The current state of start-stop input control is returned to the
    *        caller.
    *
    * @return true if start-stop input control is enabled; false otherwise
    */
   bool getStartStopInputState();

   /**
    * Enables or disables start-stop input control.  See
    * getStartStopInputState() for more information.
    *
    * @pre The serial port is open.
    * @post Start-stop input control is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see getStartStopInputState
    */
   vpr::ReturnStatus setStartStopInput(const bool& flag);

   /**
    * Gets the state of start-stop output control.  When enabled, when the
    * terminal driver receives a STOP command, output stops.  When it
    * receives a START command, output resumes.  If disabled, these
    * commands are read as normal characters.
    *
    * @pre The serial port is open.
    * @post The current state of start-stop output control is returned to
    *        the caller.
    *
    * @return true if start-stop output control is enabled; false otherwise.
    */
   bool getStartStopOutputState();

   /**
    * Enables or disables start-stop output control.  See
    * getStartStopOutputState() for more information.
    *
    * @pre The serial port is open.
    * @post Start-stop output control is ensabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see getStartStopOutputState
    */
   vpr::ReturnStatus setStartStopOutput(const bool& flag);

   /**
    * Gets the current state of parity generation for outgoing bytes and
    * parity checking for incoming bytes.
    *
    * @pre The serial port is open.
    * @post The current parity generation/checking state is returned to the
    *        caller.
    *
    * @return true if parity generation and checking are enabled for
    *         outgoing and incoming bytes respectively.  false is returned
    *         if parity generation and checking are disabled.
    */
   bool getParityGenerationState();

   /**
    * Enables or disables parity generation for outgoing bytes and parity
    * checking for incoming bytes.
    *
    * @pre The serial port is open.
    * @post Parity generation and checking are enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setParityGeneration(const bool& flag);

   /**
    * Returns the current state of parity error marking for this serial port.
    *
    * @pre This serial port is open.
    *
    * @return true is returned if parity error marking is enabled.  false is
    *         returned if parity error masking is disabled.
    */
   bool getParityErrorMarkingState();

   /**
    * Enables marking of bytes with parity errors or framing errors (except
    * BREAKs).  This is only active if input parity and framing error
    * reporting is enabled (see setInputParityCheck() for more
    * information).  The mark is the three-byte sequence \377 \0 X where X
    * is the byte received in error.  If bit stripping is enabled, a valid
    * \377 byte is passed as the two-byte sequence \377 \377.
    *
    * @pre This serial port is open.
    * @post Bytes with an error are marked and passed on to the reader.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see setInputParityCheck
    */
   vpr::ReturnStatus setParityErrorMarking(const bool& flag);

   /**
    * Gets the current parity checking type (either odd or even).
    *
    * @pre The serial port is open.
    * @post The parity type is returned to the caller.
    *
    * @return A vpr::SerialTypes::ParityType value giving the parity
    *           checking being done.
    */
   SerialTypes::ParityType getParity();

   /**
    * Enables odd or event parity.
    *
    * @pre The serial port is open.
    * @post Odd or even parity is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setParity(const SerialTypes::ParityType& type);

   /**
    * Gets the current input baud rate.
    *
    * @pre The serial port is open.
    * @post The current input baud rate is returned to the caller via the
    *        by-reference parameter.
    *
    * @param rate  A reference to a vpr::Uint32 to be used as storage for
    *               the current input baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getInputBaudRate(vpr::Uint32& rate);

   /**
    * Sets the current input baud rate.
    *
    * @pre The serial port is open, and the rate is a valid baud rate.
    * @post The input baud rate is updated to use the given value.
    *
    * @param rate  The new input baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setInputBaudRate(const vpr::Uint32& rate);

   /**
    * Gets the current output baud rate.
    *
    * @pre The serial port is open.
    * @post The current output baud rate is returned to the caller via the
    *        by-reference parameter.
    *
    * @param rate  A reference to a vpr::Uint32 to be used as storage for
    *               the current output baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getOutputBaudRate(vpr::Uint32& rate);

   /**
    * Sets the current output baud rate.
    *
    * @pre The serial port is open, and the rate is a valid baud rate.
    * @post The output baud rate is updated to use the given value.
    *
    * @param rate  The new output baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setOutputBaudRate(const vpr::Uint32& rate);

   /**
    * Waits for all output to be transmitted.
    *
    * @pre The serial port is open.
    * @post All output is transmitted.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus drainOutput();

   /**
    * Alters the input or output flow control.  Based on the
    * vpr::SerialTypes::FlowActionOption argument, output can be suspended
    * and restarted or the terminal device can be told to stop or to resume
    * sending data.
    *
    * @pre The serial device is open.
    * @post Based on the argument value, the input or output flow control is
    *        updated.
    *
    * @param opt  The flow control change.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus controlFlow(vpr::SerialTypes::FlowActionOption opt);

   /**
    * Returns the current enable state of hardware flow control.
    *
    * @pre This serial port is open.
    *
    * @return true is returned if hardware flow control is enabled.
    *         false is returned otherwise.
    *
    * @see controlFlow
    */
   bool getHardwareFlowControlState();

   /**
    * Attempts to enable "hardware" flow control.  While some documents
    * recommend the use of hardware flow control over the use of software
    * flow control, not all platforms implement hardware flow control.  Hence,
    * this may have no effect depending on the operating system.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see controlFlow
    */
   vpr::ReturnStatus setHardwareFlowControl(const bool& flag);

   /**
    * Discards either the input buffer (unread data received from the
    * terminal device) or the output buffer (data written but not yet
    * transmitted to the terminal device).  The argument tells which queue
    * (or queues) to flush.
    *
    * @pre The serial device is open.
    * @post Based on the argument, the appropriate queue is flushed of its
    *        current contents.
    *
    * @param queue  The queue (or queues) to be flushed.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus flushQueue(vpr::SerialTypes::FlushQueueOption queue);

   /**
    * Transmits a continuous stream of zero bits for the given duration.  If
    * the argument is 0, the transmission will last between 0.25 and 0.5
    * seconds.  Otherwise, the duration specfies the number of seconds to
    * send the zero bit stream.
    *
    * @pre The serial device is open.
    * @post A stream of zero bits are sent to the serial device for the
    *        given duration.
    *
    * @param duration The length of time to send a zero bit stream to the
    *                 serial device.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus sendBreak(const vpr::Int32 duration);

    /**
    * Return the status of the carrier detect signal.
    * @return - May be platform dependent, but will at least be as follows.
    *           0 - not high, 1 - high, -1 - Not supported
    */
   int getCarrierDetect()
   {
      return -1;
   }

   /**
    * Return the status of the data set ready line.
    * @return - May be platform dependent, but will at least be as follows.
    *           0 - not high, 1 - high, -1 - Not supported
    */
   int getDataSetReady()
   {
      return -1;
   }

   /**
    * Return the status of the clear to send.
    * @return - May be platform dependent, but will at least be as follows.
    *           0 - not high, 1 - high, -1 - Not supported
    */
   int getClearToSend()
   {
      return -1;
   }

   /**
    * Return the status of the ring indicator line.
    * @return - May be platform dependent, but will at least be as follows.
    *           0 - not high, 1 - high, -1 - Not supported
    */
   int getRingIndicator()
   {
      return -1;
   }

   /** Sets the data terminal ready line. */
   vpr::ReturnStatus setDataTerminalReady(bool val)
   {
      return vpr::ReturnStatus::Fail;
   }

   /** Sets the ready to send line. */
   vpr::ReturnStatus setRequestToSend(bool val)
   {
      return vpr::ReturnStatus::Fail;
   }

   /**
    * Implementation of the <code>read</code> template method.  This reads at
    * most the specified number of bytes from the serial port into the given
    * buffer.
    *
    * @pre The port is open for reading, and the buffer is at least
    *      <code>length</code> bytes long.
    * @post The given buffer has length bytes copied into it from the port,
    *       and the number of bytes read successfully is returned to the
    *       caller via the <code>bytes_read</code> parameter.
    *
    * @param buffer     A pointer to the buffer where the port's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the port is in non-blocking mode,
    *         and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not begin
    *         within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   vpr::ReturnStatus read_i(void* buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Implementation of the <code>readn</code> template method.  This reads
    * exactly the specified number of bytes from the serial port into the
    * given buffer.
    *
    * @pre The port is open for reading, and the buffer is at least
    *      <code>length</code> bytes long.
    * @post The given buffer has <code>length</code> bytes copied into
    *       it from the port, and the number of bytes read successfully
    *       is returned to the caller via the <code>bytes_read</code>
    *       parameter.
    *
    * @param buffer     A pointer to the buffer where the ports's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the port is in non-blocking mode,
    *         and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not begin
    *         within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   vpr::ReturnStatus readn_i(void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_read,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Implementation of the <code>write</code> template method.  This writes
    * the given buffer to the serial port.
    *
    * @pre The port is open for writing.
    * @post The given buffer is written to the I/O port, and the number
    *       of bytes written successfully is returned to the caller via the
    *       <code>bytes_written</code> parameter.
    *
    * @param buffer        A pointer to the buffer to be written.
    * @param length        The length of the buffer.
    * @param bytes_written The number of bytes written to the port.
    * @param timeout       The maximum amount of time to wait for data to be
    *                      available for writing.  This argument is optional
    *                      and defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock is returned if the handle is in
    *         non-blocking mode, and the write operation could not be
    *         completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the write operation failed.
    */
   vpr::ReturnStatus write_i(const void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_written,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

protected:
   HANDLE mHandle;     /**< handle to communication file */
   DWORD mOpenFlag;    /**< flag to specify how to open comm port */
   std::string mName;  /**< name of communication device */
   bool mOpen;
   DWORD mBlocking;    /**< flag for blocking I/O */
   bool mParityMark;   /**< flag for parity marking */
   vpr::Uint8 mCurrentTimeout;
};

} // End of vpr namespace


#endif  /* _VPR_SERIAL_PORT_IMPL_WIN32_H_ */
