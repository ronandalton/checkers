#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H


#include <array>
#include <cstdint>


constexpr int MAX_BOARD_ALTERATIONS_PER_MOVE = 11;


using move_t = std::array<std::uint8_t, MAX_BOARD_ALTERATIONS_PER_MOVE>;


#endif
