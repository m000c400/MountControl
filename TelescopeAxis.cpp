#include "TelescopeAxis.h"

TelescopeAxis::TelescopeAxis(uint8_t interface, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, bool enable): AccelStepper(interface,  pin1, pin2, pin3, pin4, enable)
{
	Direction = 1;
	Stopped = 1;
	RunMotor = 0;
	Enabled = 0;
}

int TelescopeAxis::getDirection(void)
{
	return Direction;
}

int TelescopeAxis::isStopped(void)
{
	return Stopped;
}

int TelescopeAxis::isEnabled(void)
{
	return Enabled;
}

int TelescopeAxis::getMotionMode(void)
{
	return _motionmode;
}

void TelescopeAxis::setMotionMode(AxisMotionMode Mode)
{
	_motionmode = Mode;
}

void TelescopeAxis::Run(void)
{
	if(!RunMotor)
		return;
		
	if(_motionmode == GOTO)
	{
		
		AccelStepper::run();
		if( AccelStepper::distanceToGo() == 0)
		{
			Stopped = 1;
			_motionmode = SLEW;
		}	
		else
		{
			Stopped = 0;
		}
	}
	else if(_motionmode == SLEW)
	{
		Stopped = 1;
		AccelStepper::runSpeed();
	}
}

void TelescopeAxis::setDirection(int Direct)
{
	Direction = Direct;
}

void  TelescopeAxis::moveTo(long absolute)
{
	_motionmode = GOTO;
	AccelStepper::moveTo(absolute);
}

void TelescopeAxis::stopAxis(int Disable)
{
	AccelStepper::stop(); 
	RunMotor = 0;
	Stopped = 1;
	_motionmode = SLEW;
	//if(Disable)
	//	AccelStepper::disableOutputs();
}

void TelescopeAxis::disableOutputs(void)
{
	AccelStepper::disableOutputs();
	Enabled = 0;
}

void TelescopeAxis::enableOutputs(void)
{
	AccelStepper::enableOutputs();
	Enabled = 1;
}


void TelescopeAxis::startAxis(void)
{
	RunMotor = 1;
}
