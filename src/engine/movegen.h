#ifndef MOVEGEN_H
#define MOVEGEN_H


#include "engine/bitboard.h"


int generateMoves(const Bitboard &board, bool is_whites_turn, Bitboard *next_positions);


#endif // MOVEGEN_H
