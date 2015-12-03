#include <AccelStepper.h>
#include <EEPROM.h>


#include "TelescopeAxis.h"
#include "PinAssignments.h"
#include "MountConfig.h"
#include "IRCodes.h"

//#define DEBUG 1
//#define IR
//#define DUALPORT


#define BUFFERSIZE 80
#define TRUE 1
#define FALSE 0

char COMMANDEND = 0x0D;


#ifdef IR
//#include <IRremote.h>
IRrecv IRRx(IR_RX);
decode_results IRResults;
unsigned long IRTimeOut;
int IRTrack = false;
int IRSlew = false;
#define IRTIMEOUT 500
#endif

TelescopeAxis RA_Motor( RA_STEP, RA_DIRECTION, RA_nENABLE, RA_nRESET, RA_nSLEEP, RA_MODE2, RA_MODE1, RA_MODE0);
TelescopeAxis DEC_Motor(DEC_STEP, DEC_DIRECTION, DEC_nENABLE, DEC_nRESET, DEC_nSLEEP, DEC_MODE2, DEC_MODE1, DEC_MODE0);
MountConfig Config;

float RASpeed = 0;
float DECSpeed = 0;
float RASideRealRate = 0;
float DECSideRealRate = 0;

#define CONTROL 1
#define CONFIGURE 2
int MountMode = CONTROL;

char Response1[40];
char CommandBuffer1[BUFFERSIZE + 1];
int  Buffer1Used = 0;

#ifdef DUALPORT
char Response2[40];
char CommandBuffer2[BUFFERSIZE + 1];
int  Buffer2Used = 0;
#endif

void setup()
{
  Config.LoadMountConfig();
  Serial.begin(9600);

#ifdef DUALPORT
  Serial1.begin(9600);
#endif  

#ifdef IR
  IRRx.enableIRIn();
#endif

  pinMode(STATUSPIN, OUTPUT);

#if DEBUG >=1
  Serial2.begin(9600);
  Serial2.println("Debug Port");
#endif

  RA_Motor.setMaxSpeed(Config.GetRAGotoSpeed()); RA_Motor.setAcceleration(Config.GetRAGotoSpeed()/4);
  DEC_Motor.setMaxSpeed(Config.GetDECGotoSpeed()); DEC_Motor.setAcceleration(Config.GetDECGotoSpeed()/4);

  RASideRealRate = Config.GetRAStepsPerAxisRev() / (float)86164.09053  ;
  DECSideRealRate = Config.GetDECStepsPerAxisRev() / (float)86164.09053  ;
}

void loop()
{
  while (Serial.available())
  {
    if (MountMode == CONTROL)
      MountControlMode(CommandBuffer1, &Buffer1Used, Response1);
    else
      MountConfigurationMode(CommandBuffer1, &Buffer1Used, Response1);
  }

#ifdef DUALPORT
  while (Serial1.available())
  {
    if (MountMode == CONTROL)
      MountControlMode(CommandBuffer2, &Buffer2Used, Response2);
  }
#endif


#ifdef IR
  if (IRRx.decode(&IRResults))
  {
    //      Serial.println(IRResults.value, HEX);
    ProcessIRMessage(IRResults.value);
    IRRx.resume(); // Receive the next value
  }
  else
  {
    IRTimeOutCheck();
  }
#endif

  RA_Motor.Run();
  DEC_Motor.Run();
}

void MountControlMode(char *CommandBuffer, int *BufferUsed, char *Response)
{
  if (*BufferUsed < BUFFERSIZE)
  {
    Serial.readBytes(&CommandBuffer[*BufferUsed], 1);

    if (CommandBuffer[*BufferUsed] == ':')
    {
      *BufferUsed = 0;
    }
    else if (CommandBuffer[*BufferUsed] == 0x0D)
    {
      CommandBuffer[*BufferUsed] = '\0';
      ToggleStatusPin();

#if DEBUG >=3
      Serial2.print("Command ");
      Serial2.print(CommandBuffer);
#endif

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

        case 'Z' : MountMode = CONFIGURE; CommandError(Response, 0); CommandEND(Response, 1);
          break;

        default  : CommandOK(Response, 0); CommandEND(Response, 1);
          break;
      }
      
      Serial.print(Response);
#if DEBUG >=3
      Serial2.print(" Response ");
      Serial2.println(Response);
#endif

      *BufferUsed = 0;
    }
    else
    {
      (*BufferUsed)++;
    }
  }
  else
  {
    *BufferUsed = 0;
  }
}


