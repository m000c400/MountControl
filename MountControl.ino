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
#include <arduino.h>
#include <stdint.h>

#include <AccelStepper.h>
#include <EEPROM.h>
#include <IRremote.h>

#include "TelescopeAxis.h"
#include "PinAssignments.h"
#include "MountConfig.h"
#include "SkyWatcherTelescope.h"

#define SerialPort1 Serial
//#define SerialPort2 Serial

#define COMMANDBUFFERSIZE 40
#define RESPONSEBUFFERSIZE 300
#define TRUE 1
#define FALSE 0

char COMMANDEND = 0x0D;

TelescopeAxis *RA_Motor;
TelescopeAxis *DEC_Motor;
SkyWatcherTelescope *Telescope;
MountConfig *Config;

void setup()
{
  Config = new MountConfig();
  
  Config->LoadMountConfig();
  
  SerialPort1.begin(9600);
  
  #ifdef SerialPort2
    SerialPort2.begin(9600);
  #endif

  pinMode(STATUSPIN, OUTPUT);

  RA_Motor = new TelescopeAxis( Config->GetRAStepPin(), Config->GetRADirectionPin(), Config->GetRAEnablePin(), Config->GetRAnSleepPin(), Config->GetRAnResetPin(), 
                                Config->GetRAStepInvert(), Config->GetRADirectionInvert(), Config->GetRAEnableInvert());
  
  DEC_Motor = new TelescopeAxis( Config->GetDECStepPin(), Config->GetDECDirectionPin(), Config->GetDECEnablePin(), Config->GetDECnSleepPin(), Config->GetDECnResetPin(), 
                                Config->GetDECStepInvert(), Config->GetDECDirectionInvert(), Config->GetDECEnableInvert());

  RA_Motor->setMaxSpeed(Config->GetRAGotoSpeed()); RA_Motor->setAcceleration(4*Config->GetRAGotoSpeed());
  DEC_Motor->setMaxSpeed(Config->GetDECGotoSpeed()); DEC_Motor->setAcceleration(4*Config->GetDECGotoSpeed());

  Telescope = new SkyWatcherTelescope(RA_Motor, DEC_Motor, Config);  
}

void loop()
{
  ProcessSerialPort1();
  
  #ifdef SerialPort2
  //ProcessSerialPort2();
  #endif
  
  Telescope->Run();
}


void ToggleStatusPin(void)
{
  static int8_t Status = HIGH;

  digitalWrite(STATUSPIN, Status);

  Status = !Status;
}




void ProcessSerialPort1(void)
{
  static char Response[RESPONSEBUFFERSIZE];
  static char CommandBuffer[COMMANDBUFFERSIZE + 1];
  static int16_t  BufferUsed = 0;

  while (SerialPort1.available())
  {
    if (BufferUsed < COMMANDBUFFERSIZE)
    {
      SerialPort1.readBytes(&CommandBuffer[BufferUsed], 1);
      if (CommandBuffer[BufferUsed] == ':')
      {
        BufferUsed = 0;
      }
      else if (CommandBuffer[BufferUsed] == COMMANDEND)
      {
        CommandBuffer[BufferUsed] = '\0'; ToggleStatusPin();
        Telescope->MountControl(CommandBuffer, Response);
        SerialPort1.print(Response);
        BufferUsed = 0;
      }
      else
      {
         BufferUsed++;       
      }
    }
    else
    {
      BufferUsed = 0;
    }
  }
}

#ifdef SerialPort2
void ProcessSerialPort2(void)
{
  static char Response[RESPONSEBUFFERSIZE];
  static char CommandBuffer[COMMANDBUFFERSIZE + 1];
  static int16_t  BufferUsed = 0;

  while (SerialPort2.available())
  {
    if (BufferUsed < COMMANDBUFFERSIZE)
    {
      SerialPort2.readBytes(&CommandBuffer[BufferUsed], 1);
      if (CommandBuffer[BufferUsed] == ':')
      {
        BufferUsed = 0;
      }
      else if (CommandBuffer[BufferUsed] == COMMANDEND)
      {
        CommandBuffer[BufferUsed] = '\0'; ToggleStatusPin();
        Telescope->MountControl(CommandBuffer, Response);
        SerialPort2.print(Response);
        BufferUsed = 0;
      }
      else
      {
         BufferUsed++;       
      }
    }
    else
    {
      BufferUsed = 0;
    }
  }
}

#endif
