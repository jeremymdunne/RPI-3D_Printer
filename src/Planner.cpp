#include "Planner.h"



Planner::Planner(string hardwareSettingsLocation, string softwareSettingsLocation, string printFileLocation, vector <PrinterFlags> flags){
  cout << "File: " << hardwareSettingsLocation << endl;
  printerFlags = flags;
  cout << "Planner called " << endl;
  for(int i = 0; i < flags.size(); i ++){
    if(flags[i] == BUFFER_TEST){
      cout << "Buffer Test!!" << endl;
      isBufferTest=true;
    }
    else if(flags[i] == SAFE_MODE){
      cout << "Making Printer Safe!" << endl;
      safeMode = true;
    }
  }
  timeStart = millis();
  initializeSettings(softwareSettingsLocation);
  printer = new Interface(hardwareSettingsLocation, printerFlags);
  cout  << "Interface initialized! " << endl;
  if(safeMode){
    printer->turnOn();
    printer->safe();

  }
  else{
    gCodeParser = new Interpreter("./print.txt");
    cout  << "Parser initialized! " << endl;
    thread bufferThread(&Planner::bufferController,this);
    bufferThread.detach();
    cout  << "Buffer initialized! " << endl;
    thread printLoopThread(&Planner::printLoop,this);
    cout  << "PrintLoop initialized! " << endl;
    printLoopThread.join();
  }
}

PrinterInformation Planner::getPrinterInformation(){
  InterfaceInformation printerInfo = printer->getInformation();
  PrinterInformation info;
  info.position = printerInfo.position;
  info.extrudedAmount = printerInfo.extrudedAmount;
  info.temperature = printerInfo.temperature;
  return info;
}


void Planner::initializeSettings(string softwareSettingsLocation){
  ConfigParser softwareSettings(softwareSettingsLocation);

  //accuracy
  softwareSettings.getValue("Resolution",&planningResolution);
  cout << "Resolution: " << planningResolution << endl;
  softwareSettings.getValue("BufferLength", &bufferLength);
  softwareSettings.getValue("ShouldPathPlan", &pathPlanning);
  cout << "Should pathPlanning: " << pathPlanning << endl;
  softwareSettings.getValue("StartXSpeed", &startSpeed.x);
  softwareSettings.getValue("StartYSpeed", &startSpeed.y);
cout << "StartXSpeed: " << startSpeed.x << endl;
  softwareSettings.getValue("XAccel", &accelerationXY[0]);
  softwareSettings.getValue("YAccel", &accelerationXY[1]);

  softwareSettings.getValue("XDeccel", &deccelerationXY[0]);
  softwareSettings.getValue("YDeccel", &deccelerationXY[1]);

  softwareSettings.getValue("MaxAccelXSpeed", &maxAccelSpeed.x);
  softwareSettings.getValue("MaxAccelYSpeed", &maxAccelSpeed.y);
  softwareSettings.getValue("ShouldAccel", &shouldAccelerate);
  softwareSettings.getValue("MinDistMMToAccel", &minDistMMToAccel);


  cout << "Accel: " << accelerationXY[0] << endl;
}

