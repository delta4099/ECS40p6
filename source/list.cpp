// Author: Sean Davis

#include "list.h"
#include "directory.h"
#include <iostream>


ListNode::ListNode(Directory *director, ListNode *nex) : directory(director),
  next(nex)
{
}   // ListNode()


ListNode::~ListNode()
{
  delete directory;
}  // ~ListNode()


List::List() : head(NULL)
{
} // List()


List::List(const List &rhs)
{
  ListNode *rhsPtr, *prev = NULL;
  
  for (rhsPtr = rhs.head; rhsPtr; rhsPtr = rhsPtr->next)
  {
    Directory *directory = new Directory(*rhsPtr->directory);
    
    if (!prev)
      prev = head = new ListNode(directory, NULL);
    else  // inserting after first node
      prev = prev->next = new ListNode(directory, NULL);
  }  // for each node in list
}  // List copy constructor


List::~List() 
{
  for (ListNode *ptr = head; ptr; ptr = head)
  {
    head = ptr->next;
    delete ptr;
  }  // for each node in the list
} // ~List()


Directory* List::operator[] (int index)
{
  ListNode *ptr = head;
  
  for (int i = 0; ptr && i < index; i++, ptr = ptr->next);
    
  if (ptr)
    return ptr->directory;
  else  // ptr is NULL so index is beyond the end of the list
    return NULL;
}   // operator[]


const Directory* List::operator[] (int index) const
{
  ListNode *ptr = head;
  
  for (int i = 0; ptr && i < index; i++, ptr = ptr->next);
    
  if (ptr)
    return ptr->directory;
  else  // ptr is NULL so index is beyond the end of the list
    return NULL;
}   // operator[] const]


 List& List::operator+= (Directory *directory)
 {
   ListNode *ptr, *prev = NULL;
   
   for (ptr = head; ptr && *ptr->directory < *directory; ptr = ptr->next)
     prev = ptr;
   
    if (!prev)
     head = new ListNode(directory, head);
   else  // if inserting in middle of list.
     prev->next = new ListNode(directory, ptr);

   return *this;
 }  // operator+=
 
 
  
