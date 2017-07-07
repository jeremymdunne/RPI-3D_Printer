#ifndef _PLANNER_H
#define _PLANNER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include <string>
#include <string.h>
#include <fstream>
#include <thread>
#include <exception>
#include "DataTypes.h"
#include "stringParser.h"
#include "Interpreter.h"
#include "timer.h"
#include "FileReader.h"
#include "ConfigParser.h"
#include "Interface.h"

using namespace std;


class Planner{

public:
  Planner(string hardwareSettingsLocation, string softwareSettingsLocation, string printFileLocation, vector <PrinterFlags> flags);
  PrinterInformation getPrinterInformation();
  void stop();

private: 
  //functions
  void bufferPopulator();
  void printLoop();
  vector <MovementCommand> planLine(codeCommand end);
  vector <MovementCommand> planLine(codeCommand end, Point3d location, float extrudedAmount);
  void updateUser();
  void initializeSettings(string hardwareSettingsLocation);
  void bufferController();
  //variables
  long timeStart;
  bool shouldRun = true;
  int curFeedrate = 0;
  bool isBufferTest = false;
  bool isInAbsoluteMode = true;
  bool isBufferingDone = false;
  float lastFeedrate = 100;
  bool safeMode = false;
  bool pathPlanning = false;
  float planningResolution = .1; //in MM
  int bufferLength = 200;
  bool canBufferWrite = true;
  vector <InterfaceCommand> interfaceBuffer;
  Interface *printer;
  Interpreter *gCodeParser;
  vector <PrinterFlags> printerFlags;
  vector <InterfaceCommand> unHandledCommands;

  Speed3d maxAccelSpeed;
  Speed3d startSpeed; //for acceleration
  float accelerationXY[2] = {0,0}; //mm/s^2
  float deccelerationXY[2] = {0,0};
  bool shouldAccelerate = false;
  float minDistMMToAccel = 0;

};

#endif
