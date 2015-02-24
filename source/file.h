#ifndef FILE_H
  #define FILE_H

#include <iostream>
#include "permissions.h"
#include "Time.h"

using namespace std; 

class File 
{
  Time time;
protected: 
  char *name;
  Permissions permissions;
  void updateTime(); 
public: 
  File(); 
  File( const File &rhs); 
  File(const char *nam, Permissions perm);
  bool operator== (const File &rhs) const;
  bool operator< (const File &rhs) const;
  Permissions getPermissions(); 
  void touch(); 
  void ls(bool isLongFormat) const; 
  
};
#endif // File_H 
