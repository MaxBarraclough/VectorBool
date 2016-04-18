// Inspired by https://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=98
// > If you specify a different type of allocator, the compiler generates the
// > vector<bool> specialization from the primary vector template

// See also http://www.cplusplus.com/reference/vector/vector-bool/
// and      http://isocpp.org/blog/2012/11/on-vectorbool

// Let's see if we can 'trick' it with an allocator
// which simply 'forwards' to the default allocator

// This file should compile on g++ 4.8.4 on GNU/Linux
// It also uses Boost for BOOST_STRONG_TYPEDEF
// and for BOOST_STATIC_ASSERT

#include <boost/static_assert.hpp>
#include <boost/serialization/strong_typedef.hpp>

#include <vector>
#include <type_traits>


std::allocator<bool> instance; // allocator instantiates ok

BOOST_STRONG_TYPEDEF(int,   MyIntType)    // basic use is ok
BOOST_STRONG_TYPEDEF(float, MyFloatType)  // ok
// BOOST_STRONG_TYPEDEF(std::allocator<bool>, MyAllocator)
// doesn't work, because operator< is not defined

// Instead we can use inheritance to simulate a strong typedef:
class MyAllocator : public std::allocator<bool> { };
// identical to:
// class MyAllocator : public std::vector<bool>::allocator_type { };
 

 
BOOST_STATIC_ASSERT(( ! std::is_same<   std::allocator<bool> , MyAllocator   >::value  ));
// despite the fact that the types do not match, it fails to prevent bool specialization

BOOST_STATIC_ASSERT(( ! std::is_same<
          std::vector<bool,MyAllocator> ,
          std::vector<bool>             > ::value));
// The resulting vector classes are not the same type,
// but as we see below we have not prevented bool specialization

int main() {
  std::vector<int> intvec;
  int& intref = intvec.front();         // this is fine
  const int& intref2 = intvec.front();  // this is fine

  std::vector< bool, MyAllocator > badvec;

  badvec.flip(); // Only defined if bool specialization has been triggered
                 // (and it *is* defined here)

  badvec.push_back(true);            // this is fine, of course

// bool& ref1 = badvec.front();
// error, because second template argument hasn't prevented specialisation

  const bool& ref2 = badvec.front(); // ok
  // bool&& ref3 = badvec.front();   // ok in Clang++ 3.4 with -Wc++11-extensions

  return 0;
}

