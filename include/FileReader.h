#ifndef _FileReader_CLASS
#define _FileReader_CLASS


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#define END_OF_FILE  "-1";
#define OPEN_FAILED false;

using namespace std;

class FileReader{
  public:
    FileReader(string fileLocation);
    string getLine();
    bool isDone();
    void close();

  private:
    ifstream file;
};

#endif
