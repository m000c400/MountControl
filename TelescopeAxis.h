#ifndef TELESCOPEAXIS_h
#define TELESCOPEAXIS_h

#include <AccelStepper.h>

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

// These defs cause trouble on some versions of Arduino
#undef round


/////////////////////////////////////////////////////////////////////
class TelescopeAxis : public AccelStepper
{
	public:
		typedef enum
		{
			SLEW  = 0, 
			GOTO  = 1
		} AxisMotionMode;
	
		TelescopeAxis(uint8_t STEP, uint8_t DIRECTION, uint8_t nENABLE, uint8_t nRESET, uint8_t nSLEEP, uint8_t MODE2, uint8_t MODE1, uint8_t MODE0);

		int getDirection(void);
		int isStopped(void);
		int isEnabled(void);
		int getMotionMode(void);
		void setMotionMode(AxisMotionMode Mode);
		void Run(void);
		void setDirection(int);
		void startAxis(void);
		void stopAxis(int Disable);
		void moveTo(long absolute);

    long currentPosition(void);
    void setCurrentPosition(long Position);
    
		void enableOutputs(void);
		void disableOutputs(void);


	protected:
				
	private:
    AxisMotionMode _motionmode;
		int Direction;
		int Stopped;
		int RunMotor;
		int Enabled;
    long TargetPosition;
    uint8_t Mode0_Pin;
    uint8_t Mode1_Pin;
    uint8_t Mode2_Pin;
};


#endif 