void ReportVersion(char *Command, char *Response)
{
  strcpy(Response, "=010500");
  CommandEND(Response, 7);
}

void ReportHighSpeedRatio(char *Command, char *Response)
{
  strcpy(Response, "=10");
  CommandEND(Response, 3);
}

void ReportStepsPerWormRev(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config.GetRAStepsPerWormRev());
      CommandEND(Response, 7);
      break;

    case '2' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config.GetDECStepsPerWormRev());
      CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SetTrackingSpeed(char *Command, char *Response)
{
  long Divisor = 0;
  float Speed = 0;

#if DEBUG >=1
  Serial2.println("Set Tracking Speed ");
#endif

  switch (Command[1] )
  {
    case '1' : Divisor = HexToLong_Short(Command, 2);
      if (Divisor != 0)
      {
        Speed = (float)64932 / (float)Divisor;
        if (RA_Motor.getDirection() == 0)
        {
          Speed = -1 * Speed;
        }
        RA_Motor.setSpeed(Speed);
        RASpeed = Speed;
      }
#if DEBUG >=1
      Serial2.println("\t\tNew Tracking Speed "); Serial2.println((int)(Speed*100));
#endif

      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : Divisor = HexToLong_Short(Command, 2);
      if (Divisor != 0)
      {
        Speed = (float)64932 / (float)Divisor;
        if (DEC_Motor.getDirection() == 0)
        {
          Speed = -1 * Speed;
        }
        DEC_Motor.setSpeed(Speed);
        DECSpeed = Speed;
      }
#if DEBUG >=1
      Serial2.print("\t\tNew Tracking Speed "); Serial2.println((int)(Speed*100));
#endif

      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      return;
      break;
  }
}


