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
			FUNCTION  = 0, ///< Use the functional interface, implementing your own driver functions (internal use only)
			DRIVER    = 1, ///< Stepper Driver, 2 driver pins required
			FULL2WIRE = 2, ///< 2 wire stepper, 2 motor pins required
			FULL3WIRE = 3, ///< 3 wire stepper, such as HDD spindle, 3 motor pins required
			FULL4WIRE = 4, ///< 4 wire full stepper, 4 motor pins required
			HALF3WIRE = 6, ///< 3 wire half stepper, such as HDD spindle, 3 motor pins required
			HALF4WIRE = 8  ///< 4 wire half stepper, 4 motor pins required
		} MotorInterfaceType;
		
		typedef enum
		{
			SLEW  = 0, 
			GOTO  = 1
		} AxisMotionMode;
	
		TelescopeAxis(uint8_t interface = TelescopeAxis::FULL4WIRE , uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);

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
		
		void enableOutputs(void);
		void disableOutputs(void);


	protected:
				
	private:
		AxisMotionMode _motionmode;
		int Direction;
		int Stopped;
		int RunMotor;
		int Enabled;
};


#endif 

