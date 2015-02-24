#include <cstring>
#include <iostream>
#include "file.h"

using namespace std;

File::File()
{
    
}

File::File(const File &rhs) : permissions(rhs.permissions)
{
  name = new char[strlen(rhs.name) + 1];
  strcpy(name, rhs.name);
  
}

File::File(const char *nam, Permissions perm)
{
  name = new char[strlen(nam) + 1];
  strcpy(name, nam);
  permissions.set(0777, owner);
}

void File::updateTime()
{
    time.update(); 
}

Permissions File::getPermissions(); 
{
  return permissions;  
}

void File::touch();
{
    
}

void ls (bool isLongFormat) const
{
    
}

bool File::operator== (const File &rhs) const
{
  return strcmp(name, rhs.name) == 0;
}  // operator==


bool File::operator< (const File &rhs) const
{
  return strcmp(name, rhs.name) < 0;
}  // operator<
