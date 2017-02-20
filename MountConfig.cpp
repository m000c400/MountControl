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
    aint32_t with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MountConfig.h"
#include "PinAssignments.h"

#include <EEPROM.h>


MountConfig::MountConfig(void)
{
}

void MountConfig::LoadMountConfig(void)
{
  NONVolatile_Read((void *)&cs, sizeof(cs));

  if (cs.StoreVersion != 1)
  {
    cs.StoreVersion = 1;

    //cs.RAWormRatio = 144; // 130
    //cs.DECWormRatio = 144; // 108 = 65 * 30/18

    //cs.RAStepsPerMotorRev = 32 * 200 * 2 ;
    //cs.DECStepsPerMotorRev = 32 * 200 * 2;


    cs.RAWormRatio = 130; // 130
    cs.DECWormRatio = 108; // 108 = 65 * 30/18

    cs.RAStepsPerMotorRev = 200 * 16 ;
    cs.DECStepsPerMotorRev = 200 * 16 ;


    cs.RAGotoSpeed = 4000;
    cs.RASlewSpeed = 4000;
    cs.DECGotoSpeed = 4000;
    cs.DECSlewSpeed = 4000;

    cs.SerialBaud = 9600;

    cs.RADirectionPin = RA_DIRECTION;
    cs.RADirectionInvert = 0;
    cs.RAStepPin = RA_STEP;
    cs.RAStepInvert = 0;
    cs.RAEnablePin = RA_ENABLE;
    cs.RAEnableInvert = 0;
    cs.RAnResetPin = RA_nRESET;
    cs.RAnSleepPin = RA_nSLEEP;
  
    cs.DECDirectionPin = DEC_DIRECTION;
    cs.DECDirectionInvert = 0;
    cs.DECStepPin = DEC_STEP;
    cs.DECStepInvert = 0;
    cs.DECEnablePin = DEC_ENABLE;
    cs.DECEnableInvert = 0;
    cs.DECnResetPin = DEC_nRESET;
    cs.DECnSleepPin = DEC_nSLEEP;    
  }
}

int32_t MountConfig::GetRAWormRatio(void)
{
  return cs.RAWormRatio;
}

void MountConfig::SetRAWormRatio(int32_t Ratio)
{
  cs.RAWormRatio = Ratio;
}

int32_t MountConfig::GetDECWormRatio(void)
{
  return cs.DECWormRatio;
}

void MountConfig::SetDECWormRatio(int32_t Ratio)
{
  cs.DECWormRatio = Ratio;
}

int32_t MountConfig::GetRAStepsPerMotorRev(void)
{
  return cs.RAStepsPerMotorRev;
}

int32_t MountConfig::GetDECStepsPerMotorRev(void)
{
  return cs.DECStepsPerMotorRev;
}

int32_t MountConfig::GetRAStepsPerWormRev(void)
{
  return (int32_t) cs.RAStepsPerMotorRev;
}

int32_t MountConfig::GetDECStepsPerWormRev(void)
{
  return (int32_t) cs.DECStepsPerMotorRev;
}

int32_t MountConfig::GetRAStepsPerAxisRev(void)
{
  return (int32_t) cs.RAStepsPerMotorRev * (int32_t) cs.RAWormRatio;
}

int32_t MountConfig::GetDECStepsPerAxisRev(void)
{
  return (int32_t) cs.DECStepsPerMotorRev * (int32_t) cs.DECWormRatio;
}

float MountConfig::GetRAGotoSpeed(void)
{
  return cs.RAGotoSpeed;
}

float MountConfig::GetRASlewSpeed(void)
{
  return cs.RASlewSpeed;
}

float MountConfig::GetDECGotoSpeed(void)
{
  return cs.DECGotoSpeed;
}

float MountConfig::GetDECSlewSpeed(void)
{
  return cs.DECSlewSpeed;
}

void MountConfig::SetRAGotoSpeed(float Speed)
{
  cs.RAGotoSpeed = Speed;
}

void MountConfig::SetRASlewSpeed(float Speed)
{
  cs.RASlewSpeed = Speed;
}

void MountConfig::SetDECGotoSpeed(float Speed)
{
  cs.DECGotoSpeed = Speed;
}

void MountConfig::SetDECSlewSpeed(float Speed)
{
  cs.DECSlewSpeed = Speed;
}

void MountConfig::SetRAStepsPerMotorRev(int32_t Steps)
{
  cs.RAStepsPerMotorRev = Steps;
}

