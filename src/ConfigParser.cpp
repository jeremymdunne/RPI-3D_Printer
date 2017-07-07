#include "ConfigParser.h"

ConfigParser::ConfigParser(string file){
ifstream f(file.c_str());
  while(!f.eof()){
    string s;
    getline(f, s);
    this->lines.push_back(s);
  }
}

string ConfigParser::lineBeforEquals(string line){
  int end = line.find('=');
  string var = line.substr(0,end -1 );
  return var;
}

string ConfigParser::lineAfterEquals(string line){
  int end = line.find('=');
  string var = line.substr(end + 1);
  return var;
}

string ConfigParser::parseForString(string value){
  int start = value.find('"') + 1;
  int end = value.find(';');
  string result = value.substr(start, end - 3);
  return result;
}

void ConfigParser::getValue(string what, string *name ){
  for(int i =0; i < this->lines.size(); i ++){
    if(lineBeforEquals(this->lines[i]) == what){
      string value = lineAfterEquals(this->lines[i]);
        *name = parseForString(value);
    }
  }
}

void ConfigParser::getValue(string what, bool *value){
  string stringValue;
  this->getValue(what, &stringValue);
  if(stringValue == "true") *value = true;
  else *value = false;
}

int ConfigParser::parseForInt(string value){
  int start =  1;
  int end = value.find(';');
  string result = value.substr(start, end - 1);
  return atoi(result.c_str());
}

float ConfigParser::parseForFloat(string value){
  int start =  1;
  int end = value.find(';');
  string result = value.substr(start, end - 1);
  return atof(result.c_str());
}

void ConfigParser::getValue(string what, int *value ){
  for(int i =0; i < this->lines.size(); i ++){
    if(lineBeforEquals(this->lines[i]) == what){
      string holdValue = lineAfterEquals(this->lines[i]);
        *value = parseForInt(holdValue);
    }
  }
}

void ConfigParser::getValue(string what, float *value){
  for(int i =0; i < this->lines.size(); i ++){
    if(lineBeforEquals(this->lines[i]) == what){
      string holdValue = lineAfterEquals(this->lines[i]);
        *value = parseForFloat(holdValue);
    }
  }
}
