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
	
		TelescopeAxis(uint8_t STEP, uint8_t DIRECTION, uint8_t ENABLE, uint8_t SLEEP, uint8_t RESET, uint8_t nSTEP, uint8_t nDIRECTION, uint8_t nENABLE);

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
};


#endif 

