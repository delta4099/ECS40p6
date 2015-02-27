// Author: Sean Davis
#include <cstring>
#include <iostream>
#include "directory.h"
#include "permissions.h"
#include "list.h"

using namespace std;

Directory::Directory(const char *nam, Directory *paren, const char *owner)
  : File(nam, Permissions(0777, owner)), subDirectoryCount(0), parent(paren) 
 {
  
 }  // Directory())


Directory::Directory(const Directory &rhs) : File(rhs), 
  subDirectoryCount(rhs.subDirectoryCount),  
  parent(rhs.parent) 
{
  for (int i = 0; i < rhs.subDirectoryCount; i++)
  {
    const Directory *directory = 
      dynamic_cast<const Directory*>(rhs.subDirectories[i]); 
    
    if (directory) 
    {
      Directory *  d1 = new Directory(*directory);
      subDirectories += d1;
      d1 -> parent = this; 
    }  // if directory
    else  // if it is a file
    {
      File * d1 = new File(rhs.subDirectories[i]->name
        , *(rhs.subDirectories[i]->getPermissions())); 
      subDirectories += d1;
    }  // makes a new file
  }  // for every subDirectory
}  // Directory copy constructor


Directory* Directory::cd(int argCount, const char *arguments[], 
                         const char *user)
{
  if (argCount != 2)
  {
    cout << "usage: cd directoryName\n";
    return this;
  }  // if two many arguments

  if (strcmp(arguments[1], "..") == 0)
  {
    if (parent)
      return parent;
    else  // this is root
      return this;
  }  // if cd ..

  for (int i = 0; i < subDirectoryCount; i++)
  {
    if (*subDirectories[i] == Directory(arguments[1]))
    {
      Directory *directory = dynamic_cast<Directory*> (subDirectories[i]); 
      
      if (!directory)
      {
        cout << arguments[1] << ": Not a directory.\n"; 
        return this; 
      }  // test if it's a directory 
      
      if (subDirectories[i]->getPermissions()->isPermitted(EXECUTE_PERM, 
          user))
        return directory;
      else  // subdirectory doesn't have execute permissions
      {
        cout << arguments[1] << ": Permission denied.\n";
        return this;
      }   // else no execute permissions for the specified directory
    }  // if found the named directory 
  }  // for each subdirectory
 
  cout << "cd: " << arguments[1] << ": No such file or directory\n";
  return this;
}  // cd()


bool Directory:: iscpCorrectFormat(int argCount, const char *arguments[])
{
  if (argCount != 3)
  {
    if (argCount == 1)
      cout << "cp: missing operand\n";
    else   // more than one argument, but not 3
      if (argCount == 2)
        cout << "cp: missing destination file operand after ‘" << arguments[1]
             << "’\n";
      else  // more than 3 arguments
        cout << "Too many operands\n";

    cout << "Try 'cp --help' for more information.\n";
    return false;
  }  // if incorrect number of operands

  if (strcmp(arguments[1], arguments[2]) == 0)
  {
    cout << "cp: ‘" << arguments[1] << "’ and ‘" << arguments[2]
      << "’ are the same file\n";
    return false;
  }  // if source and destination have the same name

  for (int i = 0; i < subDirectoryCount; i++)
    if (*subDirectories[i] == Directory(arguments[2]))
    {
      cout << "cp: '" << arguments[2] << "' already exists.\n";
      return false;
    }  // if the destination directory already exists

  return true;
}  //  iscpCorrectFormat()


void Directory::chmod(int argCount, const char *arguments[], const char *user)
// changes the permissions of the arguments
{
  if (argCount < 3)
  {
    if (argCount == 1)
      cout << "chmod: missing operand\n";
    else   // argCount == 2
      cout << "chmod: missing operand after ‘" << arguments[1] << "’\n";

    cout << "Try 'chmod --help' for more information.\n";
    return;
  }   // if wrong number of arguments
  
  short newPermissions = Permissions::checkOctals(arguments[1]);
  int j;
  
  if (newPermissions < 0)
  {
    cout << "chmod: invalid mode: ‘" << arguments[1] << "’\n";
    cout << "Try 'chmod --help' for more information.\n";
    return;
  }  // if invalid permissions
  
  for (int i = 2; i < argCount; i++)
  {
    for (j = 0; j < subDirectoryCount; j++)
      if (Directory(arguments[i]) == *subDirectories[j])
      {
        subDirectories[j]->setPermissions(newPermissions, user, true);
        break;
      }  // if matched name of directory with argument
    
    if (j == subDirectoryCount)
      cout << "chmod: cannot access ‘" << arguments[i] 
          << "’: No such file or directory\n";
  }  // for each filename
}  // chmod()


void Directory::chown(int argCount, const char *arguments[])
{
  int j;
  
  if (argCount != 3)
  {
    if (argCount == 1)
      cout << "chown: missing operand\n";
    else  // 2 or more than 3 arguments
      if (argCount == 2)
        cout << "chown: missing operand after ‘" << arguments[1] << "’\n";
      else  // more than 3 arguments
        cout << "chown: Too many arguments.\n";

    cout << "Try 'chown --help' for more information.\n";
    return;
  }  // if wrong number of arguments
  
  for (j = 0; j < subDirectoryCount; j++)
    if (Directory(arguments[2]) == *subDirectories[j])
      {
        subDirectories[j]->setPermissions(0755, arguments[1], false);
        break;
      }  // if matched name of directory with argument
    
  if (j == subDirectoryCount)
    cout << "chown: cannot access ‘" << arguments[2] 
        << "’: No such file or directory\n";
}  // chown()


