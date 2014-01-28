#include "Configuration.h"

Configuration::Configuration(void)
{
}

void Configuration::LoadConfiguration(void)
{
  
  //RATotalSteps =  (long)MountConfiguration->GetRAWormRatio() * (long)MountConfiguration->GetRAMotorStepsPerRev() * (long)MountConfiguration->GetRACountScale();
  //DECTotalSteps = (long)MountConfiguration->GetDECWormRatio() * (long)MountConfiguration->GetDECMotorStepsPerRev() * (long)MountConfiguration->GetDECCountScale();

  
}

long Configuration::GetRAWormRatio(void)
{
  return cs.RAWormRatio;
}

long Configuration::GetDECWormRatio(void)
{
  return cs.DECWormRatio;
}

long Configuration::GetRAStepsPerMotorRev(void)
{
  return cs.RAStepsPerMotorRev;
}

long Configuration::GetDECStepsPerMotorRev(void)
{
  return cs.DECStepsPerMotorRev;
}

long Configuration::GetRAStepsPerWormRev(void)
{
   return (long) cs.RAStepsPerMotorRev * (long) cs.RAMotorCountScale;
}

long Configuration::GetDECStepsPerWormRev(void)
{
  return (long) cs.DECStepsPerMotorRev * (long) cs.DECMotorCountScale;
}

long Configuration::GetRAStepsPerAxisRev(void)
{
   return (long) cs.RAStepsPerMotorRev * (long) cs.RAMotorCountScale * (long) cs.RAWormRatio;
}

long Configuration::GetDECStepsPerAxisRev(void)
{
  return (long) cs.DECStepsPerMotorRev * (long) cs.DECMotorCountScale * (long) cs.DECWormRatio;
}

long Configuration::GetRAMotorCountScale(void)
{
   return (long) cs.RAMotorCountScale;
}

long Configuration::GetDECMotorCountScale(void)
{
  return (long) cs.DECMotorCountScale;
}

float Configuration::GetRAGotoSpeed(void)
{
  return cs.RAGotoSpeed;
}

float Configuration::GetRASlewSpeed(void)
{
  return cs.RASlewSpeed;
}

float Configuration::GetDECGotoSpeed(void)
{
  return cs.DECGotoSpeed;
}

float Configuration::GetDECSlewSpeed(void)
{
  return cs.DECSlewSpeed;
}
 
void Configuration::SetRAWormRatio(long Ratio)
{
  cs.RAWormRatio = Ratio;
}

void Configuration::SetDECWormRatio(long Ratio)
{
  cs.DECWormRatio = Ratio;
}
    
void Configuration::SetRAStepsPerMotorRev(long Steps)
{
  cs.RAStepsPerMotorRev = Steps;
}

void Configuration::SetDECStepsPerMotorRev(long Steps)
{
  cs.DECStepsPerMotorRev = Steps;
}
    
void Configuration::SetRAMotorCountScale(long Scale)
{
  cs.RAMotorCountScale = Scale;
}

void Configuration::SetDECMotorCountScale(long Scale)
{
  cs.DECMotorCountScale = Scale;
}

/*
int Configuration::EEPROM_write(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

int Configuration::EEPROM_read(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}
*/
