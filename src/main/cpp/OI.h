/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include <frc/Joystick.h>
#include <frc/Buttons/JoystickButton.h>
using namespace frc;
class OI {
public:
	Joystick* stick{new Joystick(0)};
	JoystickButton HighGearBtn{stick, 4};
	JoystickButton LowGearBtn{stick, 2};
	JoystickButton linesensor{stick, 7};
	Command* lineFollow;
	OI();
};
