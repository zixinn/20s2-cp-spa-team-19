#include "PKB/Next.h"

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

// Next(1, 2), Next(2, 3), Next(3, 4), Next(4, 5), Next(5, 6), Next(6, 3), Next(3, 7),
// Next(7, 8), Next(7, 9), Next(8, 10), Next(9, 10), Next(10, 11), Next(11, 12)

// Next*(1, 2),
// Next*(2, 3), Next*(1, 3)
// Next*(3, 4), Next*(2, 4), Next*(1, 4)
// Next*(4, 5), Next*(3, 5), Next*(2, 5), Next*(1, 5)
// Next*(5, 6), Next*(4, 6), Next*(3, 6), Next*(2, 6), Next*(1, 6)
// Next*(6, 3), Next*(5, 3), Next*(4, 3), Next*(3, 3)
// Next*(6, 4), Next*(5, 4), Next*(4, 4)
// Next*(6, 5), Next*(5, 5)
// Next*(6, 6)
// Next*(3, 7), Next*(6, 7), Next*(5, 7), Next*(4, 7), Next*(2, 7), Next*(1, 7)
// Next*(7, 8), Next*(3, 8), Next*(6, 8), Next*(5, 8), Next*(4, 8), Next*(2, 8), Next*(1, 8)
// Next*(7, 9), Next*(3, 9), Next*(6, 9), Next*(5, 9), Next*(4, 9), Next*(2, 9), Next*(1, 9)
// Next*(8, 10), Next*(7, 10), Next*(3, 10), Next*(6, 10), Next*(5, 10), Next*(4, 10), Next*(2, 10), Next*(1, 10)
// Next*(9, 10),
// Next*(10, 11), Next*(8, 11), Next*(9, 11), Next*(7, 11), Next*(3, 11), Next*(6, 11), Next*(5, 11), Next*(4, 11), Next*(2, 11), Next*(1, 11)
// Next*(11, 12), Next*(10, 12), Next*(8, 12), Next*(9, 12), Next*(7, 12), Next*(3, 12), Next*(6, 12), Next*(5, 12), Next*(4, 12), Next*(2, 12), Next*(1, 12)