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
#include "SkyWatcherTelescope.h"

#include <arduino.h>

//#define DEBUG 2
#define DEBUGSerial Serial2 

SkyWatcherTelescope::SkyWatcherTelescope(TelescopeAxis *RAA, TelescopeAxis *DECA, MountConfig *C)
{
  RAAxis = RAA;
  DECAxis = DECA;
  Config = C; 
}

void SkyWatcherTelescope::MountControl(char *CommandBuffer, char *Response)
{
  switch (CommandBuffer[0])
  {
    case 'a' : ReportStepsPerAxisRevolution(CommandBuffer, Response);
      break;

    case 'b' : ReportTimerInterruptFrequency(CommandBuffer, Response);
      break;

    case 'e' : ReportVersion(CommandBuffer, Response);
      break;

    case 'f' : ReportAxisStatus(CommandBuffer, Response);
      break;

    case 'g' : ReportHighSpeedRatio(CommandBuffer, Response);
      break;

    case 'j' : ReportPosition(CommandBuffer, Response);
      break;

    case 's' : ReportStepsPerWormRev(CommandBuffer, Response);
      break;

    case 'E' : SetAxisPosition(CommandBuffer, Response);
      break;

    case 'F' : InitialiseAxis(CommandBuffer, Response);
      break;

    case 'G' : SetAxisMotionMode(CommandBuffer, Response);
      break;

    case 'H' : Goto(CommandBuffer, Response);
      break;

    case 'I' : SetTrackingSpeed(CommandBuffer, Response);
      break;

    case 'J' : StartMotion(CommandBuffer, Response);
      break;

    case 'K' : StopAxis(CommandBuffer, Response);
      break;

    case 'L' : StopNow(CommandBuffer, Response);
      break;

    case '0' : ShowCommands(Response);
      break;
        
    case '1' : ReportHardwareConfiguration(Response);
      break;
    
    case '2' : ReportAxisConfiguration(Response);
      break;
    
    case '3' : WriteMountConfig(Response);
      break;

    case '4' : SetMotorSteps(CommandBuffer, Response);
      break;
    
    case '5' : SetWormRatio(CommandBuffer, Response);
      break;  

    case '6' : SetGOTOSpeed(CommandBuffer, Response);
      break;  

    case '7' : SetStepInvert(CommandBuffer, Response);
      break;

    case '8' : SetDirectionInvert(CommandBuffer, Response);
      break;
    
    case '9' : SetEnableInvert(CommandBuffer, Response);
      break;
      
    default  : CommandOK(Response, 0); CommandEND(Response, 1);
      break;
  }
}

void SkyWatcherTelescope::Run(void)
{
  RAAxis->Run();
  DECAxis->Run();
}

void SkyWatcherTelescope::ReportVersion(char *Command, char *Response)
{
  strcpy(Response, ("=010500"));
  CommandEND(Response, 7);
}

void SkyWatcherTelescope::ReportHighSpeedRatio(char *Command, char *Response)
{
  strcpy(Response, ("=10"));
  CommandEND(Response, 3);
}

void SkyWatcherTelescope::ReportStepsPerWormRev(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config->GetRAStepsPerWormRev());
      CommandEND(Response, 7);
      break;

    case '2' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config->GetDECStepsPerWormRev());
      CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void SkyWatcherTelescope::ReportPosition(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, RAAxis->currentPosition());
      CommandEND(Response, 7);
      break;

    case '2' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, DECAxis->currentPosition());
      CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}



void SkyWatcherTelescope::SetTrackingSpeed(char *Command, char *Response)
{
  long Divisor = 0;
  float Speed = 0;

#if DEBUG >=1
  DEBUGSerial.println("Set Tracking Speed ");
#endif

  switch (Command[1] )
  {
    case '1' : Divisor = HexToLong_Short(Command, 2);
      if (Divisor != 0)
      {
        Speed = (float)64932 / (float)Divisor;
        if (RAAxis->getDirection() == 0)
        {
          Speed = -1 * Speed;
        }
        RAAxis->setSpeed(Speed);
        //RASpeed = Speed;
      }
#if DEBUG >=1
      DEBUGSerial.println("\t\tNew Tracking Speed "); DEBUGSerial.println((int)(Speed*100));
#endif

      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : Divisor = HexToLong_Short(Command, 2);
      if (Divisor != 0)
      {
        Speed = (float)64932 / (float)Divisor;
        if (DECAxis->getDirection() == 0)
        {
          Speed = -1 * Speed;
        }
        DECAxis->setSpeed(Speed);
        //DECSpeed = Speed;
      }
#if DEBUG >=1
      DEBUGSerial.print("\t\tNew Tracking Speed "); DEBUGSerial.println((int)(Speed*100));
#endif

      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      return;
      break;
  }
}


