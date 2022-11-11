#include "engine/random.h"

#include <cassert>


Random::Random(std::uint64_t seed) :
	m_state {seed}
{
	assert(seed != 0);
}


std::uint64_t Random::get() {
	auto x = m_state;

	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;

	m_state = x;

	return x * 0x2545f4914f6cdd1d;
}
