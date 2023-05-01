#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.h"

class Evaluation {
public:
    Evaluation() = delete;

    static int evaluateBoard( const Board &board );
};

#endif