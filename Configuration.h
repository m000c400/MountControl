#ifndef CONFIGURATION_h
#define CONFIGURATION_h

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#include <EEPROM.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

#include <EEPROM.h>


// These defs cause trouble on some versions of Arduino
#undef round

struct Configuration_Struct
{
  long RAWormRatio; 
  long DECWormRatio; 
  
  long RAStepsPerMotorRev; //25600;
  long DECStepsPerMotorRev; //5333;
  
  long RAMotorCountScale;
  long DECMotorCountScale;
    
  float RAGotoSpeed;
  float RASlewSpeed;
  float DECGotoSpeed;
  float DECSlewSpeed;
};   

/////////////////////////////////////////////////////////////////////
class Configuration 
{
  public:
    Configuration();
    void LoadConfiguration(void);
    void WriteConfiguration(void);
    
    long GetRAWormRatio(void); 
    void SetRAWormRatio(long Ratio); 
    
    long GetDECWormRatio(void);
    void SetDECWormRatio(long Ratio);
    
    long GetRAStepsPerMotorRev(void);
    void SetRAStepsPerMotorRev(long Steps);

    long GetDECStepsPerMotorRev(void); 
    void SetDECStepsPerMotorRev(long Steps); 
    
    long GetRAMotorCountScale(void);
    void SetRAMotorCountScale(long Scale);

    long GetDECMotorCountScale(void);
    void SetDECMotorCountScale(long Scale);
  
    long GetRAStepsPerWormRev(void);
    long GetDECStepsPerWormRev(void);
    
    long GetRAStepsPerAxisRev(void);
    long GetDECStepsPerAxisRev(void);

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
    struct Configuration_Struct cs;
    int EEPROM_write(void *Object, int Size);
    int EEPROM_read(void *Object, int Size);
};


#endif 

