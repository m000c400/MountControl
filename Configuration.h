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
  long RAWormRatio; // 130
  long DECWormRatio; // 108 = 65 * 30/18
  
  long RAStepsPerMotorRev; //25600;
  long DECStepsPerMotorRev; //5333;
  
  long RAMotorCountScale;
  long DECMotorCountScale;
    
  long RAStepsPerWormRev; //25600;
  long DECStepsPerWormRev; //5333;
  
  long RATotalStepsPerRev;
  long DECTotalStepPerRev;

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
    void SaveConfiguration(void);
    
    long GetRAWormRatio(void); 
    long GetDECWormRatio(void);
    
    long GetRAStepsPerMotorRev(void);
    long GetDECStepsPerMotorRev(void); 
    
    long GetRAMotorCountScale(void);
    long GetDECMotorCountScale(void);
  
    long GetRAStepsPerWormRev(void);
    long GetDECStepsPerWormRev(void);
    
    long GetRAStepsPerAxisRev(void);
    long GetDECStepsPerAxisRev(void);


    void SetRAWormRatio(long Ratio); 
    void SetDECWormRatio(long Ratio);
    
    void SetRAStepsPerMotorRev(long Steps);
    void SetDECStepsPerMotorRev(long Steps); 
    
    void SetRAMotorCountScale(long Scale);
    void SetDECMotorCountScale(long Scale);
  
 
    
    float GetRAGotoSpeed(void);
    float GetRASlewSpeed(void);
    float GetDECGotoSpeed(void);
    float GetDECSlewSpeed(void);
    
    
      
  protected:
				
  private:
    struct Configuration_Struct cs;
    //int EEPROM_write(int ee, const T& value);
    //int EEPROM_read(int ee, const T& value);
};


#endif 