void Planner::printLoop(){
  int line = 0;
  bool isDone = false;
  InterfaceCommand newPrintCommand;
    printer->turnOn();
    //delay(100);
    cout << "Size: " << interfaceBuffer.size() << endl;
     cout << "isDone: " << isDone << " shouldRun " << shouldRun << endl;
    //cout << (interfaceBuffer[interfaceBuffer.size()].command) << endl;
    //cout << "Waiting...." << interfaceBuffer.size() << "  " << isBufferingDone << endl;
    while(interfaceBuffer.size() < bufferLength & !isBufferingDone){
       cout << "Waiting...." << interfaceBuffer.size() << "  " << isBufferingDone << endl;
       delay(10);
      // cout << "Waiting...." << interfaceBuffer.size() << "  " << isBufferingDone << endl;
     }
     //cout << "isDone: " << isDone << " shouldRun " << shouldRun << endl;
    while(!isDone && shouldRun){
      while(interfaceBuffer.size() < 1) delay(1);
      //get new command
      //cout << "Size: " << interfaceBuffer.size() << endl;
      try{
         newPrintCommand = interfaceBuffer[0];
      }
      catch(exception &e){
        cout << "VECTOR 0 ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << e.what() << "\n\n\n\n\n\n\n\n\n\n\n" << endl;
        delay(5000);
      }
	  cout << "Next!" << endl;
      //cout << " 1 " << endl;
      if(isBufferTest){
        cout << "Line: " << line << " command: " << newPrintCommand.command << endl;
        cout << "X: " << newPrintCommand.x << "\nY: "<<newPrintCommand.y<< "\nZ: " << newPrintCommand.z<<"\nE: "<<newPrintCommand.e <<endl;
      }
      if(newPrintCommand.command == SET_EXTRUDER_TEMP){
        printer->setExtruderTemp(newPrintCommand.s);
		cout << "Extruder temp set!" << endl;
      }

      else if(newPrintCommand.command == SET_EXTRUDER_TEMP_AND_WAIT){
		  cout << "Get to temp Planner" << endl;
        printer->getExtruderToTemp(newPrintCommand.s);
      }

      else if(newPrintCommand.command == MOVE || newPrintCommand.command == MOVE_WITH_EXTRUSION){
		  //cout << "Writing " << endl;
		  //cout << newPrintCommand.lineSegments[newPrintCommand.lineSegments.size() - 1].extrudePosition << endl;
        //cout << "Command: " << newPrintCommand.fullCommand << endl;
        for(int i = 0; i < newPrintCommand.lineSegments.size(); i ++){

          //cout << "Speeds: X: " << newPrintCommand.lineSegments[i].targetSpeed.x << " Y: " << newPrintCommand.lineSegments[i].targetSpeed.y <<  " Z: " << newPrintCommand.lineSegments[i].targetSpeed.z << endl;
          //cout <<" X Pos: " << newPrintCommand.lineSegments[i].targetPosition.x << " Y: " << newPrintCommand.lineSegments[i].targetPosition.y << " Z: " << newPrintCommand.lineSegments[i].targetPosition.z << " E: " << newPrintCommand.lineSegments[i].extrudePosition << endl;
          printer->moveToPosition(newPrintCommand.lineSegments[i]);
          //cout << "Done" << endl;
          //delay(10);
        }
		//cout << "Wirte success" << endl;
      }

      else if(newPrintCommand.command == MOVE_WITH_EXTRUSION){

      }
      else if(newPrintCommand.command == SET_ABSOLUTE_MODE){
          isInAbsoluteMode = true;
      }

      else if(newPrintCommand.command == SET_RELATIVE_MODE){
        isInAbsoluteMode = false;
      }

      else if(newPrintCommand.command == SET_EXTRUDER_ABSOLUTE_MODE){
          //TODO
      }
      else if(newPrintCommand.command == TURN_FILAMENT_FAN_OFF){
        //TODO
      }
      else if(newPrintCommand.command == MOVE_TO_ORIGIN){
        if(newPrintCommand.x != DNE) printer->resetAxis(X_AXIS);
        if(newPrintCommand.y != DNE) printer->resetAxis(Y_AXIS);
        if(newPrintCommand.z != DNE) printer->resetAxis(Z_AXIS);
      }
      else if(newPrintCommand.command == SET_POSITION){
        if(newPrintCommand.e != DNE){
          printer->zeroExtruder();
        }
      }
      else if(newPrintCommand.command == SET_UNITS_TO_MM){
        //TODO
      }
      else if(newPrintCommand.command == GET_ZERO_POSITION){
        //Ignore
      }
      else if(newPrintCommand.command == TURN_FILAMENT_FAN_ON){
        //TODO
      }
      else if(newPrintCommand.command == SET_BED_TEMPERATURE){
        //TODO
      }
      else if(newPrintCommand.command == IDLE){
        //TODO
      }
      else if(newPrintCommand.command == DONE_WITH_PRINT){
        //TODO
        isDone = true;
      }


      else {
        unHandledCommands.push_back(newPrintCommand);
        cout << "Pushed back?!" << endl;
      }
      //cout << " 4 " << endl;
      //erase
      canBufferWrite = false;
      delayMicroseconds(800);
      interfaceBuffer.erase(interfaceBuffer.begin());
      //delayMicroseconds(3000);
      //cout << "Size: " << interfaceBuffer.size() << endl;
      canBufferWrite = true;
      //cout << " 5 " << endl;
      //thread t(&Planner::updateUser,this);
      //t.detach();
      updateUser();
	  cout << "Update Success" << endl;
    }

  cout << "Done!" << endl;
  printer->turnOff();
  if(unHandledCommands.size() > 0)
  cout << "unHandledCommands: " << endl;
  for(int i = 0; i < unHandledCommands.size(); i ++){
    cout << unHandledCommands[i].command << endl;
  }

}

