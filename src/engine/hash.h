#ifndef HASH_H
#define HASH_H


class Bitboard;
class BitstringList;

#include <cstdint>


using hash_t = std::uint64_t;


hash_t getHash(const Bitboard &bitboard, bool is_whites_turn, const BitstringList &bitstring_list);


#endif
