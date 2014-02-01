#include "Configuration.h"

Configuration::Configuration(void)
{
}

void Configuration::LoadConfiguration(void)
{
  /*
  cs.RAWormRatio = 130; // 130
  cs.DECWormRatio = 108; // 108 = 65 * 30/18
  
  cs.RAStepsPerMotorRev = 12800;
  cs.DECStepsPerMotorRev = 3200; 
  
  cs.RAMotorCountScale = 2;
  cs.DECMotorCountScale = 16;
    
  cs.RAGotoSpeed = 3000;
  cs.RASlewSpeed = 3000;
  cs.DECGotoSpeed = 3000;
  cs.DECSlewSpeed = 3000;
*/
  EEPROM_read((void *)&cs,sizeof(cs));  
}

long Configuration::GetRAWormRatio(void)
{
  return cs.RAWormRatio;
}

void Configuration::SetRAWormRatio(long Ratio)
{
  cs.RAWormRatio = Ratio;
}

long Configuration::GetDECWormRatio(void)
{
  return cs.DECWormRatio;
}

void Configuration::SetDECWormRatio(long Ratio)
{
  cs.DECWormRatio = Ratio;
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

void Configuration::SetRAGotoSpeed(float Speed)
{
  cs.RAGotoSpeed = Speed;
}

void Configuration::SetRASlewSpeed(float Speed)
{
  cs.RASlewSpeed = Speed;
}

void Configuration::SetDECGotoSpeed(float Speed)
{
  cs.DECGotoSpeed = Speed;
}

void Configuration::SetDECSlewSpeed(float Speed)
{
  cs.DECSlewSpeed = Speed;
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

void Configuration::WriteConfiguration(void)
{
  EEPROM_write((void *)&cs,sizeof(cs));
}


int Configuration::EEPROM_write(void *Object, int Size)
{
    const byte* p = (byte*)Object;
    unsigned int i;
    for (i = 0; i < Size; i++)
          EEPROM.write(i, *p++);
    return i;
}

int Configuration::EEPROM_read(void *Object, int Size)
{
    byte* p = (byte*)(void*)Object;
    unsigned int i;
    for (i = 0; i < Size; i++)
          *p++ = EEPROM.read(i);
    return i;
}

