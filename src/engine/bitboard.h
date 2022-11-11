#ifndef BITBOARD_H
#define BITBOARD_H


#include <cstdint>


using U32 = std::uint32_t;


struct Bitboard {
	U32 black_pieces;
	U32 white_pieces;
	U32 king_pieces;
};


#endif
