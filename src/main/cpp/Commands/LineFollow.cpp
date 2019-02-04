/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Commands/LineFollow.h"
#include <frc/AnalogInput.h>
#include "Robot.h"

static frc::AnalogInput photoleft(0), photoright(1);
static constexpr double threshold = 1;
static inline bool readPhoto (frc::AnalogInput const & input) {
  return input.GetVoltage() > threshold;
}

LineFollow::LineFollow() : frc::Command("Follow line"){
  // Use Requires() here to declare subsystem dependencies
  // eg. Requires(Robot::chassis.get());
  Requires (& Robot::driveSystem);
}

// Called just before this Command runs the first time
void LineFollow::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void LineFollow::Execute() {
  double speed=.2;
  double joystickreading=-Robot::oi.stick->GetRawAxis(3);
  if(std::abs(joystickreading)>.1)speed=.6*joystickreading;
  double Swerveamount=.25;//std::copysign(.25,speed);
  if(readPhoto(photoleft)) 
        if(readPhoto(photoright)) Robot::driveSystem->ArcadeDrive(speed, 0, false);
                            else   Robot::driveSystem->ArcadeDrive(speed, -Swerveamount, false);
  else  if(readPhoto(photoright)) Robot::driveSystem->ArcadeDrive(speed, +Swerveamount, false);
                            else   Robot::driveSystem->ArcadeDrive(0, 0, false);
}

// Make this return true when this Command no longer needs to run execute()
bool LineFollow::IsFinished() { return false; }

// Called once after isFinished returns true
void LineFollow::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void LineFollow::Interrupted() {}

bool LineFollow::Report() {return readPhoto(photoleft)||readPhoto(photoright);
}