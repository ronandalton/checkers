#ifndef COMPACT_MOVE_H
#define COMPACT_MOVE_H


#include <cstdint>


class CompactMove {
public:
	CompactMove(int starting_position, bool is_jump, int direction);
	CompactMove() = default; // creates a blank move (doesn't exist)

	void addJumpDirection(int direction);

	bool exists() const;
	int getStartingPosition() const;
	int getNumberOfJumps() const;
	int getDirection(int index) const;
	bool isJump() const;

	static constexpr int MAX_JUMPS = 9;

private:
	uint32_t m_data = 0;

	int getField(int shift, int width) const;

	static constexpr int EXISTS_SHIFT = 0;
	static constexpr int EXISTS_WIDTH = 1;
	static constexpr int START_POS_SHIFT = 1;
	static constexpr int START_POS_WIDTH = 5;
	static constexpr int NUM_JUMPS_SHIFT = 6;
	static constexpr int NUM_JUMPS_WIDTH = 4;
	static constexpr int DIRECTION_LIST_SHIFT = 10;
	static constexpr int DIRECTION_LIST_ITEM_WIDTH = 2;
};


#endif // COMPACT_MOVE_H
