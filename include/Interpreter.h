#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include <string>
#include <string.h>
#include <iostream>
#include "DataTypes.h"
#include "FileReader.h"
#include "stringParser.h"
using namespace std;
using namespace stringParser;

class Interpreter{
//handles commands given to it by the GCode
public:
  Interpreter(string fileLocation);
  codeCommand getNextCommand();
  bool isDone();
private:
  FileReader *file;
  bool isLineGood(string command);
  string getNewLine();
  string trim(string line);

};

#endif
