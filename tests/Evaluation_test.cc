#include <iostream>

#include "Evaluation.h"
#include "catch2/catch_test_macros.hpp"

/* ------------------------------ Constructors ------------------------------ */

TEST_CASE( "Evaluation of the board in initial position is 0", "[Evaluation.evaluate]" ) {
    Board b;
    std::cout << Evaluation::evaluate( b );
}
