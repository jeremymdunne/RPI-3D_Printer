#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

#define START_CHARACTER_NOT_FOUND "START_NPOS"
#define ELEMENT_DOES_NOT_EXIST "DNE"
#define ELEMENT_DOES_NOT_EXIST_FLOAT_VALUE -123.456F


using namespace std;

namespace stringParser{
string getNextElement(string *toParse, char delim, bool shouldModify);
bool doesElementExist(string *toParse, char start);
string getElement(string *toParse, char start, char delim, bool shouldModify);
float getElementValue(string *toParse, char start, char delim, bool shouldModify);
vector<string> getAllElements(string toParse, char delim);
}
