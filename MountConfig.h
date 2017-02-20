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
#ifndef MountConfig_h
#define MountConfig_h

#include <Arduino.h>
#include <stdint.h>

// These defs cause trouble on some versions of Arduino
#undef round

struct MountConfig_Struct
{
  uint8_t   StoreVersion;

  uint32_t SerialBaud;
  
  int32_t RAWormRatio; 
  int32_t DECWormRatio; 
  
  int32_t RAStepsPerMotorRev; //25600;
  int32_t DECStepsPerMotorRev; //5333;
  
  float RAGotoSpeed;
  float RASlewSpeed;
  float DECGotoSpeed;
  float DECSlewSpeed;

  uint8_t RADirectionPin;
  uint8_t RADirectionInvert;

  uint8_t RAStepPin;
  uint8_t RAStepInvert;
  
  uint8_t RAEnablePin;
  uint8_t RAEnableInvert;
  uint8_t RAnResetPin;
  uint8_t RAnSleepPin;
  
  uint8_t DECDirectionPin;
  uint8_t DECDirectionInvert;

  uint8_t DECStepPin;
  uint8_t DECStepInvert;
  
  uint8_t DECEnablePin;
  uint8_t DECEnableInvert;
  
  uint8_t DECnResetPin;
  uint8_t DECnSleepPin;
    
};   

/////////////////////////////////////////////////////////////////////
class MountConfig 
{
  public:
    MountConfig(void);
    void LoadMountConfig(void);
    void WriteMountConfig(void);
    
    int32_t GetRAWormRatio(void); 
    void SetRAWormRatio(int32_t Ratio); 
    
    int32_t GetDECWormRatio(void);
    void SetDECWormRatio(int32_t Ratio);
    
    int32_t GetRAStepsPerMotorRev(void);
    void SetRAStepsPerMotorRev(int32_t Steps);

    int32_t GetDECStepsPerMotorRev(void); 
    void SetDECStepsPerMotorRev(int32_t Steps); 
    
    int32_t GetRAStepsPerWormRev(void);
    int32_t GetDECStepsPerWormRev(void);
    
    int32_t GetRAStepsPerAxisRev(void);
    int32_t GetDECStepsPerAxisRev(void);

    uint8_t GetRAStepPin(void);
    void    SetRAStepPin(uint8_t Pin);
    uint8_t GetRADirectionPin(void);
    void    SetRADirectionPin(uint8_t Pin);
    uint8_t GetRAEnablePin(void);
    void    SetRAEnablePin(uint8_t Pin);
    uint8_t GetRAnResetPin(void);
    void    SetRAnResetPin(uint8_t Pin);
    uint8_t GetRAnSleepPin(void);
    void    SetRAnSleepPin(uint8_t Pin);
    uint8_t GetRAStepInvert(void);
    void    SetRAStepInvert(uint8_t Invert);
    uint8_t GetRADirectionInvert(void);
    void    SetRADirectionInvert(uint8_t Invert);
    uint8_t GetRAEnableInvert(void);
    void    SetRAEnableInvert(uint8_t Invert);

    uint8_t GetDECStepPin(void);
    void    SetDECStepPin(uint8_t Pin);
    uint8_t GetDECDirectionPin(void);
    void    SetDECDirectionPin(uint8_t Pin);
    uint8_t GetDECEnablePin(void);
    void    SetDECEnablePin(uint8_t Pin);
    uint8_t GetDECnResetPin(void);
    void    SetDECnResetPin(uint8_t Pin);
    uint8_t GetDECnSleepPin(void);
    void    SetDECnSleepPin(uint8_t Pin);
    uint8_t GetDECStepInvert(void);
    void    SetDECStepInvert(uint8_t Invert);
    uint8_t GetDECDirectionInvert(void);
    void    SetDECDirectionInvert(uint8_t Invert);
    uint8_t GetDECEnableInvert(void);
    void    SetDECEnableInvert(uint8_t Invert);


    
    float GetRAGotoSpeed(void);
    float GetRASlewSpeed(void);
    float GetDECGotoSpeed(void);
    float GetDECSlewSpeed(void);
    void SetRAGotoSpeed(float Speed);
    void SetRASlewSpeed(float Speed);
    void SetDECGotoSpeed(float Speed);
    void SetDECSlewSpeed(float Speed);
      
  protected:
				
  private:
    struct MountConfig_Struct cs;
    uint16_t NONVolatile_Read(void *Object, uint16_t Size);
    uint16_t NONVolatile_Write(void *Object, uint16_t Size);
};


#endif 

