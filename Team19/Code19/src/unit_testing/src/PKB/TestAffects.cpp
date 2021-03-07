#include "PKB/Affects.h"

#include "catch.hpp"
using namespace std;

//    procedure Second {
//    1.    x = 0;
//    2.    i = 5;
//    3.    while(i != 0) {
//    4.      x = x + 2 * y;
//    5.      call third;
//    6.      i = i - 1; }
//    7.    if (x == 1) then {
//    8.      x = x + 1;
//          } else {
//    9.      z = 1; }
//    10.   z = z + x + i;
//    11.   y = z + 2;
//    12.   x = x * y + z;}

//    procedure Third {
//    13.   z = 5;
//    14.   v = z;
//    15.   print v;}

//    Affects(1, 4), Affects(1, 8), Affects(1,10), Affects(1,12)
//    Affects(2, 6), Affects(2,10)
//    Affects(4, 8), Affects(4, 10), Affects(4,12), Affects(4,4)
//    Affects(6, 10), Affects(6,6)
//    Affects(8, 10), Affects(8, 12)
//    Affects(9, 10)
//    Affects(10, 11), Affects(10, 12)
//    Affects(11, 12)
//    Affects(13, 14)

//    Affects*(1,4), Affects*(1,8), Affects*(1,10), Affects*(1,12), Affects*(1, 11)
//    Affects*(2,6), Affects*(2,10), Affects*(2,11), Affects*(2,12)
//    Affects*(4,8), Affects*(4,10), Affects*(4,12), Affects*(4,4), Affects*(4,11)
//    Affects*(6, 10), Affects*(6,6), Affects*(6,11), Affects*(6, 12)
//    Affects*(8, 10), Affects*(8,12), Affects*(8, 11)
//    Affects*(9, 10), Affects*(9, 11), Affects*(9, 12)
//    Affects*(10, 11), Affects*(10, 12)
//    Affects*(11,12)
//    Affects*(13,14)