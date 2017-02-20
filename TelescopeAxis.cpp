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
#include "TelescopeAxis.h"

TelescopeAxis::TelescopeAxis(uint8_t STEP, uint8_t DIRECTION, uint8_t ENABLE, uint8_t SLEEP, uint8_t RESET, uint8_t nSTEP, uint8_t nDIRECTION, uint8_t nENABLE): AccelStepper(AccelStepper::DRIVER, STEP, DIRECTION)
{
	Direction = 1;
	Stopped = 1;
	RunMotor = 0;
	Enabled = 0;
  TargetPosition = 0;

  pinMode(RESET,OUTPUT); digitalWrite(RESET,HIGH);
  pinMode(SLEEP,OUTPUT); digitalWrite(SLEEP,HIGH);

  pinMode(ENABLE,OUTPUT); //digitalWrite(ENABLE,LOW);

  AccelStepper::setEnablePin(ENABLE);
  AccelStepper::setPinsInverted(nDIRECTION,nSTEP,nENABLE);
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


