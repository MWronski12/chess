#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

/* -------------------------------- TYPEDEFS -------------------------------- */

using SquareIndex = uint8_t;

/* ---------------------------------- ENUMS --------------------------------- */

enum PieceColor { WHITE, BLACK };

enum PieceType {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
};

enum GameResult { WHITE_WON, BLACK_WON, DRAW, IN_PROGRESS };

/* -------------------------------- CONSTANTS ------------------------------- */

auto const PAWN_VALUE = 100;
auto const KNIGHT_VALUE = 300;
auto const BISHOP_VALUE = 325;
auto const ROOK_VALUE = 500;
auto const QUEEN_VALUE = 900;
auto const KING_VALUE = 32767;

auto const PAWN_ACTION_VALUE = 6;
auto const KNIGHT_ACTION_VALUE = 3;
auto const BISHOP_ACTION_VALUE = 3;
auto const ROOK_ACTION_VALUE = 2;
auto const QUEEN_ACTION_VALUE = 1;
auto const KING_ACTION_VALUE = 1;

auto const NULL_SQUARE = 64;

auto const CAPTURE_MOVE_REWARD = 2;

/* --------------------------- BOARD POSITION MAPS -------------------------- */

const PieceType STARTING_POSITION[64] = {
    ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK,  PAWN,  PAWN,   PAWN,   PAWN,  PAWN,
    PAWN,  PAWN,   PAWN,   EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY,
    EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY,
    EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, PAWN,   PAWN,   PAWN,  PAWN,
    PAWN,  PAWN,   PAWN,   PAWN,  ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK };

/* ---------------------- SQUARE PIECE EVALUATION MAPS ---------------------- */

const int8_t PAWN_TABLE[64] = {
    0,  0,   0,  0, 0,  0,  0,  0,   50,  50, 50, 50, 50, 50, 50, 50, 10, 10, 20, 30, 30,  20,
    10, 10,  5,  5, 10, 27, 27, 10,  5,   5,  0,  0,  0,  25, 25, 0,  0,  0,  5,  -5, -10, 0,
    0,  -10, -5, 5, 5,  10, 10, -25, -25, 10, 10, 5,  0,  0,  0,  0,  0,  0,  0,  0,
};

const int8_t KNIGHT_TABLE[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,   0,   -20, -40, -30, 0,   10,  15,  15, 10,
    0,   -30, -30, 5,   15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,   -30, -30, 5,   10, 15,
    15,  10,  5,   -30, -40, -20, 0,   5,   5,   0,   -20, -40, -50, -40, -20, -30, -30, -20, -40, -50,
};

const int8_t BISHOP_TABLE[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,  0,   0,   -10, -10, 0,   5,   10,  10, 5,
    0,   -10, -10, 5,   5,   10,  10,  5,   5,   -10, -10, 0,   10, 10,  10,  10,  0,   -10, -10, 10,  10, 10,
    10,  10,  10,  -10, -10, 5,   0,   0,   0,   0,   5,   -10, 20, -10, -40, -10, -10, -40, -10, -20,
};

const int8_t KING_TABLE[64] = {
    -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40,
    -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30, -20, -10, -20, -20, -20,
    -20, -20, -20, -10, 20,  20,  0,   0,   0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20,
};

const int8_t KING_END_GAME_TABLE[64] = {
    -50, -40, -30, -20, -20, -30, -40, -50, -30, -20, -10, 0,   0,   -10, -20, -30, -30, -10, 20,  30,  30, 20,
    -10, -30, -30, -10, 30,  40,  40,  30,  -10, -30, -30, -10, 30,  40,  40,  30,  -10, -30, -30, -10, 20, 30,
    30,  20,  -10, -30, -30, -30, 0,   0,   0,   0,   -30, -30, -50, -30, -30, -30, -30, -30, -30, -50 };

#endif