void Planner::updateUser(){
  //long timeStart = micros();
  //print out location, temp, and current time
  InterfaceInformation printerStatus = printer->getInformation();
  cout << "\nPrinter Position: \nX: "<<  printerStatus.position.x  << "\tY: " <<  printerStatus.position.y << "\tZ: " <<  printerStatus.position.z << "\tE: " <<  printerStatus.extrudedAmount << "\tTemp: " <<  printerStatus.temperature << endl;
  float secs = (millis()-timeStart)/1000.0;
  int minutes = ((int)secs/60)%60;
  int hours = (int)(secs/60.0/60);
  cout << "Cur Print Time: " << hours << " Hour(s): " << minutes << " Minutes: " << (int)secs%60 << " Seconds" << endl;
  //cout << "Time for update: " << micros() - timeStart << endl;
}

void Planner::bufferPopulator(){

}

void Planner::bufferController(){
  bool isDone = false;
  Point3d location;
  location.x = 0;
  location.y = 0;
  location.z = 0;
  float extrudedAmount = 0;

      try{
  while(!isDone && shouldRun  && !gCodeParser->isDone()){
    //cout << "Fell out" << endl;
    while(interfaceBuffer.size() < bufferLength && !gCodeParser->isDone() && shouldRun){
      codeCommand newPrintCommand;
      InterfaceCommand newInterfaceCommand;
      vector <MovementCommand> plannedLine;
        //get new
        //cout<<"Buffer size: " << interfaceBuffer.size() << endl;
        newPrintCommand = gCodeParser->getNextCommand();
        newInterfaceCommand.set(newPrintCommand);
        if(newPrintCommand.command == MOVE || newPrintCommand.command == MOVE_WITH_EXTRUSION){
          //plan
          plannedLine = planLine(newPrintCommand, location, extrudedAmount);
          location = plannedLine[plannedLine.size() - 1].targetPosition;
          extrudedAmount = plannedLine[plannedLine.size() -1].extrudePosition;
          newInterfaceCommand.lineSegments = plannedLine;
        }
        if(newPrintCommand.command == MOVE_TO_ORIGIN){
          if(newPrintCommand.x != DNE) location.x = 0;
          if(newPrintCommand.y != DNE) location.y = 0;
          if(newPrintCommand.z != DNE) location.z = 0;
        }
        if(newPrintCommand.command == SET_POSITION){
          if(newPrintCommand.e != DNE) extrudedAmount = 0;
        }
        while(!canBufferWrite){
          //cout << "Collision! " << endl;
          delayMicroseconds(1);

        }

        interfaceBuffer.push_back(newInterfaceCommand);
        //cout << "Size: " << interfaceBuffer.size()*sizeof(newInterfaceCommand) << endl;
    }
    //delay(1);

  }
}
catch(exception &e){
  cout << "Error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << e.what() << "\n\n\n\n\n\n\n\n\n\n" <<endl;
  delay(1000);
}
  //push back a done with print
  cout << "Buffering Done! " << endl;
  InterfaceCommand lastCommand;
  lastCommand.command = DONE_WITH_PRINT;
  interfaceBuffer.push_back(lastCommand);
  isBufferingDone = true;
  isDone = true;

}

