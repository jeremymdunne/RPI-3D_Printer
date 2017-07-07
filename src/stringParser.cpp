
#include "stringParser.h"

string stringParser::getNextElement(string *toParse, char delim, bool shouldModify){
  string toReturn;
  int next = toParse->find(delim);
  if(next == string::npos){
    toReturn = ELEMENT_DOES_NOT_EXIST;
    if(shouldModify) *toParse = "";
  }
  else{
    string toReturn = toParse->substr(0, next);
    if(shouldModify) *toParse = toParse->substr(next + 1);
  }
  return toReturn;
}

bool stringParser::doesElementExist(string *toParse, char start){
  if(toParse->find(start) == string::npos) return false;
  return true;
}

string stringParser::getElement(string *toParse, char start, char delim, bool shouldModify){
  string toReturn;
  if(!doesElementExist(toParse, start)) return ELEMENT_DOES_NOT_EXIST;
  int startLoc = toParse->find(start);
  int endLoc = toParse->find(delim, startLoc + 1);
  if(endLoc == string::npos){
    toReturn = toParse->substr(startLoc);
    if(shouldModify) *toParse = toParse->substr(0,startLoc);
  }
  else{
    toReturn = toParse->substr(startLoc, endLoc);
    if(shouldModify){
      string tempHold;
      tempHold = toParse->substr(0, startLoc);
      if(endLoc != toParse->length()) tempHold += toParse->substr(endLoc + 1);
      *toParse = tempHold;
     }
  }
  return toReturn;
}

float stringParser::getElementValue(string *toParse, char start, char delim, bool shouldModify){
  if(!doesElementExist(toParse, start)) return ELEMENT_DOES_NOT_EXIST_FLOAT_VALUE;
  string element = getElement(toParse, start, delim, shouldModify);
  float value = atof(element.substr(1).c_str()); //remove start char
  return value;
}

vector<string> stringParser::getAllElements(string toParse, char delim){
  bool isDone = false;
  vector <string> toReturn;
  while(!isDone){
    int next = toParse.find(delim);
    if(next == string::npos){
      if(toParse != "")
       toReturn.emplace_back(toParse);
      isDone = false;
      break;
     }
    toReturn.emplace_back(toParse.substr(0,next));
    toParse = toParse.substr(next + 1);
  }
  return toReturn;
}
