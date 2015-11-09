#ifndef CONFIGURATION_h
#define CONFIGURATION_h

#include <Arduino.h>

#if defined (_VARIANT_ARDUINO_DUE_X_)
#include <DueFlashStorage.h>
#include <efc.h>
#include <flash_efc.h>
#endif

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
    int NONVolatile_Read(void *Object, unsigned int Size);
    int NONVolatile_Write(void *Object, unsigned int Size);
};


#endif 