void ReportStepsPerAxisRevolution(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config.GetRAStepsPerAxisRev());
      CommandEND(Response, 7);
      break;

    case '2' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, Config.GetDECStepsPerAxisRev());
      CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void ReportPosition(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, RA_Motor.currentPosition());
      CommandEND(Response, 7);
      break;

    case '2' : CommandOK(Response, 0);
      ULongToHex_Short(Response, 1, DEC_Motor.currentPosition());
      CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void ReportTimerInterruptFrequency(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : strcpy(Response, "=A7FD00"); CommandEND(Response, 7);
      break;

    case '2' : strcpy(Response, "=A7FD00"); CommandEND(Response, 7);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void StopAxis(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RA_Motor.stopAxis(FALSE);
#if DEBUG >=1
      Serial2.println("\t\tStop RA Axis");
#endif
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    case '2' : DEC_Motor.stopAxis(FALSE);
#if DEBUG >=1
      Serial2.println("\t\tStop DEC Axis");
#endif

      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void StopNow(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RA_Motor.stopAxis(TRUE);
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    case '2' : DEC_Motor.stopAxis(TRUE);
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void InitialiseAxis(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RA_Motor.enableOutputs();
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    case '2' : DEC_Motor.enableOutputs();
      CommandOK(Response, 0); CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void ReportAxisStatus(char *Command, char *Response)
{
  switch (Command[1])
  {
    case '1' : CommandOK(Response, 0);

      if (RA_Motor.isEnabled() == 1)
        Response[3] = '1';
      else
        Response[3] = '0';

      if (RA_Motor.getMotionMode() == 1)
        Response[2] = '1';
      else
        Response[2] = '0';

      if (RA_Motor.getDirection() == 1)
      {
        if (RA_Motor.isStopped())
          Response[1] = '1';
        else
          Response[1] = '0';
      }
      else
      {
        if (RA_Motor.isStopped())
          Response[1] = '3';
        else
          Response[1] = '2';
      }

#if DEBUG >=3
      Serial2.print("\t\t");
      Serial2.print(" Direction "); Serial2.print(RA_Motor.getDirection());
      Serial2.print(" Stopped "); Serial2.print(RA_Motor.isStopped());
      Serial2.print(" Motion Mode "); Serial2.print(RA_Motor.getMotionMode());
      Serial2.print(" Enabled "); Serial2.println(RA_Motor.isEnabled());
#endif
      CommandEND(Response, 4);
      break;

    case '2' : CommandOK(Response, 0);

      if (DEC_Motor.isEnabled() == 1)
        Response[3] = '1';
      else
        Response[3] = '0';

      if (DEC_Motor.getMotionMode() == 1)
        Response[2] = '1';
      else
        Response[2] = '0';

      if (DEC_Motor.getDirection() == 1)
      {
        if (DEC_Motor.isStopped())
          Response[1] = '1';
        else
          Response[1] = '0';
      }
      else
      {
        if (DEC_Motor.isStopped())
          Response[1] = '3';
        else
          Response[1] = '2';
      }

#if DEBUG >=3
      Serial2.print("\t\t");
      Serial2.print(" Direction "); Serial2.print(DEC_Motor.getDirection());
      Serial2.print(" Stopped "); Serial2.print(DEC_Motor.isStopped());
      Serial2.print(" Motion Mode "); Serial2.print(DEC_Motor.getMotionMode());
      Serial2.print(" Enabled "); Serial2.println(DEC_Motor.isEnabled());
#endif
      CommandEND(Response, 4);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void SetAxisPosition(char *Command, char *Response)
{
  long Position = 0;

  switch (Command[1] )
  {
    case '1' : Position = HexToLong_Short(Command, 2);
      RA_Motor.setCurrentPosition(Position);
#if DEBUG >=2
      Serial2.print("\t\tNew Position "); Serial2.println(Position, HEX);
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : Position = HexToLong_Short(Command, 2);
      DEC_Motor.setCurrentPosition(Position);
#if DEBUG >=2
      Serial2.print("\t\tNew Position "); Serial2.println(Position, HEX);
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}

void StartMotion(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : RA_Motor.startAxis();
#if DEBUG >=1
      Serial2.println("\t\tStart RA Axis");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : DEC_Motor.startAxis();
#if DEBUG >=1
      Serial2.print("\t\tStart DEC Axis");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void SetAxisMotionMode(char *Command, char *Response)
{
  switch (Command[1] )
  {
    case '1' : //
#if DEBUG >=1
      Serial2.print("\t\tRA Axis");
#endif

      if (Command[2] == '0')
      {
#if DEBUG >=1
        Serial2.print(" High Speed : GOTO");
#endif
        RA_Motor.setMotionMode(TelescopeAxis::GOTO);
        RA_Motor.setMaxSpeed(Config.GetRAGotoSpeed());
      }
      else if (Command[2] == '2')
      {
#if DEBUG >=1
        Serial2.print(" Low Speed : GOTO");
#endif
        RA_Motor.setMotionMode(TelescopeAxis::GOTO);
        RA_Motor.setMaxSpeed(Config.GetDECGotoSpeed());
      }
      else if (Command[2] == '1')
      {
#if DEBUG >=1
        Serial2.print(" Low Speed : SLEW");
#endif
        RA_Motor.setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          RA_Motor.setSpeed(Config.GetRASlewSpeed());
        else
          RA_Motor.setSpeed(-1 * Config.GetRASlewSpeed());
      }
      else if (Command[2] == '3')
      {
#if DEBUG >=1
        Serial2.print(" High Speed : SLEW");
#endif
        RA_Motor.setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          RA_Motor.setSpeed(Config.GetRASlewSpeed());
        else
          RA_Motor.setSpeed(-1 * Config.GetRASlewSpeed());
      }

      if (Command[3] == '0')
      {
        RA_Motor.setDirection(1);
#if DEBUG >=1
        Serial2.print(" Clockwise");
#endif
      }
      else
      {
        RA_Motor.setDirection(0);
#if DEBUG >=1
        Serial2.print(" Anti-clockwise");
#endif
      }
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : //
#if DEBUG >=1
      Serial2.print("\t\tDEC Axis");
#endif
      if (Command[2] == '0')
      {
        DEC_Motor.setMotionMode(TelescopeAxis::GOTO);
        DEC_Motor.setMaxSpeed(Config.GetDECGotoSpeed());
#if DEBUG >=1
        Serial2.print(" High Speed : GOTO");
#endif
      }
      else if (Command[2] == '2')
      {
        DEC_Motor.setMotionMode(TelescopeAxis::GOTO);
        DEC_Motor.setMaxSpeed(Config.GetDECSlewSpeed());
#if DEBUG >=1
        Serial2.print(" Low Speed : GOTO");
#endif
      }
      else if (Command[2] == '1')
      {
        DEC_Motor.setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          DEC_Motor.setSpeed(Config.GetDECSlewSpeed());
        else
          DEC_Motor.setSpeed(-1 * Config.GetDECSlewSpeed());
#if DEBUG >=1
        Serial2.print(" Low Speed : SLEW");
#endif
      }
      else if (Command[2] == '3')
      {
        DEC_Motor.setMotionMode(TelescopeAxis::SLEW);
        if (Command[3] == '0')
          DEC_Motor.setSpeed(Config.GetDECSlewSpeed());
        else
          DEC_Motor.setSpeed(-1 * Config.GetDECSlewSpeed());
#if DEBUG >=1
        Serial2.print(" High Speed : SLEW");
#endif
      }

      if (Command[3] == '0')
      {
        DEC_Motor.setDirection(1);
#if DEBUG >=1
        Serial2.print(" Clockwise");
#endif
      }
      else
      {
        DEC_Motor.setDirection(0);
#if DEBUG >=1
        Serial2.print(" Anti-Clockwise");
#endif
      }
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void Goto(char *Command, char *Response)
{
  long Position = 0;
  long Offset = 0;

  switch (Command[1] )
  {
    case '1' : Offset = HexToLong_Short(Command, 2);

      if (RA_Motor.getDirection() == 1)
        Position = RA_Motor.currentPosition() + Offset;
      else
        Position = RA_Motor.currentPosition() - Offset;

      RA_Motor.moveTo(Position);
#if DEBUG >=2
      Serial2.print("\t\tRA Goto "); Serial2.print(Position, HEX);
      Serial2.print(" ");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    case '2' : Offset = HexToLong_Short(Command, 2);

      if (DEC_Motor.getDirection() == 1)
        Position = DEC_Motor.currentPosition() + Offset;
      else
        Position = DEC_Motor.currentPosition() - Offset;

      DEC_Motor.moveTo(Position);
#if DEBUG >=2
      Serial2.print("\t\tDEC Goto "); Serial2.print(Position, HEX);
      Serial2.print(" ");
#endif
      CommandOK(Response, 0), CommandEND(Response, 1);
      break;

    default : CommandError(Response, 0);
      CommandEND(Response, 1);
      break;
  }
}


void CommandOK(char *Buffer, byte offset)
{
  *(Buffer + offset) = '=';
}

void CommandError(char *Buffer, byte offset)
{
  *(Buffer + offset) = '!';
}

void CommandEND(char *Buffer, byte offset)
{
  *(Buffer + offset) = COMMANDEND;
  *(Buffer + offset + 1) = '\0';
}




const char HexLookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void NibbleToHex(char *Buffer, byte offset, byte Nibble)
{
  Buffer[offset] = HexLookup[Nibble & 0x0f];
}

void ByteToHex(char *Buffer, byte offset, byte Byte)
{
  NibbleToHex(Buffer, offset + 1, Byte & 0x0f);
  NibbleToHex(Buffer, offset, (Byte >> 4) & 0x0f);
}

void UIntToHex(char *Buffer, byte offset, unsigned int UInt)
{
  ByteToHex(Buffer, offset, UInt & 0xff);
  ByteToHex(Buffer, offset + 2, (UInt >> 8) & 0xff);
}

void UIntToHex_Short(char *Buffer, byte offset, unsigned int UInt)
{
  NibbleToHex(Buffer, offset, UInt & 0x0f);
  ByteToHex(Buffer, offset + 1, (UInt >> 8) & 0xff);
}


void ULongToHex(char *Buffer, byte offset, unsigned long ULong)
{
  UIntToHex(Buffer, offset, ULong & 0xffff);
  UIntToHex(Buffer, offset + 4, (ULong >> 16) & 0xffff);
}

void ULongToHex_Short(char *Buffer, byte offset, unsigned long ULong)
{
  UIntToHex(Buffer, offset, ULong & 0xffff);
  ByteToHex(Buffer, offset + 4, (ULong >> 16) & 0xff);

}

long HexToLong(char *Buffer, byte offset)
{
  long result = 0;

  return result;
}

long HexToLong_Short(char *Buffer, byte offset)
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


int HexToNibble(char c)
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

void ToggleStatusPin(void)
{
  static int Status = HIGH;

  digitalWrite(STATUSPIN, Status);

  Status = !Status;
}



void MountConfigurationMode(char *CommandBuffer, int *BufferUsed, char *Response)
{
  if (*BufferUsed < BUFFERSIZE)
  {
    Serial.readBytes(&CommandBuffer[*BufferUsed], 1);
    Serial.print(CommandBuffer[*BufferUsed]);

    if (CommandBuffer[*BufferUsed] == ':')
    {
      *BufferUsed = 0;
    }
    else if (CommandBuffer[*BufferUsed] == 0x0D)
    {
      CommandBuffer[*BufferUsed] = '\0';
      ToggleStatusPin();

#if DEBUG >=1
      Serial2.print("Command ");
      Serial2.println(CommandBuffer);
#endif

      switch (CommandBuffer[0])
      {
        case 'a' : ReportConfiguration();
          break;

        case 'b' : SetMotorSteps(&CommandBuffer[2]);
          break;

        case 'd' : SetWormRatio(&CommandBuffer[2]);
          break;

        case 'e' : SetGOTOSpeed(&CommandBuffer[2]);
          break;

        case 'Z' : MountMode = CONTROL;
          break;

        case 'W' : Config.WriteMountConfig();
          break;

        default  : CommandOK(Response, 0); CommandEND(Response, 1);
          break;
      }
      *BufferUsed = 0;
    }
    else
    {
      (*BufferUsed)++;
    }
  }
  else
  {
    *BufferUsed = 0;
  }
}

void ReportConfiguration(void)
{
  Serial.println("\r\nParameter\tR.A.\tDeclination ");
  Serial.print  ("Motor Steps\t");
  Serial.print(Config.GetRAStepsPerMotorRev()); Serial.print("\t"); Serial.println(Config.GetDECStepsPerMotorRev());
  Serial.print("Worm Ratio\t");
  Serial.print(Config.GetRAWormRatio()); Serial.print("\t"); Serial.println(Config.GetDECWormRatio());
  Serial.print("GOTO Speed\t");
  Serial.print((float)Config.GetRAGotoSpeed(), 2); Serial.print("\t"); Serial.println((float)Config.GetDECGotoSpeed(), 2);
  Serial.print("Slew Speed\t");
  Serial.print(Config.GetRASlewSpeed()); Serial.print("\t"); Serial.println(Config.GetDECSlewSpeed());
}


void SetMotorSteps(char *CommandBuffer)
{
  long _Steps;
  _Steps = atol(&CommandBuffer[2]);

  if (CommandBuffer[0] == '1')
    Config.SetRAStepsPerMotorRev(_Steps);
  else if (CommandBuffer[0] == '2')
    Config.SetDECStepsPerMotorRev(_Steps);
}

void SetWormRatio(char *CommandBuffer)
{
  long _Ratio;
  _Ratio = atol(&CommandBuffer[2]);

  if (CommandBuffer[0] == '1')
    Config.SetRAWormRatio(_Ratio);
  else if (CommandBuffer[0] == '2')
    Config.SetDECWormRatio(_Ratio);
}

void SetGOTOSpeed(char *CommandBuffer)
{
  long _Speed;
  _Speed = atol(&CommandBuffer[2]);

  if (CommandBuffer[0] == '1')
    Config.SetRAGotoSpeed(_Speed);
  else if (CommandBuffer[0] == '2')
    Config.SetDECGotoSpeed(_Speed);
}



void ProcessIRMessage(unsigned long command)
{
#ifdef IR
  static int TrackSpeed = 1;

  switch (command)
  {
    case IR_OK:  if (IRTrack == false)
      {
        RA_Motor.setMotionMode(TelescopeAxis::SLEW);
        RA_Motor.setSpeed(RASideRealRate * TrackSpeed);
        RASpeed = RASideRealRate * TrackSpeed;
        RA_Motor.startAxis();
        IRTrack = true;
      }
      else
      {
        RA_Motor.stopAxis(FALSE);
        DEC_Motor.stopAxis(FALSE);
        RASpeed = 0;
        DECSpeed = 0;
        IRTrack = false;
      }
      Serial.println("IR OK");
      break;

    case IR_1: TrackSpeed = 1; break;
    case IR_2: TrackSpeed = 2; break;
    case IR_3: TrackSpeed = 4; break;
    case IR_4: TrackSpeed = 8; break;
    case IR_5: TrackSpeed = 16; break;
    case IR_6: TrackSpeed = 32; break;
    case IR_7: TrackSpeed = 64; break;
    case IR_8: TrackSpeed = 128; break;
    case IR_9: TrackSpeed = 256; break;

    case IR_LEFT: RA_Motor.setMotionMode(TelescopeAxis::SLEW);
      RA_Motor.setSpeed(RASideRealRate * TrackSpeed);
      RASpeed = RASideRealRate * TrackSpeed;
      RA_Motor.startAxis();
      IRSlew = true;
      IRTimeOut = millis() + IRTIMEOUT;
      Serial.println("Slew Left");
      break;

    case IR_RIGHT: RA_Motor.setMotionMode(TelescopeAxis::SLEW);
      RA_Motor.setSpeed(-1 * RASideRealRate * TrackSpeed);
      RASpeed = -1 * RASideRealRate * TrackSpeed;
      RA_Motor.startAxis();
      IRSlew = true;
      IRTimeOut = millis() + IRTIMEOUT;
      Serial.println("Slew Right");
      break;



    case IR_UP: DEC_Motor.setMotionMode(TelescopeAxis::SLEW);
      DEC_Motor.setSpeed(-1 * DECSideRealRate * TrackSpeed);
      DECSpeed = -1 * DECSideRealRate * TrackSpeed;
      DEC_Motor.startAxis();
      IRSlew = true;
      IRTimeOut = millis() + IRTIMEOUT;
      Serial.println("Slew Up");
      break;


    case IR_DOWN : DEC_Motor.setMotionMode(TelescopeAxis::SLEW);
      DEC_Motor.setSpeed(DECSideRealRate * TrackSpeed);
      DECSpeed = DECSideRealRate * TrackSpeed;
      DEC_Motor.startAxis();
      IRSlew = true;
      IRTimeOut = millis() + IRTIMEOUT;
      Serial.println("Slew Down");
      break;


    case IR_CONTINUE: IRTimeOut = millis() + IRTIMEOUT;
      Serial.println("Slew Continue");
      break;

  }
#endif
}

void IRTimeOutCheck()
{
#ifdef IR
  unsigned long Now;

  if (IRSlew == true)
  {
    Now = millis();

    if (Now > IRTimeOut)
    {
      Serial.println("Slew Time Out");
      RA_Motor.stopAxis(FALSE);
      DEC_Motor.stopAxis(FALSE);
      RASpeed = 0;
      DECSpeed = 0;
      IRSlew = false;
    }
  }
#endif
}


