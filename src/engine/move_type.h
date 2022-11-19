#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H


#include <cstdint>


// move format: (from right to left)
// 5 bits: starting position (0-31)
// 4 bits: number of jumps (0-9)
// 2 bits: direction of first part of move
// 2 bits: direction of second part of move [optional - only applies if double jump]
// 2 bits ... [for triple jumps and above]
// maximum move length = 5 + 4 + (2 * 9) = 27 (fits within uin32_t)

using move_t = uint32_t;

constexpr int MOVE_TYPE_START_POS_SHIFT = 0;
constexpr int MOVE_TYPE_NUM_JUMPS_SHIFT = 5;
constexpr int MOVE_TYPE_DIRECTION_LIST_SHIFT = 9;

constexpr int MOVE_TYPE_START_POS_WIDTH = 5;
constexpr int MOVE_TYPE_NUM_JUMPS_WIDTH = 4;
constexpr int MOVE_TYPE_DIRECTION_LIST_ITEM_WIDTH = 2;


#endif // MOVE_TYPE_H