void Directory::cp(int argCount, const char *arguments[], const char *user)
{
  if (!  iscpCorrectFormat(argCount, arguments))
    return;

  for (int i = 0; i < subDirectoryCount; i++)
    if (*subDirectories[i] == Directory(arguments[1]))
    {
      if (!subDirectories[i]->getPermissions()->isPermitted(READ_PERM, user ))
      {
        cout << "cp: cannot open ‘" << arguments[1] 
             << "’ for reading: Permission denied\n";
        return;
      }  // if not allowed to read
      
      Directory *directory = dynamic_cast<Directory*> (subDirectories[i]); 
      
      if (!directory)
      {
        File *destinationFile = new File(arguments[2]
          , *(subDirectories[i]->getPermissions())); 
        subDirectories += destinationFile;
      }  // test if it's a File
      else // if Directory
      {
        Directory *destinationDirectory = new Directory(*directory);
        delete [] destinationDirectory->name;
        destinationDirectory->name = new char[strlen(arguments[2]) + 1];
        strcpy(destinationDirectory->name, arguments[2]);
        subDirectories += destinationDirectory;
      }  // makes a directory 
      
      subDirectoryCount++;  
      return; 
    }  // if found source subdirectory
  
  cout << "cp: cannot stat ‘" << arguments[1] << "’: No such file or "
       <<  "directory" << "\nTry 'cp --help' for more information.\n";
}  // cp())

void Directory::ls(int argCount, const char *arguments[], const char *user)const
{
  if (argCount > 2 || (argCount == 2 && strcmp(arguments[1], "-l") != 0))
    cout << "usage: ls [-l]\n";
  else  // correct number of arguments
  {
    if (getPermissions()->isPermitted(READ_PERM, user))
    {
      if (argCount == 1)  // simple ls
      {
        for (int i = 0; i < subDirectoryCount; i++)
        {
          subDirectories[i]->ls(false); 
          cout << " "; 
        }  // for every subDirectory
        
        cout << "\n";
      }  // if simple ls
      else  // must be ls -l
      {
        for (int i = 0; i < subDirectoryCount; i++)
        {
          subDirectories[i]->ls(true); 
        }  // for each subdirectory
      }  // else is ls -l
    }  // if have read permissions
    else // don't have read permissions
      cout << "ls: cannot open directory .: Permission denied\n";
  }  // else correct arguments
}  // ls()


void Directory::mkdir(int argCount, const char *arguments[], const char *user)
{
  int i;
  
  if (argCount == 1)
  {
    cout << "mkdir: missing operand\n";
    cout << "Try 'mkdir --help' for more information.\n";
    return;
  }  // if too many arguments
   
  for (int argPos = 1; argPos < argCount; argPos++)
  {
    for (i = 0; i < subDirectoryCount; i++)
      if (*subDirectories[i] == Directory(arguments[argPos]))
      {
        cout << "mkdir: cannot create directory ‘" << arguments[argPos] 
             << "’: File exists\n"; 
        break;
      }  // if subdirectory already exists.
    
    if (i == subDirectoryCount)
    {
      if (getPermissions()->isPermitted(WRITE_PERM, user))
      {
        subDirectories += new Directory(arguments[argPos], this, user);
        updateTime();
        subDirectoryCount++;
      }  // if have write permissions
      else // no write permissions
        cout << "mkdir: cannot create directory ‘" << arguments[argPos] 
             <<  "’: Permission denied\n";
    }  // if name not found
  }  // for each filename
}  // mkdir()


void Directory::touch(int argCount, const char *arguments[], const char *user)
{
  int i;
  
  if (argCount == 1)
  {
    cout << "touch: missing operand\n";
    cout << "Try 'touch --help' for more information.\n";
    return;
  }  // if too many arguments
   
  for (int argPos = 1; argPos < argCount; argPos++)
  {
    for (i = 0; i < subDirectoryCount; i++)
      if (*subDirectories[i] == Directory(arguments[argPos]))
      {
        File::touch(*subDirectories[i]);
        break;
      }  // if subdirectory already exists.
    
    if (i == subDirectoryCount)
    {
      if (getPermissions()->isPermitted(WRITE_PERM, user))
      {
        subDirectories += new File(arguments[argPos], Permissions(0666, user));
        updateTime();
        subDirectoryCount++;
      }  // if have write permissions
      else // no write permissions
        cout << "touch: cannot create file ‘" << arguments[argPos] 
             <<  "’: Permission denied\n";
    }  // if name not found
  }  // for each filename
}  // touch()


void Directory::showPath() const
{
  if (parent == NULL)
  {
    cout << "/";
    return;
  }  // at root
  
  parent->showPath();
  cout << name << "/";
}  // showPath())

ostream& Directory::write(ostream &os) const
{
  os << "D"; 
  File::write(os);
  os << subDirectoryCount << endl; 
  
  for (int i = 0; i < subDirectoryCount; i++)
    os << *subDirectories[i];
  return os; 
}  // write()
