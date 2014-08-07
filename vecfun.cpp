#include <vector>

#include "stdio.h"

// Inspired by http://isocpp.org/blog/2012/11/on-vectorbool
// See also https://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=98

// I used clang++ 3.4 and g++ 4.8 on GNU/Linux
// The former can be persuaded to allow rvalue references with -Wc++11-extensions
// I don't know about gcc.


/*
template <typename T> void func(std::vector<T> & vec) {
  T & ref = vec.front();
}
*/

int main() {

  std::vector<int> intvec;
  int& intref = intvec.front(); // this is fine

  std::vector< bool, std::allocator<bool> > badvec;
  badvec.push_back(true);
  bool& ref1 = badvec.front(); // error! The second template argument doesn't seem to have precented specialisation!
  const bool& ref2 = badvec.front(); // ok
  // bool&& ref3 = badvec.front(); // ok (in Clang++ 3.4 with C++11 enabled)

//  func(badvec);

}

