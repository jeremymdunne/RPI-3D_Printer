#ifndef _CONFIGPARSER_H
#define _CONFIGPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std; 

class ConfigParser{
  public:
    ConfigParser(string file);
    void getValue(string what, string *name);
    void getValue(string what, int *value);
    void getValue(string what, float *value);
    void getValue(string what, bool *value);

  private:
    vector <string> lines;
    float parseForFloat(string value);
    string lineBeforEquals(string line);
    string lineAfterEquals(string line);
    string parseForString(string value);
    int parseForInt(string value);
};

#endif
