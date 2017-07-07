#ifndef _DATATYPES_H
#define _DATATYPES_H


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <wiringPi.h>


#define SET_EXTRUDER_TEMP "M104"
#define SET_EXTRUDER_TEMP_AND_WAIT "M109"
#define SET_UNITS_TO_MM "G21"
#define MOVE_WITH_EXTRUSION "G1"

#define MOVE "G0"
#define SET_ABSOLUTE_MODE "G90"
#define SET_RELATIVE_MODE "G91"
#define SET_EXTRUDER_ABSOLUTE_MODE "M82"
#define TURN_FILAMENT_FAN_ON "M106"
#define TURN_FILAMENT_FAN_OFF "M107"
#define MOVE_TO_ORIGIN "G28"
#define SET_POSITION "G92"
#define GET_ZERO_POSITION "M117"
#define SET_BED_TEMPERATURE "M140"
#define IDLE "M84"
#define DONE_WITH_PRINT "DONE_WITH_PRINT"

using namespace std;
/*

*/
#define ABSOLUTE_COORDINATE 123
#define RELATIVE_COORDINATE 345
#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

#define DNE -1234


enum PrinterFlags{
  MOVEMENT_OFF,
  EXTRUDER_OFF,
  BUFFER_TEST,
  HEATER_OFF,
  SAFE_MODE
};
/*
enum GCodeCommands {
  SET_EXTRUDER_TEMP,
  SET_EXTRUDER_TEMP_AND_WAIT,
  MOVE_WITH_EXTRUSION,
  MOVE,
  SET_ABSOLUTE_MODE,
  SET_RELATIVE_MODE,
  SET_EXTRUDER_ABSOLUTE_MODE,
  TURN_FILAMENT_FAN_OFF,
  MOVE_TO_ORIGIN,
  SET_POSITION,
  GET_ZERO_POSITION,
  SET_BED_TEMPERATURE,
  IDLE,
  DONE_WITH_PRINT,
  UNKOWN
};
*/





class Point3d{
public:
  float x, y, z;
};

class Speed3d{
public:
  float x;
  float y;
  float z;
  void setAll(float speed){
    x = speed;
    y = speed;
    z = speed;
  }
};

class InterfaceInformation{
public:
  Point3d position;
  float extrudedAmount;
  int temperature;
};

class PrinterInformation{
public:
  Point3d position;
  float extrudedAmount;
  int temperature;
  float percentDone;
  long printTime;
};

class MovementCommand{
  //used to tell printer to move to point
public:
  int coordinateType = DNE;
  Point3d targetPosition;
  Speed3d targetSpeed;
  float extrudePosition = DNE;
  void toString(){
    cout << "X: " << targetPosition.x << " Y: " << targetPosition.y << " Z: " << targetPosition.z << " E: " << extrudePosition << endl;
  }
};

class codeCommand{
public:
  //lists all posible command types
  string command;
  float x = DNE,y = DNE,z = DNE, e = DNE;  // g1 or g0
  int s = DNE,f = DNE;
  string fullCommand;
};
class InterfaceCommand{
  //essentially a codeCommand with an already parsed MovementCommand
public:
    vector <MovementCommand> lineSegments;
    string command; //M107, etc
    float x = DNE,y = DNE,z = DNE, e = DNE;  // g1 or g0
    int s = DNE,f = DNE;
    string fullCommand;
    void set(codeCommand printCommand){
      command = printCommand.command;
      x = printCommand.x;
      y = printCommand.y;
      z = printCommand.z;
      e = printCommand.e;
      f = printCommand.f;
      s = printCommand.s;
      fullCommand = printCommand.fullCommand;
    }

  };