vector <MovementCommand> Planner::planLine(codeCommand end, Point3d location, float extrudedAmount){
  //get current location
  if(end.f != DNE) lastFeedrate = end.f;
  vector <MovementCommand> line;
  float partsPerMM = 1.0/planningResolution; //instead of step based accuracy, use mm accuracy

  if(isInAbsoluteMode){
    //cout << "\n\nDesired Ends: X: " << end.x << " Y: " << end.y << " Z: " << end.z << endl;
    float deltaX = 0, deltaY = 0, deltaZ = 0,deltaE = 0;
    if(end.f != DNE) curFeedrate = end.f;
    if(end.x != DNE) deltaX = end.x - location.x;
    if(end.y != DNE) deltaY = end.y - location.y;
    if(end.z != DNE) deltaZ = end.z - location.z;
    if(end.e != DNE) deltaE = end.e - extrudedAmount;
    //saftey check, idk why i have to do this but I do.....
    if(fabs(deltaX) < .1) deltaX = 0;
    if(fabs(deltaY) < .1) deltaY = 0;
    if(fabs(deltaZ) < .1) deltaZ = 0;
    if(fabs(deltaE) < .1) deltaE = 0;
    //cout << "Delta XYZ: " << deltaX << " " << deltaY << " " << deltaZ << " " << deltaE << endl;
    //cout << "Location: " << location.x << " " << location.y << " " << location.z << " " << extrudedAmount << endl;
    if(deltaX == 0 && deltaY == 0 && deltaZ == 0){
      MovementCommand command;
      command.coordinateType = ABSOLUTE_COORDINATE;
      command.targetPosition.x = location.x;
      command.targetPosition.y = location.y;
      command.targetPosition.z = location.z;
      if(deltaE != 0)
      command.extrudePosition = extrudedAmount + deltaE;
      else
      command.extrudePosition = extrudedAmount;
      command.targetSpeed.setAll(lastFeedrate);
      line.push_back(command);
      //delete command;
    }
    else if(deltaZ != 0){
      //cout << "Z Movement! " << endl;
      MovementCommand command;
      command.coordinateType = ABSOLUTE_COORDINATE;
      command.targetPosition.z = end.z;
      command.targetPosition.x = location.x;
      command.targetPosition.y = location.y;
      command.extrudePosition = extrudedAmount;
      command.targetSpeed.setAll(lastFeedrate);
      line.push_back(command);
      //delete command;
    }

    else{
      int segments = (fabs(deltaX) + fabs(deltaY))*partsPerMM;
      //trapezoid speed
      VelocityTrapezoid *xSpeeds;
      VelocityTrapezoid *ySpeeds;
      float maxSpeedX;
      float maxSpeedY;
      if(lastFeedrate < maxAccelSpeed.x) maxSpeedX = lastFeedrate;
      else maxSpeedX = maxAccelSpeed.x;
      if(lastFeedrate < maxAccelSpeed.y) maxSpeedY = lastFeedrate;
      else maxSpeedY = maxAccelSpeed.y;
      //cout << "Last Feed Rate: " << lastFeedrate << endl;
      //cout << "maxSpeedX: " << maxSpeedX << endl;
      //delay(1000);

      if(deltaX!= 0 && shouldAccelerate) xSpeeds = new VelocityTrapezoid(startSpeed.x,maxSpeedX,accelerationXY[0], deccelerationXY[0],fabs(deltaX),segments);
      if(deltaY!=0 && shouldAccelerate) ySpeeds = new VelocityTrapezoid(startSpeed.y,maxSpeedY,accelerationXY[1], deccelerationXY[1],fabs(deltaY),segments);
      float timeTaken = 0;
      for(int i = 0; i < segments; i ++){
        MovementCommand command;
        command.coordinateType = ABSOLUTE_COORDINATE;
        command.targetPosition.x = location.x + (deltaX/segments) *i;
        command.targetPosition.y = location.y + (deltaY/segments)*i;
        command.targetPosition.z = location.z;
        command.extrudePosition = extrudedAmount + (deltaE/segments) * i;
        command.targetSpeed.setAll(lastFeedrate);
        if(lastFeedrate > startSpeed.x) command.targetSpeed.x = startSpeed.x;
        if(lastFeedrate > startSpeed.y) command.targetSpeed.y = startSpeed.y;
        if(deltaX!=0 && shouldAccelerate){
          command.targetSpeed.x = xSpeeds->getSpeed(i);
          //cout << "SpeedX: " << command.targetSpeed.x << endl;
        }
        if(deltaY!=0 && shouldAccelerate)command.targetSpeed.y = ySpeeds->getSpeed(i);
        //if(deltaX!=0)cout << "X Speed: " << xSpeeds->getSpeed(i) << endl;
        //if(deltaY!=0)cout << "Y Speed: " << ySpeeds->getSpeed(i) << endl;
        line.push_back(command);

        //delete command;
      }
        if(deltaX!=0 && shouldAccelerate)delete xSpeeds;
        if(deltaY!=0 && shouldAccelerate)delete ySpeeds;
    }
  }
  else{
    //assume non diagonal move
    MovementCommand command;
    command.coordinateType = RELATIVE_COORDINATE;
    if(end.x != DNE) command.targetPosition.x = end.x;
    if(end.y != DNE) command.targetPosition.y = end.y;
    if(end.z != DNE) command.targetPosition.z = end.z;
    if(end.e != DNE) command.extrudePosition = end.e;
    command.targetSpeed.setAll(lastFeedrate);
    line.push_back(command);
    //delete command;
  }

  //cout << " Size: " << line.size() << endl;
 return line;
}

