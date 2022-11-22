#ifndef MOVEGEN_H
#define MOVEGEN_H


struct Bitboard;
class Move;


int generateMoves(const Bitboard &board, bool is_whites_turn, Bitboard *next_positions, Move *moves);


#endif // MOVEGEN_H
