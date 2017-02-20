/* 
    GEM Control System
    Copyright (C) <2015>  <Mike Roberts>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SKYWATCHERTELESCOPE_h
#define SKYWATCHERTELESCOPE_h

#include <stdint.h>

#include "TelescopeAxis.h"
#include "MountConfig.h"

#define TRUE 1
#define FALSE 0

class SkyWatcherTelescope
{
	public:
		SkyWatcherTelescope(TelescopeAxis *RAA, TelescopeAxis *DECA, MountConfig *C);
                void MountControl(char *CommandBuffer, char *Response);   
                void Run(void);
    
	protected:
				
	private:
                TelescopeAxis *RAAxis;
                TelescopeAxis *DECAxis;
                MountConfig *Config;

    void ReportVersion(char *Command, char *Response);
    void ReportHighSpeedRatio(char *Command, char *Response);
    void ReportStepsPerWormRev(char *Command, char *Response);
    void ReportStepsPerAxisRevolution(char *Command, char *Response);
    void ReportTimerInterruptFrequency(char *Command, char *Response);
    void ReportAxisStatus(char *Command, char *Response);
    void ReportPosition(char *Command, char *Response);
    void SetAxisPosition(char *Command, char *Response);
    void InitialiseAxis(char *Command, char *Response);
    void SetAxisMotionMode(char *Command, char *Response);
    void Goto(char *Command, char *Response);
    void SetTrackingSpeed(char *Command, char *Response);
    void StartMotion(char *Command, char *Response);
    void StopAxis(char *Command, char *Response);
    void StopNow(char *Command, char *Response);
    void CommandOK(char *Buffer, byte offset);
    void CommandEND(char *Buffer, byte offset);
    void CommandError(char *Buffer, byte offset);
    void NibbleToHex(char *Buffer, byte offset, byte Nibble);
    void ByteToHex(char *Buffer, byte offset, byte Byte);
    void UIntToHex(char *Buffer, byte offset, unsigned int UInt);
    void UIntToHex_Short(char *Buffer, byte offset, unsigned int UInt);
    void ULongToHex(char *Buffer, byte offset, unsigned long ULong);
    void ULongToHex_Short(char *Buffer, byte offset, unsigned long ULong);
    long HexToLong(char *Buffer, byte offset);
    long HexToLong_Short(char *Buffer, byte offset);
    int HexToNibble(char c);

    
    void ReportHardwareConfiguration(char * Response);
    void ReportAxisConfiguration(char * Response);
    void WriteMountConfig(char *Response);
    void SetGOTOSpeed(char *CommandBuffer, char *Response);
    void SetMotorSteps(char *CommandBuffer, char *Response);
    void SetWormRatio(char *CommandBuffer, char *Response);
    void ShowCommands(char *Response);
    void SetStepInvert(char *CommandBuffer, char *Response);
    void SetDirectionInvert(char *CommandBuffer, char *Response);
    void SetEnableInvert(char *CommandBuffer, char *Response);

};
#endif 