void MountConfig::SetDECStepsPerMotorRev(int32_t Steps)
{
  cs.DECStepsPerMotorRev = Steps;
}

void MountConfig::WriteMountConfig(void)
{
  NONVolatile_Write((void *)&cs, sizeof(cs));
}


/********************************************/

uint8_t MountConfig::GetRAStepPin(void)
{
  return cs.RAStepPin;
}

void MountConfig::SetRAStepPin(uint8_t Pin)
{
  cs.RAStepPin = Pin;
}

uint8_t MountConfig::GetRADirectionPin(void)
{
  return cs.RADirectionPin;
}

void MountConfig::SetRADirectionPin(uint8_t Pin)
{
  cs.RADirectionPin = Pin;
}

uint8_t MountConfig::GetRAEnablePin(void)
{
  return cs.RAEnablePin;
}

void MountConfig::SetRAEnablePin(uint8_t Pin)
{
  cs.RAEnablePin = Pin;
}

uint8_t MountConfig::GetRAnSleepPin(void)
{
  return cs.RAnSleepPin;
}

void MountConfig::SetRAnSleepPin(uint8_t Pin)
{
  cs.RAnSleepPin = Pin;
}

uint8_t MountConfig::GetRAnResetPin(void)
{
  return cs.RAnResetPin;
}

void MountConfig::SetRAnResetPin(uint8_t Pin)
{
  cs.RAnResetPin = Pin;
}

uint8_t MountConfig::GetRAStepInvert(void)
{
  return cs.RAStepInvert;
}

void MountConfig::SetRAStepInvert(uint8_t Invert)
{
  cs.RAStepInvert = Invert;
}

uint8_t MountConfig::GetRADirectionInvert(void)
{
  return cs.RADirectionInvert;
}

void MountConfig::SetRADirectionInvert(uint8_t Invert)
{
  cs.RADirectionInvert = Invert;
}

uint8_t MountConfig::GetRAEnableInvert(void)
{
  return cs.RAEnableInvert;
}

void MountConfig::SetRAEnableInvert(uint8_t Invert)
{
  cs.RAEnableInvert = Invert;
}

/*********************************************/

uint8_t MountConfig::GetDECStepPin(void)
{
  return cs.DECStepPin;
}

void MountConfig::SetDECStepPin(uint8_t Pin)
{
  cs.DECStepPin = Pin;
}

uint8_t MountConfig::GetDECDirectionPin(void)
{
  return cs.DECDirectionPin;
}

void MountConfig::SetDECDirectionPin(uint8_t Pin)
{
  cs.DECDirectionPin = Pin;
}

uint8_t MountConfig::GetDECEnablePin(void)
{
  return cs.DECEnablePin;
}

void MountConfig::SetDECEnablePin(uint8_t Pin)
{
  cs.DECEnablePin = Pin;
}

uint8_t MountConfig::GetDECnSleepPin(void)
{
  return cs.DECnSleepPin;
}

void MountConfig::SetDECnSleepPin(uint8_t Pin)
{
  cs.DECnSleepPin = Pin;
}

uint8_t MountConfig::GetDECnResetPin(void)
{
  return cs.DECnResetPin;
}

void MountConfig::SetDECnResetPin(uint8_t Pin)
{
  cs.DECnResetPin = Pin;
}

uint8_t MountConfig::GetDECStepInvert(void)
{
  return cs.DECStepInvert;
}

void MountConfig::SetDECStepInvert(uint8_t Invert)
{
  cs.DECStepInvert = Invert;
}

uint8_t MountConfig::GetDECDirectionInvert(void)
{
  return cs.DECDirectionInvert;
}

void MountConfig::SetDECDirectionInvert(uint8_t Invert)
{
  cs.DECDirectionInvert = Invert;
}

uint8_t MountConfig::GetDECEnableInvert(void)
{
  return cs.DECEnableInvert;
}

void MountConfig::SetDECEnableInvert(uint8_t Invert)
{
  cs.DECEnableInvert = Invert;
}

/*********************************************/









uint16_t MountConfig::NONVolatile_Write(void *Object, uint16_t Size)
{
  const byte* p = (byte*)Object;
  unsigned int i;
  for (i = 0; i < Size; i++)
    EEPROM.write(i, *p++);
  return i;
}

uint16_t MountConfig::NONVolatile_Read(void *Object, uint16_t Size)
{
  byte* p = (byte*)(void*)Object;
  unsigned int i;
  for (i = 0; i < Size; i++)
    *p++ = EEPROM.read(i);
  return i;

}


