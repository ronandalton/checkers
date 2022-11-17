#ifndef BITBOARD_H
#define BITBOARD_H


#include <cstdint>


using u32 = std::uint32_t;


struct Bitboard {
	u32 black_pieces;
	u32 white_pieces;
	u32 king_pieces;
};


#endif
