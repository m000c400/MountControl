#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include "TelescopeAxis.h"
#include "PinAssignments.h"

long RAWormRatio=130;
long DECWormRatio=108; // 108 = 65 * 30/18
long RAMotorStepsPerRev=3200; //25600;
long DECMotorStepsPerRev=3200; //25600;//5333;
long RATotalSteps= 33280000;
long DECTotalSteps=208000;
long RACountScale = 16;
long DECCountScale = 16;


#define BUFFERSIZE 80
#define TRUE 1
#define FALSE 0

float RAFastGotoSpeed=3500;
float RASlowGotoSpeed=3500;
float RAFastSlewSpeed=10;
float RASlowSlewSpeed=10;
float DECFastGotoSpeed=3000;
float DECSlowGotoSpeed=3000;
float DECFastSlewSpeed=10;
float DECSlowSlewSpeed=10;

char COMMANDEND = 0x0D;
//#define DEBUG 3
#define LCD

#ifdef LCD
LiquidCrystal lcd(LCD_rs, LCD_enable, LCD_d4, LCD_d5, LCD_d6, LCD_d7);
#endif

TelescopeAxis RA_Motor( TelescopeAxis::DRIVER, RA_STEP, RA_DIRECTION);
TelescopeAxis DEC_Motor( TelescopeAxis::DRIVER, DEC_STEP, DEC_DIRECTION);

char CommandBuffer[BUFFERSIZE + 1];
int  BufferUsed = 0;
long RA_Requested;
long RA_Position;
long DEC_Requested;
long DEC_Position;

void setup() 
{
  Serial.begin(9800);

#ifdef LCD
  lcd.begin(16, 2);
#endif

  pinMode(STATUSPIN, OUTPUT);
  
  #if DEBUG >=1
    Serial1.begin(115200);
  #endif
  
  RA_Motor.setEnablePin( RA_ENABLE);
  DEC_Motor.setEnablePin(DEC_ENABLE);
  
  RA_Motor.setPinsInverted(false, false, false);
  DEC_Motor.setPinsInverted(false, false, true);

  RA_Motor.disableOutputs();
  DEC_Motor.disableOutputs();
  
  RA_Motor.setMaxSpeed(1000);RA_Motor.setAcceleration(4000);
  DEC_Motor.setMaxSpeed(1000);DEC_Motor.setAcceleration(4000);
  
  RA_Motor.setCurrentPosition(0x080000);
  DEC_Motor.setCurrentPosition(0x080000);
  
  RATotalSteps=   (long)RAWormRatio * (long)RAMotorStepsPerRev * (long)RACountScale;
  DECTotalSteps = (long)DECWormRatio* (long)DECMotorStepsPerRev * (long)DECCountScale;
}

void loop() 
{
  static char Response[40];
  
  while (Serial.available())
  {
    if(BufferUsed < BUFFERSIZE)
    {
      Serial.readBytes(&CommandBuffer[BufferUsed], 1);
      
      if(CommandBuffer[BufferUsed] == ':')  
      {
        BufferUsed = 0;
      }
      else if (CommandBuffer[BufferUsed] == 0x0D)
      {
        CommandBuffer[BufferUsed] = '\0';
        ToggleStatusPin();

        #if DEBUG >=1
        Serial1.print("Command ");
        Serial1.println(CommandBuffer);
        #endif
        
        switch (CommandBuffer[0])
        {
          case 'a' : ReportStepsPerRevolution(CommandBuffer,Response);
          break;
          
          case 'b' : ReportTimerInterruptFrequency(CommandBuffer,Response);
          break;
          
          case 'e' : ReportVersion(CommandBuffer,Response);
          break;
          
          case 'f' : ReportAxisStatus(CommandBuffer,Response);
          break;

          case 'g' : ReportHighSpeedRatio(CommandBuffer,Response);
          break;
          
          case 'j' : ReportPosition(CommandBuffer,Response);
          break;
           
          case 's' : ReportStepsPerWormRev(CommandBuffer,Response);
          break;
          
          case 'E' : SetAxisPosition(CommandBuffer,Response);
          break;
          
          case 'F' : InitialiseAxis(CommandBuffer,Response);
          break;
          
          case 'G' : SetAxisMotionMode(CommandBuffer,Response);
          break;
          
          case 'H' : Goto(CommandBuffer,Response);
          break;
          
          case 'I' : SetTrackingSpeed(CommandBuffer,Response);
          break;
          
          case 'J' : StartMotion(CommandBuffer,Response);
          break;
          
          case 'K' : StopAxis(CommandBuffer,Response);
          break;
          
          case 'L' : StopNow(CommandBuffer,Response);
          break;
          
          default  : CommandOK(Response,0); CommandEND(Response,1);
          break;          
        }
        Serial.print(Response);
        #if DEBUG >=1
        Serial1.print("Response ");
        Serial1.println(Response);
        #endif

        BufferUsed = 0; 
      }
      else
      {
         BufferUsed ++;
      }
    }
    else
    {
      BufferUsed = 0;
    }  
  }
/*
  Serial1.print("Ra Mode ");
  Serial1.print(RA_Motor.getMotionMode());
  Serial1.print(" Stop ");
  Serial1.print(RA_Motor.isStopped());
  Serial1.print(" Speed ");
  Serial1.print(RA_Motor.speed());
  Serial1.print(" Enabled ");
  Serial1.println(RA_Motor.isEnabled());
  
  Serial1.print("  DEC Mode");
  Serial1.print(DEC_Motor.getMotionMode());
  Serial1.print(" Stop ");
  Serial1.print(DEC_Motor.isStopped());
  Serial1.print(" Speed ");
  Serial1.print(DEC_Motor.speed());
  Serial1.print(" Enabled ");
  Serial1.println(DEC_Motor.isEnabled());
 */ 
  
  
  RA_Motor.Run();
  DEC_Motor.Run();
}

