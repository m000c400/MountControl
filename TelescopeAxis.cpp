#include "TelescopeAxis.h"

TelescopeAxis::TelescopeAxis(uint8_t STEP, uint8_t DIRECTION, uint8_t nENABLE, uint8_t nRESET, uint8_t nSLEEP, uint8_t MODE2, uint8_t MODE1, uint8_t MODE0): AccelStepper(AccelStepper::DRIVER, STEP, DIRECTION)
{
	Direction = 1;
	Stopped = 1;
	RunMotor = 0;
	Enabled = 0;
  TargetPosition = 0;


  Mode0_Pin = MODE0; Mode1_Pin = MODE1; Mode2_Pin = MODE2;
  pinMode(MODE2,OUTPUT); digitalWrite(MODE2,LOW);
  pinMode(MODE1,OUTPUT); digitalWrite(MODE2,LOW);
  pinMode(MODE0,OUTPUT); digitalWrite(MODE2,LOW);  

  pinMode(nENABLE,OUTPUT); digitalWrite(nENABLE,HIGH);
  pinMode(nRESET,OUTPUT); digitalWrite(nRESET,HIGH);
  pinMode(nSLEEP,OUTPUT); digitalWrite(nSLEEP,HIGH);

  AccelStepper::setEnablePin(nENABLE);
  AccelStepper::setPinsInverted(false,false,true);
  AccelStepper::disableOutputs();
  AccelStepper::setCurrentPosition(0x080000);
  
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
  TargetPosition = absolute; 
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

long TelescopeAxis::currentPosition(void)
{
  return AccelStepper::currentPosition();
}

void TelescopeAxis::setCurrentPosition(long Position)
{
  AccelStepper::setCurrentPosition(Position);
}

void TelescopeAxis::startAxis(void)
{
	RunMotor = 1;
}


