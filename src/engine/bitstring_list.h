#ifndef BITSTRING_LIST_H
#define BITSTRING_LIST_H


#include "engine/hash.h"

#include <array>


class BitstringList {
public:
	BitstringList();
	hash_t getBitstring(int index) const;
	hash_t getBitstringForPieceSquare(int position, int piece) const;
	hash_t getBitstringForWhitesTurn() const;

private:
	std::array<hash_t, 129> m_bitstrings; // 32 squares * 4 piece types + 1 for whites turn = 129
};


#endif