class VelocityTrapezoid{
public:
    VelocityTrapezoid(long startSpeedMMSec, long endSpeedMMSec, long maxSpeedMMSec, float accelMMSS, float deccelMMSS, float mmToTravel, long steps){
        //velocities = new int[steps];
        float mmPerStep = mmToTravel/steps;
        long plateauReachMM = (maxSpeedMMSec - startSpeedMMSec)/accelMMSS;
        long plateauEndMM = mmToTravel - (maxSpeedMMSec - endSpeedMMSec)/deccelMMSS;
        //minSpeed = startSpeedMMSec;
        //maxSpeed = maxSpeedMMSec;
        if(maxSpeedMMSec < startSpeedMMSec){
          for(int i = 0; i < steps; i ++) velocities.push_back(endSpeedMMSec);
        }
        else{
          if(plateauReachMM < plateauEndMM){
            for(int i = 0; i < plateauReachMM/mmPerStep; i ++){
              velocities.push_back(startSpeedMMSec + i*mmPerStep*accelMMSS);
              if(velocities[velocities.size()] > maxSpeedMMSec) velocities[i] = maxSpeedMMSec;
            }
            for(int i = plateauReachMM/mmPerStep; i < (plateauEndMM-plateauReachMM)/mmPerStep; i ++){
              velocities.push_back(maxSpeedMMSec);
            }
            for(int i = plateauReachMM/mmPerStep; i < steps; i ++){
              velocities.push_back(maxSpeedMMSec - (i-plateauReachMM/mmPerStep)*mmPerStep*deccelMMSS);
              if(velocities[i] < endSpeedMMSec) velocities[i] = endSpeedMMSec;
            }
          }
          else{
            long midStep = deccelMMSS/(accelMMSS + deccelMMSS) * steps;
            for(int i = 0; i < midStep; i ++){
              velocities.push_back(startSpeedMMSec + i*mmPerStep*accelMMSS);
                if(velocities[velocities.size()] > maxSpeedMMSec) velocities[i] = maxSpeedMMSec;
            }
            for(int i = midStep; i < steps; i ++){
              velocities.push_back( maxSpeedMMSec - (i-midStep)*mmPerStep*deccelMMSS);
              if(velocities[i] < endSpeedMMSec) velocities[i] = endSpeedMMSec;
            }
          }
        }
  }

  VelocityTrapezoid(long startSpeedMMSec, long maxSpeedMMSec, float accelMMSS, float deccelMMSS, float mmToTravel, long steps){
    float mmPerStep = mmToTravel/steps;
    float deltaSpeed = maxSpeedMMSec- startSpeedMMSec;
    float accelMMRequired = deltaSpeed/accelMMSS;
    float decelMMRequired = deltaSpeed/deccelMMSS;
    maxSpeed = maxSpeedMMSec;
    minSpeed = startSpeedMMSec;
    //cout << "Total Steps: " << steps << " accel stop step: " << accelMMRequired/mmPerStep << " plateau stop: " <<  steps - decelMMRequired/mmPerStep << endl;
    if(accelMMRequired + decelMMRequired < mmToTravel){
      //cout << "Accel Per Step: " << accelMMSS*mmPerStep << endl;
      //complete a plautue
      for(long i = 0; i < accelMMRequired/mmPerStep; i ++){
        velocities.push_back(startSpeedMMSec + i*accelMMSS*mmPerStep);
      }
      for(long i = accelMMRequired/mmPerStep; i < steps - decelMMRequired/mmPerStep; i ++){
        velocities.push_back(maxSpeedMMSec);
      }
      for(long i = steps - decelMMRequired/mmPerStep; i < steps; i ++){
        velocities.push_back(maxSpeedMMSec - (i-(steps - decelMMRequired/mmPerStep))*deccelMMSS*mmPerStep);
      }
    }
    else{
      //not enough space, dont bother yet
      for(long i = 0; i < steps; i ++){
        velocities.push_back(startSpeedMMSec);
      }
    }

}

  int getSpeed(int step){
    int v = velocities[step];
    if(v > maxSpeed){
      v = maxSpeed;
      //cout << "SPEED TO HIGH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n\n\n\n\n\\n\n\n\n\n\n\n\n\n\n\n" << endl;
      //delay(10000);
    }
    else if (v < minSpeed){
      v = minSpeed;
      //cout << "SPEED TO LOW!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n\n\n\n\n\\n\n\n\n\n\n\n\n\n\n\n" << endl;
      //delay(10000);
    }
  return velocities[step];
  }
private:
  float maxSpeed = 0;
  float minSpeed = 0;
  vector<int> velocities;

};

#endif
