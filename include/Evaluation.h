#include "Board.h"

class Evaluation {
public:
    Evaluation() = delete;

    static int evaluate( const Board &board );
};