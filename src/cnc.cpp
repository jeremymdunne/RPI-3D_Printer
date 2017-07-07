
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
//#include "Heater.h"
//#include "Axis.h"
#include "ConfigParser.h"
//#include "Extruder.h"
#include "Interface.h"
#include "Planner.h"
using namespace std;

using namespace timer;
using namespace stringParser;



class BitBangedSPI{
public:
  BitBangedSPI(int sck, int mosi, int miso, int cs);
  void writeBits(int toWrite, int length);
  int readBits(int length);
private:
  int _sck, _miso, _mosi, _cs;
};

BitBangedSPI::BitBangedSPI(int sck, int mosi, int miso, int cs){
  _sck = sck;
  _mosi = mosi;
  _miso = miso;
  _cs = cs;
  pinMode(_cs,OUTPUT);
  pinMode(_miso, INPUT);
  pinMode(_mosi, OUTPUT);
  pinMode(_sck, OUTPUT);
  digitalWrite(_cs,HIGH);
}

void  BitBangedSPI::writeBits(int toWrite, int length){
  for(int i = 0; i < length; i ++){
   if(toWrite & 0x80) digitalWrite(_mosi, HIGH);
   else digitalWrite(_mosi,LOW);
   toWrite <<= 1;
   digitalWrite(_sck, HIGH);
   digitalWrite(_sck,LOW);
 }
}

int BitBangedSPI::readBits(int length){
  int resp = 0;
  for(int i = 0; i < length; i ++){
    digitalWrite(_sck,HIGH);
    digitalWrite(_sck,LOW);
    resp <<= 0x1;
    if(digitalRead(_miso)) resp |= 1;
  }
  return resp;
}


int main(int argc, char *argv[]){
  wiringPiSetup();
    string command;
  for(int i = 0; i < argc; i ++){
    command += argv[i];
  }
  cout << string::npos << endl;
  vector <PrinterFlags> flags;
  if(command.find("NH") != string::npos)flags.push_back(HEATER_OFF);
  if(command.find("NM") != string::npos)flags.push_back(MOVEMENT_OFF);
  if(command.find("BT") != string::npos){
    flags.push_back(BUFFER_TEST);
    cout << "Buffer Test!" << endl;
  }
  if(command.find("safe") != string::npos)flags.push_back(SAFE_MODE);

  Planner plan("./config/printerHardware.config", "./config/printerSettings.config", "./print.txt",flags);
  PrinterInformation info = plan.getPrinterInformation();
  cout << "Temp: " << info.temperature << endl;
  return 0;
}

void printLoop(){
  //get new line

}