void SkyWatcherTelescope::ReportStepsPerAxisRevolution(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config->GetRAStepsPerAxisRev());
      CommandEND(Response, 7);
      break;

    case '2' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config->GetDECStepsPerAxisRev());
      CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}



void SkyWatcherTelescope::ReportTimerInterruptFrequency(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : strcpy(Response, ("=A7FD00")); CommandEND(Response, 7);
      break;

    case '2' : strcpy(Response, ("=A7FD00")); CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SkyWatcherTelescope::StopAxis(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RAAxis->stopAxis(FALSE);
#if DEBUG >=1
      DEBUGSerial.println("\t\tStop RA Axis");
#endif
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    case '2' : DECAxis->stopAxis(FALSE);
#if DEBUG >=1
      DEBUGSerial.println("\t\tStop DEC Axis");
#endif

      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SkyWatcherTelescope::StopNow(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RAAxis->stopAxis(TRUE);
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    case '2' : DECAxis->stopAxis(TRUE);
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SkyWatcherTelescope::InitialiseAxis(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RAAxis->enableOutputs();
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    case '2' : DECAxis->enableOutputs();
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SkyWatcherTelescope::ReportAxisStatus(char *Command, char *Response)
{
  switch (Command[1])
  {
    case '1' : CommandOK(Response, 0);

      if (RAAxis->isEnabled() == 1)
        Response[3] = '1';
      else
        Response[3] = '0';

      if (RAAxis->getMotionMode() == 1)
        Response[2] = '1';
      else
        Response[2] = '0';

      if (RAAxis->getDirection() == 1)
      {
        if (RAAxis->isStopped())
          Response[1] = '1';
        else
          Response[1] = '0';
      }
      else
      {
        if (RAAxis->isStopped())
          Response[1] = '3';
        else
          Response[1] = '2';
      }

#if DEBUG >=3
      DEBUGSerial.print("\t\t");
      DEBUGSerial.print(" Direction "); DEBUGSerial.print(RAAxis->getDirection());
      DEBUGSerial.print(" Stopped "); DEBUGSerial.print(RAAxis->isStopped());
      DEBUGSerial.print(" Motion Mode "); DEBUGSerial.print(RAAxis->getMotionMode());
      DEBUGSerial.print(" Enabled "); DEBUGSerial.println(RAAxis->isEnabled());
#endif
      CommandEND(Response, 4);
      break;

    case '2' : CommandOK(Response, 0);

      if (DECAxis->isEnabled() == 1)
        Response[3] = '1';
      else
        Response[3] = '0';

      if (DECAxis->getMotionMode() == 1)
        Response[2] = '1';
      else
        Response[2] = '0';

      if (DECAxis->getDirection() == 1)
      {
        if (DECAxis->isStopped())
          Response[1] = '1';
        else
          Response[1] = '0';
      }
      else
      {
        if (DECAxis->isStopped())
          Response[1] = '3';
        else
          Response[1] = '2';
      }

#if DEBUG >=3
      DEBUGSerial.print("\t\t");
      DEBUGSerial.print(" Direction "); DEBUGSerial.print(DECAxis->getDirection());
      DEBUGSerial.print(" Stopped "); DEBUGSerial.print(DECAxis->isStopped());
      DEBUGSerial.print(" Motion Mode "); DEBUGSerial.print(DECAxis->getMotionMode());
      DEBUGSerial.print(" Enabled "); DEBUGSerial.println(DECAxis->isEnabled());
#endif
      CommandEND(Response, 4);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SkyWatcherTelescope::SetAxisPosition(char *Command, char *Response)
{
  long Position = 0;

  switch (Command[1] )
  {
    case '1' : Position = HexToLong_Short(Command, 2);
      RAAxis->setCurrentPosition(Position);
#if DEBUG >=2
      DEBUGSerial.print("\t\tNew Position "); DEBUGSerial.println(Position, HEX);
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : Position = HexToLong_Short(Command, 2);
      DECAxis->setCurrentPosition(Position);
#if DEBUG >=2
      DEBUGSerial.print("\t\tNew Position "); DEBUGSerial.println(Position, HEX);
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SkyWatcherTelescope::StartMotion(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RAAxis->startAxis();
#if DEBUG >=1
      DEBUGSerial.println("\t\tStart RA Axis");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : DECAxis->startAxis();
#if DEBUG >=1
      DEBUGSerial.print("\t\tStart DEC Axis");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void SkyWatcherTelescope::SetAxisMotionMode(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : //
#if DEBUG >=1
      DEBUGSerial.print("\t\tRA Axis");
#endif

      if (Command[2] == '0')
      {
#if DEBUG >=1
        DEBUGSerial.print(" High Speed : GOTO");
#endif
        RAAxis->setMotionMode(TelescopeAxis::GOTO);
        RAAxis->setMaxSpeed(Config->GetRAGotoSpeed());
      }
      else if (Command[2] == '2')
      {
#if DEBUG >=1
        DEBUGSerial.print(" Low Speed : GOTO");
#endif
        RAAxis->setMotionMode(TelescopeAxis::GOTO);
        RAAxis->setMaxSpeed(Config->GetDECGotoSpeed());
      }
      else if (Command[2] == '1')
      {
#if DEBUG >=1
        DEBUGSerial.print(" Low Speed : SLEW");
#endif
        RAAxis->setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          RAAxis->setSpeed(Config->GetRASlewSpeed());
        else
          RAAxis->setSpeed(-1 * Config->GetRASlewSpeed());
      }
      else if (Command[2] == '3')
      {
#if DEBUG >=1
        DEBUGSerial.print(" High Speed : SLEW");
#endif
        RAAxis->setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          RAAxis->setSpeed(Config->GetRASlewSpeed());
        else
          RAAxis->setSpeed(-1 * Config->GetRASlewSpeed());
      }

      if (Command[3] == '0')
      {
        RAAxis->setDirection(1);
#if DEBUG >=1
        DEBUGSerial.print(" Clockwise");
#endif
      }
      else
      {
        RAAxis->setDirection(0);
#if DEBUG >=1
        DEBUGSerial.print(" Anti-clockwise");
#endif
      }
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : //
#if DEBUG >=1
      DEBUGSerial.print("\t\tDEC Axis");
#endif
      if (Command[2] == '0')
      {
        DECAxis->setMotionMode(TelescopeAxis::GOTO);
        DECAxis->setMaxSpeed(Config->GetDECGotoSpeed());
#if DEBUG >=1
        DEBUGSerial.print(" High Speed : GOTO");
#endif
      }
      else if (Command[2] == '2')
      {
        DECAxis->setMotionMode(TelescopeAxis::GOTO);
        DECAxis->setMaxSpeed(Config->GetDECSlewSpeed());
#if DEBUG >=1
        DEBUGSerial.print(" Low Speed : GOTO");
#endif
      }
      else if (Command[2] == '1')
      {
        DECAxis->setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          DECAxis->setSpeed(Config->GetDECSlewSpeed());
        else
          DECAxis->setSpeed(-1 * Config->GetDECSlewSpeed());
#if DEBUG >=1
        DEBUGSerial.print(" Low Speed : SLEW");
#endif
      }
      else if (Command[2] == '3')
      {
        DECAxis->setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          DECAxis->setSpeed(Config->GetDECSlewSpeed());
        else
          DECAxis->setSpeed(-1 * Config->GetDECSlewSpeed());
#if DEBUG >=1
        DEBUGSerial.print(" High Speed : SLEW");
#endif
      }

      if (Command[3] == '0')
      {
        DECAxis->setDirection(1);
#if DEBUG >=1
        DEBUGSerial.print(" Clockwise");
#endif
      }
      else
      {
        DECAxis->setDirection(0);
#if DEBUG >=1
        DEBUGSerial.print(" Anti-Clockwise");
#endif
      }
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void SkyWatcherTelescope::Goto(char *Command, char *Response)
{
  long Position = 0;
  long Offset = 0;

  switch (Command[1] )
  {
    case '1' : Offset = HexToLong_Short(Command, 2);

      if (RAAxis->getDirection() == 1)
        Position = RAAxis->currentPosition() + Offset;
      else
        Position = RAAxis->currentPosition() - Offset;

      RAAxis->moveTo(Position);
#if DEBUG >=2
      DEBUGSerial.print("\t\tRA Goto "); DEBUGSerial.print(Position, HEX);
      DEBUGSerial.print(" ");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : Offset = HexToLong_Short(Command, 2);

      if (DECAxis->getDirection() == 1)
        Position = DECAxis->currentPosition() + Offset;
      else
        Position = DECAxis->currentPosition() - Offset;

      DECAxis->moveTo(Position);
#if DEBUG >=2
      DEBUGSerial.print("\t\tDEC Goto "); DEBUGSerial.print(Position, HEX);
      DEBUGSerial.print(" ");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void SkyWatcherTelescope::CommandOK(char *Buffer, byte offset)
{
  *(Buffer + offset) = '=';
}

void SkyWatcherTelescope::CommandError(char *Buffer, byte offset)
{
  *(Buffer + offset) = '!';
}

void SkyWatcherTelescope::CommandEND(char *Buffer, byte offset)
{
  *(Buffer + offset) = 0x0D;
  *(Buffer + offset + 1) = '\0';
}



const char HexLookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void SkyWatcherTelescope::NibbleToHex(char *Buffer, byte offset, byte Nibble)
{
  Buffer[offset] = HexLookup[Nibble & 0x0f];
}

void SkyWatcherTelescope::ByteToHex(char *Buffer, byte offset, byte Byte)
{
  NibbleToHex(Buffer, offset + 1, Byte & 0x0f);
  NibbleToHex(Buffer, offset, (Byte >> 4) & 0x0f);
}

void SkyWatcherTelescope::UIntToHex(char *Buffer, byte offset, unsigned int UInt)
{
  ByteToHex(Buffer, offset, UInt & 0xff);
  ByteToHex(Buffer, offset + 2, (UInt >> 8) & 0xff);
}

void SkyWatcherTelescope::UIntToHex_Short(char *Buffer, byte offset, unsigned int UInt)
{
  NibbleToHex(Buffer, offset, UInt & 0x0f);
  ByteToHex(Buffer, offset + 1, (UInt >> 8) & 0xff);
}


void SkyWatcherTelescope::ULongToHex(char *Buffer, byte offset, unsigned long ULong)
{
  UIntToHex(Buffer, offset, ULong & 0xffff);
  UIntToHex(Buffer, offset + 4, (ULong >> 16) & 0xffff);
}

void SkyWatcherTelescope::ULongToHex_Short(char *Buffer, byte offset, unsigned long ULong)
{
  UIntToHex(Buffer, offset, ULong & 0xffff);
  ByteToHex(Buffer, offset + 4, (ULong >> 16) & 0xff);

}

long SkyWatcherTelescope::HexToLong(char *Buffer, byte offset)
{
  long result = 0;

  return result;
}

long SkyWatcherTelescope::HexToLong_Short(char *Buffer, byte offset)
{
  long temp = 0;

  temp = HexToNibble(Buffer[offset + 4]); temp = temp << 4;
  temp += HexToNibble(Buffer[offset + 5]);  temp = temp << 4;
  temp += HexToNibble(Buffer[offset + 2]); temp = temp << 4;
  temp += HexToNibble(Buffer[offset + 3]);  temp = temp << 4;
  temp += HexToNibble(Buffer[offset + 0]); temp = temp << 4;
  temp += HexToNibble(Buffer[offset + 1]);

  return temp;
}


int SkyWatcherTelescope::HexToNibble(char c)
{
  switch (c)
  {
    case '0' : return 0;
    case '1' : return 1;
    case '2' : return 2;
    case '3' : return 3;
    case '4' : return 4;
    case '5' : return 5;
    case '6' : return 6;
    case '7' : return 7;
    case '8' : return 8;
    case '9' : return 9;
    case 'A' : return 10;
    case 'B' : return 11;
    case 'C' : return 12;
    case 'D' : return 13;
    case 'E' : return 14;
    case 'F' : return 15;
  }
  return -1;
}

void SkyWatcherTelescope::ReportAxisConfiguration(char *Response)
{
  static char format[] = "\r\nParameter\tR.A.\tDeclination\r\n"
                "Motor Steps\t%ld\t%ld\r\n"
                "Worm Ratio\t%ld\t%ld\r\n"
                "GOTO Speed\t%ld\t%ld\r\n";

  sprintf(Response,format,
                  Config->GetRAStepsPerMotorRev(), Config->GetDECStepsPerMotorRev(),
                  Config->GetRAWormRatio(), Config->GetDECWormRatio(),
                  (uint32_t)Config->GetRAGotoSpeed(), (uint32_t)Config->GetDECGotoSpeed()
          );
  
}

void SkyWatcherTelescope::ReportHardwareConfiguration(char *Response)
{
  static char format[] = "\r\nParameter\tR.A.\tDeclination\r\n"
                "Step Pin\t%d\t%d\r\n"
                "Step Invert\t%d\t%d\r\n"
                "Direction Pin\t%d\t%d\r\n"
                "Direc'n Invert\t%d\t%d\r\n"
                "Enable Pin\t%d\t%d\r\n"
                "Enable Invert\t%d\t%d\r\n"
                
                ;

  sprintf(Response,format,
                  Config->GetRAStepPin(),Config->GetDECStepPin(),
                  Config->GetRAStepInvert(),Config->GetDECStepInvert(),
                  Config->GetRADirectionPin(),Config->GetDECDirectionPin(),
                  Config->GetRADirectionInvert(),Config->GetDECDirectionInvert(),
                  Config->GetRAEnablePin(),Config->GetDECEnablePin(),
                  Config->GetRAEnableInvert(),Config->GetDECEnableInvert()
                  );
  
}

void SkyWatcherTelescope::WriteMountConfig(char *Response)
{
  Config->WriteMountConfig();
  strcpy(Response,"OK\r\n");
}

void SkyWatcherTelescope::SetGOTOSpeed(char *CommandBuffer, char *Response)
{
  long _Speed;
  _Speed = atol(&CommandBuffer[3]);

  if (CommandBuffer[2] == '1')
    Config->SetRAGotoSpeed(_Speed);
  else if (CommandBuffer[2] == '2')
    Config->SetDECGotoSpeed(_Speed);
  else
  {
      strcpy(Response,"Err\r\n");
      return;
  }

  strcpy(Response,"OK\r\n");
}

void SkyWatcherTelescope::SetMotorSteps(char *CommandBuffer, char *Response)
{
  long _Steps;
  _Steps = atol(&CommandBuffer[3]);

  if (CommandBuffer[2] == '1')
    Config->SetRAStepsPerMotorRev(_Steps);
  else if (CommandBuffer[2] == '2')
    Config->SetDECStepsPerMotorRev(_Steps);
  else
  {
      strcpy(Response,"Err\r\n");
      return;
  }

  strcpy(Response,"OK\r\n");  
}

void SkyWatcherTelescope::SetWormRatio(char *CommandBuffer, char *Response)
{
  long _Ratio;
  _Ratio = atol(&CommandBuffer[3]);

  if (CommandBuffer[2] == '1')
    Config->SetRAWormRatio(_Ratio);
  else if (CommandBuffer[2] == '2')
    Config->SetDECWormRatio(_Ratio);
  else
  {
      strcpy(Response,"Err\r\n");
      return;
  }

  strcpy(Response,"OK\r\n");
}

void SkyWatcherTelescope::SetStepInvert(char *CommandBuffer, char *Response)
{
  uint8_t Invert;
  Invert = (uint8_t)atoi(&CommandBuffer[3]);

  if (CommandBuffer[2] == '1')
    Config->SetRAStepInvert(Invert);
  else if (CommandBuffer[2] == '2')
    Config->SetDECStepInvert(Invert);
  else
  {
      strcpy(Response,"Err\r\n");
      return;
  }

  strcpy(Response,"OK\r\n");
}

void SkyWatcherTelescope::SetDirectionInvert(char *CommandBuffer, char *Response)
{
  uint8_t Invert;
  Invert = (uint8_t)atoi(&CommandBuffer[3]);

  if (CommandBuffer[2] == '1')
    Config->SetRADirectionInvert(Invert);
  else if (CommandBuffer[2] == '2')
    Config->SetDECDirectionInvert(Invert);
  else
  {
      strcpy(Response,"Err\r\n");
      return;
  }

  strcpy(Response,"OK\r\n");
}

void SkyWatcherTelescope::SetEnableInvert(char *CommandBuffer, char *Response)
{
  uint8_t Invert;
  Invert = (uint8_t)atoi(&CommandBuffer[3]);

  if (CommandBuffer[2] == '1')
    Config->SetRAEnableInvert(Invert);
  else if (CommandBuffer[2] == '2')
    Config->SetDECEnableInvert(Invert);
  else
  {
      strcpy(Response,"Err\r\n");
      return;
  }

  strcpy(Response,"OK\r\n");
}





void SkyWatcherTelescope::ShowCommands(char *Response)
{
  strcpy(Response,("0 Help\r\n"
                   "1 Hardware Setup\r\n"
                   "2 Axis Setup\r\n"
                   "3 Save\r\n"
                   "4 Set Motor Steps\r\n"
                   "5 Set Worm Ratio\r\n"
                   "6 Set Goto Speed\r\n"
                   "7 Set Step Invert\r\n"
                   "8 Set Direc' Invert\r\n"
                   "9 Set Enable Invert\r\n"));
}

