#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "Axis.h"
#include "Extruder.h"
#include "DataTypes.h"
#include "ConfigParser.h"
#include "vector"

class Interface{
//handles all hardware
public:
    Interface(string hardwareSettingsLocation, vector <PrinterFlags> printFlags);
    void moveToPosition(MovementCommand command);
    void resetAxis(int axis);
    void resetAllAxis();
    void zeroExtruder();
    InterfaceInformation getInformation();
    Point3d getPosition();
    float getRunningExtrudedAmount();
    float getTotalExtrudedAmount();
    void setExtruderTemp(int temp);
    void getExtruderToTemp(int temp);
    void getExtruderToTemp();
    void turnOff();
    void turnOffPower();
    void turnOn();
    void emergencyStop();
    void safe();
    void coolDownExtruder();

private:
  float zAxisOffset;
  float zRaiseBeforeReset;
  bool shouldHeat = true;
  bool shouldMove = true;
  Axis *xMotor;
  Axis *yMotor;
  Axis *zMotor;
  Extruder *extruder1;
  int psuPin;
  Speed3d maxSpeed;
  string sysType;

};


#endif
