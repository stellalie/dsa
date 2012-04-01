#include <iostream>
#include <list>
#include <ctype.h>

using namespace std;

void printList(const list<char> &myList);
void fillList(list<char> &myList, const string &vowels);
void changeCase(list <char> &myList);

void printList(const list<char> &myList)
{
   list<char>::const_iterator itr;
   
   for (itr = myList.begin(); itr != myList.end(); itr++ ) {
      cout << ' ' << *itr;
   }
   cout << '\n';
}

void fillList(list<char> &myList, const string &vowels)
{
   for (int i = 0; i<vowels.length(); i++) {
      myList.push_back(vowels[i]);
   }
}

void changeCase(list <char> &myList)
{
   list<char>::iterator itr;
   
   for (itr = myList.begin(); itr != myList.end(); itr++ ) {
      if (islower(*itr)) *itr = toupper(*itr);
      else *itr = toupper(*itr);
   }
}
   
int main ()
{
  string vowels = "aeiou";
  list<char> myList;
  
  fillList(myList, vowels);
  printList(myList);
  changeCase(myList);
  printList(myList);
  
  return 0;
}

