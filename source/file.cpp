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
  
}  // File destructor

void File::updateTime()
{
    time.update(); 
}  // updateTime()

void File::printTime() const 
{
    time.print(); 
}  // printTime()

Permissions* File::getPermissions() const
{
  return new Permissions(permissions);
  // return &permissions;  
}  // getPermissions()

void File::touch(File &rhs)
{
  rhs.updateTime();
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
  else  // if not long format
    cout << name; 
}  // ls()

void File::setPermissions(short newP, const char *user)
{
  permissions.chmod(newP, user);
}  // setPermissions()


bool File::operator== (const File &rhs) const
{
  return strcmp(name, rhs.name) == 0;
}  // operator==


bool File::operator< (const File &rhs) const
{
  return strcmp(name, rhs.name) < 0;
}  // operator<

ostream& File::write(ostream &os) const 
{
  os << "F ";
  os << name << ' ' << time
    << ' ' << permissions << endl;
  return os; 
}  // write()

ostream& operator<< (ostream &os, File const &rhs)
{
  rhs.write(os); 
  // os << rhs.name << ' ' << rhs.time << ' ' <<  rhs.subDirectoryCount 
  //   << ' ' << rhs.permissions << endl;
  
  return os;
}  // operator<<


istream& operator>> (istream &is, File &rhs)
{
  char temp;
  is >> temp; 
  //if ( )
//   is >> rhs.name >> rhs.time >> rhs.subDirectoryCount >> rhs.permissions; 
//   is.ignore(10, '\n');
  
//   for (int i = 0; i < rhs.subDirectoryCount; i++)
//   {
//     Directory *subDirectory = new Directory("Dummy", &rhs);
//     is >> *subDirectory;
//     rhs.subDirectories += subDirectory;
//   }  // for each subdirectory
  
  return is;
}  // operator>>
