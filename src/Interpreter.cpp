#include "Interpreter.h"

Interpreter::Interpreter(string fileLocation){
  file = new FileReader(fileLocation);
  ofstream sanitizedPrintFile("./bin/SANITIZED_PRINT_FILE.txt", ios::out | ios::trunc);
  //TODO fix this exception....
  try{
    string newLine;
    while(!file->isDone()){
      newLine = getNewLine();
      if(newLine != ""){
        newLine = this->trim(newLine);
        sanitizedPrintFile << newLine << endl;
      }
    }
  }
  catch (exception &e){
    cout << e.what() << endl;
  }
  sanitizedPrintFile.close();
  file->close();
  file = new FileReader("./bin/SANITIZED_PRINT_FILE.txt");
}

bool Interpreter::isDone(){
  return file->isDone();
}

string Interpreter::trim(string line){
  if(line.find(';') != string::npos)
   line = line.substr(0, line.find(';'));
  if(line.at(line.length() - 1) == ' ')
    line = line.substr(0, line.length() - 1);
  return line;
}

codeCommand Interpreter::getNextCommand(){
  codeCommand toReturn;
  toReturn.fullCommand = this->getNewLine();
  toReturn.command = toReturn.fullCommand.substr(0,toReturn.fullCommand.find(' '));
  if(doesElementExist(&toReturn.fullCommand, 'X')){
    toReturn.x = getElementValue(&toReturn.fullCommand, 'X', ' ', false);
  }
  if(doesElementExist(&toReturn.fullCommand, 'Y')){
    toReturn.y = getElementValue(&toReturn.fullCommand, 'Y', ' ', false);
  }
  if(doesElementExist(&toReturn.fullCommand, 'Z')){
    toReturn.z = getElementValue(&toReturn.fullCommand, 'Z', ' ', false);
  }
  if(doesElementExist(&toReturn.fullCommand, 'E')){
    toReturn.e = getElementValue(&toReturn.fullCommand, 'E', ' ', false);
  }
  if(doesElementExist(&toReturn.fullCommand, 'F')){
    toReturn.f = (int)getElementValue(&toReturn.fullCommand, 'F', ' ', false);
  }
  if(doesElementExist(&toReturn.fullCommand, 'S')){
    toReturn.s = (int)getElementValue(&toReturn.fullCommand, 'S', ' ', false);
  }
  return toReturn;
}

bool Interpreter::isLineGood(string line){
  //make sure an M or G is in the first char
  if(line.at(0) == 'M' || line.at(0) == 'G')
  return true;
  return false;
}

string Interpreter::getNewLine(){
  //while loop until line is legit
  bool isLineLegit = false;
  string toCheck = "";
  while(!isLineLegit && !file->isDone()){
    toCheck = file->getLine();
    isLineLegit = isLineGood(toCheck);
  }
  return toCheck;
}
