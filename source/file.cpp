#include <cstring>
#include <iostream>
#include "file.h"

using namespace std;

File::File() 
{
    
}  // Default constructor

File::File(const File &rhs) : permissions(rhs.permissions)
{
  name = new char[strlen(rhs.name) + 1];
  strcpy(name, rhs.name);
  
}  // Copy constructor

File::File(const char *nam, Permissions perm) : permissions(perm)
{
  name = new char[strlen(nam) + 1];
  strcpy(name, nam); 
}  // Standard constructor

File::~File()
{
  
}

void File::updateTime()
{
    time.update(); 
}  // updateTime()

void File::printTime() const 
{
    time.print(); 
}  // printTime()

Permissions File::getPermissions() const
{
  return permissions;  
}  // getPermissions()

void File::touch()
{
    
}  // touch()

void File::ls(bool isLongFormat) const
{
  
  if (isLongFormat)
  {
    permissions.print(); 
    cout << ' ';
    time.print(); 
    cout << name << "\n"; 
  } // prints out stuff for ls -l 
  else 
  {
    cout << name; 
  }
}  // ls()

bool File::operator== (const File &rhs) const
{
  return strcmp(name, rhs.name) == 0;
}  // operator==


bool File::operator< (const File &rhs) const
{
  return strcmp(name, rhs.name) < 0;
}  // operator<
