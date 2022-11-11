#ifndef RANDOM_H
#define RANDOM_H


#include <cstdint>


// generates pseudo-random numbers using xorshift* algorithm
class Random {
public:
	Random(std::uint64_t seed);
	std::uint64_t get();

private:
	std::uint64_t m_state;
};

#endif
