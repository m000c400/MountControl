#include "MountConfig.h"

#include <EEPROM.h>

/*
  DueFlashStorage dueFlashStorage;
*/

MountConfig::MountConfig(void)
{
}

void MountConfig::LoadMountConfig(void)
{
  NONVolatile_Read((void *)&cs, sizeof(cs));

  if (cs.StoreVersion != 1)
  {
    cs.StoreVersion = 1;

    cs.RAWormRatio = 144*2; // 130
    cs.DECWormRatio = 144*2; // 108 = 65 * 30/18

    cs.RAStepsPerMotorRev = 6400;
    cs.DECStepsPerMotorRev = 6400;

    cs.RAGotoSpeed = 20000;
    cs.RASlewSpeed = 20000;
    cs.DECGotoSpeed = 20000;
    cs.DECSlewSpeed = 20000;
  }
}

long MountConfig::GetRAWormRatio(void)
{
  return cs.RAWormRatio;
}

void MountConfig::SetRAWormRatio(long Ratio)
{
  cs.RAWormRatio = Ratio;
}

long MountConfig::GetDECWormRatio(void)
{
  return cs.DECWormRatio;
}

void MountConfig::SetDECWormRatio(long Ratio)
{
  cs.DECWormRatio = Ratio;
}

long MountConfig::GetRAStepsPerMotorRev(void)
{
  return cs.RAStepsPerMotorRev;
}

long MountConfig::GetDECStepsPerMotorRev(void)
{
  return cs.DECStepsPerMotorRev;
}

long MountConfig::GetRAStepsPerWormRev(void)
{
  return (long) cs.RAStepsPerMotorRev;
}

long MountConfig::GetDECStepsPerWormRev(void)
{
  return (long) cs.DECStepsPerMotorRev;
}

long MountConfig::GetRAStepsPerAxisRev(void)
{
  return (long) cs.RAStepsPerMotorRev * (long) cs.RAWormRatio;
}

long MountConfig::GetDECStepsPerAxisRev(void)
{
  return (long) cs.DECStepsPerMotorRev * (long) cs.DECWormRatio;
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

void MountConfig::SetRAStepsPerMotorRev(long Steps)
{
  cs.RAStepsPerMotorRev = Steps;
}

void MountConfig::SetDECStepsPerMotorRev(long Steps)
{
  cs.DECStepsPerMotorRev = Steps;
}

void MountConfig::WriteMountConfig(void)
{
  NONVolatile_Write((void *)&cs, sizeof(cs));
}


int MountConfig::NONVolatile_Write(void *Object, unsigned int Size)
{
  const byte* p = (byte*)Object;
  unsigned int i;
  for (i = 0; i < Size; i++)
    EEPROM.write(i, *p++);
  return i;

  /*  byte *p;
    unsigned int i;

    p = (byte *)Object;
    dueFlashStorage.write(0, p, Size);

    return i; */
}

int MountConfig::NONVolatile_Read(void *Object, unsigned int Size)
{
  byte* p = (byte*)(void*)Object;
  unsigned int i;
  for (i = 0; i < Size; i++)
    *p++ = EEPROM.read(i);
  return i;

  /*

  byte *p = (byte*)(void*)Object;
  unsigned int i;

  byte* b = dueFlashStorage.readAddress(0); // byte array which is read from flash at adress 4
  memcpy(p, b, Size); // copy byte array to temporary struct#endif
  return i;
  */
}

