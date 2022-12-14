#ifndef BITBOARD_MOVEGEN_H
#define BITBOARD_MOVEGEN_H


struct Bitboard;
class CompactMove;


constexpr int MAX_MOVES = 49;


int generateMoves(const Bitboard &board, bool is_whites_turn, Bitboard *next_positions, CompactMove *moves);


#endif // BITBOARD_MOVEGEN_H
