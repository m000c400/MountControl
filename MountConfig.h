#ifndef MountConfig_h
#define MountConfig_h

#include <Arduino.h>

//#if defined (_VARIANT_ARDUINO_DUE_X_)
//#include <DueFlashStorage.h>
//#include <efc.h>
//#include <flash_efc.h>
//#endif

// These defs cause trouble on some versions of Arduino
#undef round

struct MountConfig_Struct
{
  int  StoreVersion;
  
  long RAWormRatio; 
  long DECWormRatio; 
  
  long RAStepsPerMotorRev; //25600;
  long DECStepsPerMotorRev; //5333;
  
  float RAGotoSpeed;
  float RASlewSpeed;
  float DECGotoSpeed;
  float DECSlewSpeed;
};   

/////////////////////////////////////////////////////////////////////
class MountConfig 
{
  public:
    MountConfig(void);
    void LoadMountConfig(void);
    void WriteMountConfig(void);
    
    long GetRAWormRatio(void); 
    void SetRAWormRatio(long Ratio); 
    
    long GetDECWormRatio(void);
    void SetDECWormRatio(long Ratio);
    
    long GetRAStepsPerMotorRev(void);
    void SetRAStepsPerMotorRev(long Steps);

    long GetDECStepsPerMotorRev(void); 
    void SetDECStepsPerMotorRev(long Steps); 
    
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
    struct MountConfig_Struct cs;
    int NONVolatile_Read(void *Object, unsigned int Size);
    int NONVolatile_Write(void *Object, unsigned int Size);
};


#endif 

