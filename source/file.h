#ifndef FILE_H
  #define FILE_H

#include <iostream>
#include "permissions.h"
#include "Time.h"

using namespace std;

class Directory;
class File 
{
  Time time;
protected: 
  Permissions permissions;
  char *name;
  void updateTime(); 
  void printTime() const;
  friend class Directory;
public: 
  File(); 
  File(const File &rhs); 
  File(const char *nam, Permissions perm);
  virtual ~File(); 
  Permissions *getPermissions() const; 
  void touch(File &rhs); 
  void read(); 
  virtual ostream& write(ostream &os) const; 
  void ls(bool isLongFormat) const; 
  bool operator== (const File &rhs) const;
  bool operator< (const File &rhs) const;
  friend ostream& operator<< (ostream &os, File const &rhs);
  friend istream& operator>> (istream &is, File &rhs);
};
#endif // File_H 
