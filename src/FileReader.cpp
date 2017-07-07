#include "FileReader.h"


FileReader::FileReader(string fileLocation){
  this->file.open(fileLocation);
  if(this->file.is_open()) cout << "opened" << endl;
  else cout << "Not opened" << endl;
}

void FileReader::close(){
  this->file.close();
}

string FileReader::getLine(){
  string line;
  //cout << this->isDone() << endl;
  if(!this->isDone()) getline(this->file,line);
  else line = END_OF_FILE;
  return line;
}

bool FileReader::isDone(){
  return (file.peek() == EOF);
}
