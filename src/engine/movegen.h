#ifndef MOVEGEN_H
#define MOVEGEN_H


#include "engine/move_type.h"
#include "engine/bitboard.h"


int generateMoves(const Bitboard &board, bool is_whites_turn, Bitboard *next_positions, move_t *moves);


#endif // MOVEGEN_H