vector <MovementCommand> Planner::planLine(codeCommand end){
  //get current location
  if(end.f != DNE) lastFeedrate = end.f;
  vector <MovementCommand> line;
  float partsPerMM = 1.0/planningResolution; //instead of step based accuracy, use mm accuracy

  if(isInAbsoluteMode){
    cout << "\n\nDesired Ends: X: " << end.x << " Y: " << end.y << " Z: " << end.z << endl;
    Point3d location = printer->getPosition();
    float extrudedAmount = printer->getRunningExtrudedAmount();
    float deltaX = 0, deltaY = 0, deltaZ = 0,deltaE = 0;
    if(end.f != DNE) curFeedrate = end.f;
    if(end.x != DNE) deltaX = end.x - location.x;
    if(end.y != DNE) deltaY = end.y - location.y;
    if(end.z != DNE) deltaZ = end.z - location.z;
    if(end.e != DNE) deltaE = end.e - extrudedAmount;
    //saftey check, idk why i have to do this but I do.....
    if(fabs(deltaX) < .1) deltaX = 0;
    if(fabs(deltaY) < .1) deltaY = 0;
    if(fabs(deltaZ) < .1) deltaZ = 0;
    if(fabs(deltaE) < .1) deltaE = 0;
    cout << "Delta XYZ: " << deltaX << " " << deltaY << " " << deltaZ << " " << deltaE << endl;
    //cout << "Location: " << location.x << " " << location.y << " " << location.z << " " << extrudedAmount << endl;
    if(deltaX == 0 && deltaY == 0 && deltaZ == 0){
      MovementCommand command;
      command.coordinateType = ABSOLUTE_COORDINATE;
      command.targetPosition.x = location.x;
      command.targetPosition.y = location.y;
      command.targetPosition.z = location.z;
      if(deltaE != 0)
      command.extrudePosition = extrudedAmount + deltaE;
      else
      command.extrudePosition = extrudedAmount;
      command.targetSpeed.setAll(lastFeedrate);
      line.push_back(command);
    }
    else if(deltaZ != 0){
      cout << "Z Movement! " << endl;
      MovementCommand command;
      command.coordinateType = ABSOLUTE_COORDINATE;
      command.targetPosition.z = end.z;
      command.targetPosition.x = location.x;
      command.targetPosition.y = location.y;
      command.extrudePosition = extrudedAmount;
      command.targetSpeed.setAll(lastFeedrate);
      line.push_back(command);
    }

    else{
      int segments = (fabs(deltaX) + fabs(deltaY))*partsPerMM;
      for(int i = 0; i < segments; i ++){
        MovementCommand command;
        command.coordinateType = ABSOLUTE_COORDINATE;
        command.targetPosition.x = location.x + (deltaX/segments)*i;
        command.targetPosition.y = location.y + (deltaY/segments)*i;
        command.targetPosition.z = DNE;
        command.extrudePosition = extrudedAmount + (deltaE/segments) * i;
        command.targetSpeed.setAll(lastFeedrate);
        line.push_back(command);
      }
    }
  }
  else{
    //assume non diagonal move
    MovementCommand command;
    command.coordinateType = RELATIVE_COORDINATE;
    if(end.x != DNE) command.targetPosition.x = end.x;
    if(end.y != DNE) command.targetPosition.y = end.y;
    if(end.z != DNE) command.targetPosition.z = end.z;
    if(end.e != DNE) command.extrudePosition = end.e;
    command.targetSpeed.setAll(lastFeedrate);
    line.push_back(command);
  }

  //cout << " Size: " << line.size() << endl;
 return line;
}
