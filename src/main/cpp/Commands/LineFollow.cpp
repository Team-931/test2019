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
enum Input {None, Left, Right, Both};

struct LineReport : frc::Command {
  Input input;
  int inputAge;
  bool following;

  void Execute() override {
    if(readPhoto(photoleft)) 
      if(readPhoto(photoright)) {
        input = Both;
        following = true;
      }
      else {
        if (input == Right) following = true;
        input = Left;
      }
    else 
      if(readPhoto(photoright)) {
        if (input == Left) following = true;
        input = Right;
      }
      else {
        if(input != None) following = true;
        ++inputAge; return;
      }
    inputAge = 0;
    	frc::SmartDashboard::PutBoolean("LineReportTrue", true);
  }

  bool IsFinished() override {return false;}

  void reset() {
    input = None;
    inputAge = 0;
  }
} lineReport;

LineFollow::LineFollow() : frc::Command("Follow line"){
  // Use Requires() here to declare subsystem dependencies
  // eg. Requires(Robot::chassis.get());
  Requires (& Robot::driveSystem);
  lineReport.Start();
}
static constexpr int ageMax = 5;

// Called just before this Command runs the first time
void LineFollow::Initialize() {
  if (lineReport.inputAge > ageMax) lineReport.reset();
}

void forward (double speed) {
  Robot::driveSystem->TankDrive (speed/2, speed/2, false);
}

void left (double speed) {
  Robot::driveSystem->TankDrive (0, speed, false);
}

void right (double speed) {
  Robot::driveSystem->TankDrive (speed, 0, false);
}

// Called repeatedly when this Command is scheduled to run
void LineFollow::Execute() {
  double speed=.2;
  double joystickreading=-Robot::oi.stick->GetRawAxis(3);
  if(std::abs(joystickreading)>.1)speed=joystickreading;
  switch (lineReport.input) {
    case Left:  if (lineReport.following) left (speed);
                else forward (speed);
                return;
    case Right: if (lineReport.following) right(speed);
                else forward (speed);
                return;
    case None:  Initialize();

    case Both:  forward(speed);
  }
}

// Make this return true when this Command no longer needs to run execute()
bool LineFollow::IsFinished() { return false; }

// Called once after isFinished returns true
void LineFollow::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void LineFollow::Interrupted() {}
