#include "engine/compact_move.h"

#include <cassert>
#include <algorithm> // for std::max


// move format: (from right to left)
// 1 bits: move exists flag, allows us to return no move found
// 5 bits: starting position (range 0-31)
// 4 bits: number of jumps, zero if normal move (range 0-9)
// 2 bits: direction of first part of move (range 0-3)
// 2 bits: direction of second part of move [optional - only applies if double jump]
// 2 bits ... [for triple jumps and above]
// maximum move length = 1 + 5 + 4 + (2 * 9) = 28 (fits within uin32_t)


CompactMove::CompactMove(int starting_position, bool is_jump, int direction) {
	assert(starting_position >= 0 && starting_position < 32);
	assert(direction >= 0 && direction < 4);

	m_data = 1 << EXISTS_SHIFT
		| starting_position << START_POS_SHIFT
		| static_cast<int>(is_jump) << NUM_JUMPS_SHIFT
		| direction << DIRECTION_LIST_SHIFT;
}


void CompactMove::addJumpDirection(int direction) {
	assert(exists());
	assert(direction >= 0 && direction < 4);
	assert(getNumberOfJumps() > 0 && getNumberOfJumps() < MAX_JUMPS);

	m_data |= direction << (DIRECTION_LIST_SHIFT + DIRECTION_LIST_ITEM_WIDTH * getNumberOfJumps());
	m_data += 1 << NUM_JUMPS_SHIFT;
}


bool CompactMove::exists() const {
	return getField(EXISTS_SHIFT, EXISTS_WIDTH);
}


int CompactMove::getStartingPosition() const {
	assert(exists());

	return getField(START_POS_SHIFT, START_POS_WIDTH);
}


int CompactMove::getNumberOfJumps() const {
	assert(exists());

	return getField(NUM_JUMPS_SHIFT, NUM_JUMPS_WIDTH);
}


int CompactMove::getDirection(int index) const {
	assert(exists());
	assert(index >= 0 && index < std::max(1, getNumberOfJumps()));

	return getField(DIRECTION_LIST_SHIFT + DIRECTION_LIST_ITEM_WIDTH * index, DIRECTION_LIST_ITEM_WIDTH);
}


bool CompactMove::isJump() const {
	assert(exists());

	return getNumberOfJumps() > 0;
}


int CompactMove::getField(int shift, int width) const {
	return (m_data >> shift) & ((1 << width) - 1);
}
