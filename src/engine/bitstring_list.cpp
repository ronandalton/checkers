#include "engine/bitstring_list.h"

#include "engine/random.h"

#include <cassert>


BitstringList::BitstringList() {
	Random generator(1);

	for (auto &bitstring : m_bitstrings) {
		bitstring = generator.get();
	}
}


hash_t BitstringList::getBitstring(int index) const {
	assert(index >= 0 && index < static_cast<int>(m_bitstrings.size()));

	return m_bitstrings[index];
}


hash_t BitstringList::getBitstringForPieceSquare(int position, int piece) const {
	assert(position >= 0 && position < 32);
	assert(piece >= 0 && piece < 4);

	return m_bitstrings[position * 4 + piece];
}


hash_t BitstringList::getBitstringForWhitesTurn() const {
	return m_bitstrings.back();
}
