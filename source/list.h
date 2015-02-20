#ifndef LIST_H
#define	LIST_H

class Directory;
class List;

class ListNode
{
  Directory *directory;
  ListNode *next;
  ListNode(Directory *director, ListNode *nex);
  ~ListNode();
  friend class List;
};  // class ListNode


class List
{
  ListNode *head;
public:
  List();
  List(const List &rhs);
  ~List();
  Directory* operator[] (int index);
  const Directory* operator[] (int index) const;
  List& operator+= (Directory *directory);
};  // class List

#endif	// LIST_H 