void ReportVersion(char *Command, char *Response)
{
  strcpy(Response,"=010500");
  CommandEND(Response,7);
}

void ReportHighSpeedRatio(char *Command, char *Response)
{
  strcpy(Response,"=10");
  CommandEND(Response,3);
}

void ReportStepsPerWormRev(char *Command, char *Response)
{
   switch (Command[1] )
   {
     case '1' : CommandOK(Response,0);
                ULongToHex_Short(Response, 1, RAMotorStepsPerRev * RACountScale);
                CommandEND(Response,7);
     break;

     case '2' : CommandOK(Response,0);
                ULongToHex_Short(Response, 1, DECMotorStepsPerRev * DECCountScale);
                CommandEND(Response,7);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}

void SetTrackingSpeed(char *Command, char *Response)
{
  long Divisor = 0;
  float Speed = 0;
  
  #if DEBUG >=1
    Serial1.println("Set Tracking Speed");
  #endif 
  
  switch (Command[1] )
  {
     case '1' : Divisor = RACountScale * HexToLong_Short(Command,2);
                if(Divisor != 0)
                {
                   Speed = (float)64932/(float)Divisor; 
                   if(RA_Motor.getDirection()== 0)
                   {
                      Speed = -1*Speed;
                   }
                   RAFastSlewSpeed = Speed;
                   RASlowSlewSpeed = Speed;
                   RA_Motor.setSpeed(Speed); 
                } 
                #if DEBUG >=2
                  Serial1.println("\t\tNew Tracking Speed "); Serial1.println(RAFastSlewSpeed,4);
                #endif
                
                #ifdef LCD
                  lcd.setCursor(0, 0);
                  lcd.print(Speed);
                #endif

                CommandOK(Response,0),CommandEND(Response,1);
     break;

     case '2' : Divisor = DECCountScale * HexToLong_Short(Command,2);
                if(Divisor != 0)
                {
                   Speed = (float)64932/(float)Divisor; 
                   if(DEC_Motor.getDirection() == 0)
                   {
                     Speed = -1*Speed;
                   }
                   DECFastSlewSpeed = Speed;
                   DECSlowSlewSpeed = Speed;
                   DEC_Motor.setSpeed(Speed);
                } 
                #if DEBUG >=2
                  Serial1.print("\t\tNew Tracking Speed "); Serial1.println(DECFastSlewSpeed,4);
                #endif
                #ifdef LCD
                  lcd.setCursor(8, 0);
                  lcd.print(Speed);
                #endif
                
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
  }
}


void ReportStepsPerRevolution(char *Command, char *Response)
{
   switch (Command[1] )
   {
     case '1' : CommandOK(Response,0);
                ULongToHex_Short(Response, 1, RATotalSteps);
                CommandEND(Response,7);
     break;

     case '2' : CommandOK(Response,0);
                ULongToHex_Short(Response, 1, DECTotalSteps);
                CommandEND(Response,7);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}

void ReportPosition(char *Command, char *Response)
{
   switch (Command[1] )
   {
     case '1' : CommandOK(Response,0);
                ULongToHex_Short(Response, 1, RA_Motor.currentPosition() * RACountScale);
//                #if DEBUG >=2
//                  Serial1.println(RA_Motor.currentPosition(),HEX);
//                #endif
                #ifdef LCD
                  lcd.setCursor(0, 1);
                  lcd.print(RA_Motor.currentPosition(),HEX);
                #endif
                CommandEND(Response,7);
     break;

     case '2' : CommandOK(Response,0);
                ULongToHex_Short(Response, 1, DEC_Motor.currentPosition() * DECCountScale);
//                #if DEBUG >=2
//                  Serial1.println(DEC_Motor.currentPosition(),HEX);
//                #endif
                #ifdef LCD
                  lcd.setCursor(8, 1);
                  lcd.print(DEC_Motor.currentPosition(),HEX);
                #endif
                CommandEND(Response,7);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}


void ReportTimerInterruptFrequency(char *Command, char *Response)
{
   switch (Command[1] )
   {
     case '1' : strcpy(Response,"=A7FD00");CommandEND(Response,7);
     break;
 
     case '2' : strcpy(Response,"=A7FD00");CommandEND(Response,7);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}

void StopAxis(char *Command, char *Response)
{
   switch (Command[1] )
   {
     case '1' : RA_Motor.stopAxis(FALSE);
                #if DEBUG >=1
                  Serial1.println("\t\tStop RA Axis"); 
                #endif
                CommandOK(Response,0);CommandEND(Response,1);
     break;

     case '2' : DEC_Motor.stopAxis(FALSE);
                #if DEBUG >=1
                  Serial1.println("\t\tStop DEC Axis"); 
                #endif

                CommandOK(Response,0);CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}

void StopNow(char *Command, char *Response)
{
   switch (Command[1] )
   {
     case '1' : RA_Motor.stopAxis(TRUE);
                CommandOK(Response,0);CommandEND(Response,1);
     break;

     case '2' : DEC_Motor.stopAxis(TRUE);
                CommandOK(Response,0);CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}

void InitialiseAxis(char *Command, char *Response)
{
   switch (Command[1] )
   {
     case '1' : RA_Motor.enableOutputs();
                CommandOK(Response,0);CommandEND(Response,1);
     break;

     case '2' : DEC_Motor.enableOutputs();
                CommandOK(Response,0);CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}

void ReportAxisStatus(char *Command, char *Response)
{
  switch (Command[1])
   {
     case '1' : CommandOK(Response,0);
                
               if(RA_Motor.isEnabled()==1)
                 Response[3] = '1';
               else
                 Response[3] = '0';
               
               if(RA_Motor.getMotionMode() == 1)
                 Response[2] = '1';
               else
                 Response[2] = '0';
                
               if(RA_Motor.getDirection() == 1)
               {
                 if(RA_Motor.isStopped())
                   Response[1] = '1';
                 else
                   Response[1] = '0';
               }
               else
               {
                 if(RA_Motor.isStopped())
                   Response[1] = '3';
                 else
                   Response[1] = '2';
               }
               
                #if DEBUG >=3
                  Serial1.print("\t\t");
                  Serial1.print(" Direction "); Serial1.print(RA_Motor.getDirection());
                  Serial1.print(" Stopped "); Serial1.print(RA_Motor.isStopped());
                  Serial1.print(" Motion Mode "); Serial1.print(RA_Motor.getMotionMode());
                  Serial1.print(" Enabled "); Serial1.println(RA_Motor.isEnabled());
                #endif
                CommandEND(Response,4);
     break;

     case '2' : CommandOK(Response,0);
              
               if(DEC_Motor.isEnabled()==1)
                 Response[3] = '1';
               else
                 Response[3] = '0';
               
               if(DEC_Motor.getMotionMode() == 1)
                 Response[2] = '1';
               else
                 Response[2] = '0';
                
               if(DEC_Motor.getDirection() == 1)
               {
                 if(DEC_Motor.isStopped())
                   Response[1] = '1';
                 else
                   Response[1] = '0';
               }
               else
               {
                 if(DEC_Motor.isStopped())
                   Response[1] = '3';
                 else
                   Response[1] = '2';
               }
                
                #if DEBUG >=3
                  Serial1.print("\t\t");
                  Serial1.print(" Direction "); Serial1.print(DEC_Motor.getDirection());
                  Serial1.print(" Stopped "); Serial1.print(DEC_Motor.isStopped());
                  Serial1.print(" Motion Mode "); Serial1.print(DEC_Motor.getMotionMode());
                  Serial1.print(" Enabled "); Serial1.println(DEC_Motor.isEnabled());
                #endif
                CommandEND(Response,4);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
   }
}

void SetAxisPosition(char *Command, char *Response)
{
  long Position = 0;
  
  switch (Command[1] )
  {
     case '1' : Position = HexToLong_Short(Command,2) / RACountScale;
                RA_Motor.setCurrentPosition(Position);
                #if DEBUG >=2
                  Serial1.print("\t\tNew Position "); Serial1.println(Position,HEX);
                #endif
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     case '2' : Position = HexToLong_Short(Command,2) / DECCountScale;
                DEC_Motor.setCurrentPosition(Position); 
                #if DEBUG >=2
                  Serial1.print("\t\tNew Position "); Serial1.println(Position,HEX);
                #endif
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
  }
}

void StartMotion(char *Command, char *Response)
{
  switch (Command[1] )
  {
     case '1' : RA_Motor.startAxis();
                #if DEBUG >=1
                  Serial1.println("\t\tStart RA Axis"); 
                #endif
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     case '2' : DEC_Motor.startAxis(); 
                #if DEBUG >=1
                  Serial1.print("\t\tStart DEC Axis");
                #endif
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
  }
}


void SetAxisMotionMode(char *Command, char *Response)
{
  switch (Command[1] )
  {
     case '1' : //
                #if DEBUG >=1
                  Serial1.print("\t\tRA Axis"); 
                #endif

                if(Command[2] == '0')
                {
                   #if DEBUG >=1
                     Serial1.print(" High Speed : GOTO");
                   #endif
                   RA_Motor.setMotionMode(TelescopeAxis::GOTO);
                   RA_Motor.setMaxSpeed(RAFastGotoSpeed);
                }
                else if(Command[2] == '2')
                {
                  #if DEBUG >=1
                     Serial1.print(" Low Speed : GOTO");
                   #endif
                   RA_Motor.setMotionMode(TelescopeAxis::GOTO);
                   RA_Motor.setMaxSpeed(RASlowGotoSpeed);
                }
                else if(Command[2] == '1')
                {
                   #if DEBUG >=1
                     Serial1.print(" Low Speed : SLEW");
                   #endif
                   RA_Motor.setMotionMode(TelescopeAxis::SLEW);
                   if(Command[3] == '0')
                     RA_Motor.setSpeed(RASlowSlewSpeed);
                   else
                     RA_Motor.setSpeed(-1*RASlowSlewSpeed);
                }
                else if(Command[2] == '3')
                {
                  #if DEBUG >=1
                     Serial1.print(" High Speed : SLEW");
                   #endif
                   RA_Motor.setMotionMode(TelescopeAxis::SLEW);
                   if(Command[3] == '0')
                     RA_Motor.setSpeed(RAFastSlewSpeed);
                   else
                     RA_Motor.setSpeed(-1*RAFastSlewSpeed);
                }
                
                if(Command[3] == '0')
                {
                  RA_Motor.setDirection(1);
                  #if DEBUG >=1
                     Serial1.print(" Clockwise");
                   #endif
                }
                else
                {
                  RA_Motor.setDirection(0);
                  #if DEBUG >=1
                     Serial1.print(" Anti-clockwise");
                   #endif
                }  
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     case '2' : //
                #if DEBUG >=1
                  Serial1.print("\t\tDEC Axis"); 
                #endif
                if(Command[2] == '0')
                {
                   DEC_Motor.setMotionMode(TelescopeAxis::GOTO);
                   DEC_Motor.setMaxSpeed(DECFastGotoSpeed);
                   #if DEBUG >=1
                     Serial1.print(" High Speed : GOTO");
                   #endif
                }
                else if(Command[2] == '2')
                {
                   DEC_Motor.setMotionMode(TelescopeAxis::GOTO);
                   DEC_Motor.setMaxSpeed(DECSlowGotoSpeed);
                   #if DEBUG >=1
                     Serial1.print(" Low Speed : GOTO");
                   #endif
                }
                else if(Command[2] == '1')
                {
                   DEC_Motor.setMotionMode(TelescopeAxis::SLEW);
                   if(Command[3] == '0')
                     DEC_Motor.setSpeed(DECSlowSlewSpeed);
                   else
                     DEC_Motor.setSpeed(-1*DECSlowSlewSpeed);
                   #if DEBUG >=1
                     Serial1.print(" Low Speed : SLEW");
                   #endif
                }
                else if(Command[2] == '3')
                {
                   DEC_Motor.setMotionMode(TelescopeAxis::SLEW);
                   if(Command[3] == '0')
                     DEC_Motor.setSpeed(DECFastSlewSpeed);
                   else
                     DEC_Motor.setSpeed(-1*DECFastSlewSpeed);
                   #if DEBUG >=1
                     Serial1.print(" High Speed : SLEW");
                   #endif                  
                }

                if(Command[3] == '0')
                {
                  DEC_Motor.setDirection(1);
                  #if DEBUG >=1
                    Serial1.print(" Clockwise");
                  #endif
                }
                else
                {
                  DEC_Motor.setDirection(0);
                  #if DEBUG >=1
                     Serial1.print(" Anti-Clockwise");
                  #endif
                }                  
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
     break;  
  }
}


void Goto(char *Command, char *Response)
{
  long Position = 0;
  long Offset = 0;
  
  switch (Command[1] )
  {
     case '1' : Offset = HexToLong_Short(Command,2) / RACountScale;
                
                if(RA_Motor.getDirection() == 1)
                  Position = RA_Motor.currentPosition() + Offset; 
                else
                  Position = RA_Motor.currentPosition() - Offset; 
                  
                RA_Motor.moveTo(Position); 
                #if DEBUG >=2
                  Serial1.print("\t\tRA Goto ");Serial1.print(Position, HEX);
                  Serial1.print(" ");
                #endif
                #ifdef LCD
                  lcd.setCursor(0, 0);
                  lcd.print(Position,HEX);
                #endif
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     case '2' : Offset = HexToLong_Short(Command,2) / DECCountScale;
                
                if(DEC_Motor.getDirection() == 1)
                  Position = DEC_Motor.currentPosition() + Offset; 
                else
                  Position = DEC_Motor.currentPosition() - Offset; 

                DEC_Motor.moveTo(Position); 
                #if DEBUG >=2
                  Serial1.print("\t\tDEC Goto ");Serial1.print(Position, HEX);
                  Serial1.print(" ");
                #endif
                #ifdef LCD
                  lcd.setCursor(8, 0);
                  lcd.print(Position, HEX);
                #endif
                CommandOK(Response,0),CommandEND(Response,1);
     break;

     default : CommandError(Response,0);
               CommandEND(Response,1);
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




const char HexLookup[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void NibbleToHex(char *Buffer, byte offset, byte Nibble) 
{
  Buffer[offset] = HexLookup[Nibble & 0x0f];
}

void ByteToHex(char *Buffer, byte offset, byte Byte)
{
  NibbleToHex(Buffer, offset+1, Byte & 0x0f);
  NibbleToHex(Buffer, offset, (Byte >> 4) & 0x0f);
}

void UIntToHex(char *Buffer, byte offset, unsigned int UInt)
{
  ByteToHex(Buffer, offset, UInt & 0xff);
  ByteToHex(Buffer, offset+2, (UInt>>8) & 0xff);
}

void UIntToHex_Short(char *Buffer, byte offset, unsigned int UInt)
{
  NibbleToHex(Buffer, offset, UInt & 0x0f);
  ByteToHex(Buffer, offset+1, (UInt>>8) & 0xff);
}


void ULongToHex(char *Buffer, byte offset, unsigned long ULong)
{
  UIntToHex(Buffer, offset, ULong & 0xffff);
  UIntToHex(Buffer, offset+4, (ULong>>16) & 0xffff);
}

void ULongToHex_Short(char *Buffer, byte offset, unsigned long ULong)
{
  UIntToHex(Buffer, offset, ULong & 0xffff);
  ByteToHex(Buffer, offset+4, (ULong>>16) & 0xff);

}

long HexToLong(char *Buffer, byte offset)
{
  long result = 0;
  
 return result;
}

long HexToLong_Short(char *Buffer, byte offset)
{
  long temp = 0;
  
  temp = HexToNibble(Buffer[offset+4]); temp = temp << 4; 
  temp += HexToNibble(Buffer[offset+5]);  temp = temp << 4; 
  temp += HexToNibble(Buffer[offset+2]); temp = temp << 4; 
  temp += HexToNibble(Buffer[offset+3]);  temp = temp << 4; 
  temp += HexToNibble(Buffer[offset+0]); temp = temp << 4; 
  temp += HexToNibble(Buffer[offset+1]);

  return temp;
}


int HexToNibble(char c)
{
  switch(c)
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
  

