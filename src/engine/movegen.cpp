#include "movegen.h"

#include "bitboard.h"
#include "move_type.h"


// these masks stop moves near the edges of the board being made if
// the landing square of the move is outside the boards boundaries
static constexpr u32 move_mask[4] = { // where normal moves are allowed
	0b1110'1111'1110'1111'1110'1111'1110'0000, // up left
	0b1111'0111'1111'0111'1111'0111'1111'0000, // up right
	0b0000'0111'1111'0111'1111'0111'1111'0111, // down right
	0b0000'1111'1110'1111'1110'1111'1110'1111, // down left
};
static constexpr u32 jump_mask[4] = { // where jumping moves are allowed
	0b1110'1110'1110'1110'1110'1110'0000'0000, // up left
	0b0111'0111'0111'0111'0111'0111'0000'0000, // up right
	0b0000'0000'0111'0111'0111'0111'0111'0111, // down right
	0b0000'0000'1110'1110'1110'1110'1110'1110, // down left
};

// the left shift to get the adjacent square for each direction:
static constexpr int even_shift[4] = {4, 3, -5, -4}; // on even rows
static constexpr int odd_shift[4] = {5, 4, -4, -3}; // on odd rows

// the left shift to get the square beyond the adjacent square:
static constexpr int jump_shift[4] = {9, 7, -9, -7}; // on even and odd rows

// these masks are used because finding an adjacent square requires
// combining two different bit shifted boards by even and odd rows
static constexpr u32 even_row = 0b0000'1111'0000'1111'0000'1111'0000'1111;
static constexpr u32 odd_row = 0b1111'0000'1111'0000'1111'0000'1111'0000;

// masks for the crowning row of each side that makes a piece a king
static constexpr u32 black_crown_row = 0b1111'0000'0000'0000'0000'0000'0000'0000;
static constexpr u32 white_crown_row = 0b0000'0000'0000'0000'0000'0000'0000'1111;

// directions are: up left, up right, down right, down left
static constexpr int NUM_DIRECTIONS = 4;


// shifts left if shift value is positive, otherwise right
static uint32_t signedBitshift(uint32_t bits, int shift) {
	if (shift >= 0) {
		return bits << shift;
	} else {
		return bits >> -shift;
	}
}


// returns the least significant bit of bits
static uint32_t lsb(uint32_t bits) {
	return bits & -bits; // two's complement bit twiddling trick
} // TODO: remove if unused


// returns the position of the bit set in value
// eg. 00000100 -> 2
// input must have a single bit set
static uint32_t getBitPosition(uint32_t value) {
	uint32_t position = 0;
	while (value >>= 1) {
		position++;
	}
	return position;
}


static int findDoubleJumps(const Bitboard &board, const u32 current_position, move_t partial_move, Bitboard *next_positions, move_t *moves) {
	int current_num_jumps = (partial_move >> MOVE_TYPE_NUM_JUMPS_SHIFT) & ((1u << MOVE_TYPE_NUM_JUMPS_WIDTH) - 1);
	bool is_whites_turn = board.white_pieces & current_position;
	bool is_king = board.king_pieces & current_position;
	const u32 my_pieces = is_whites_turn ? board.white_pieces : board.black_pieces;
	const u32 their_pieces = is_whites_turn ? board.black_pieces : board.white_pieces;
	const u32 king_pieces = board.king_pieces;
	const u32 empty_squares = ~(board.black_pieces | board.white_pieces);
	const u32 my_crown_row = is_whites_turn ? white_crown_row : black_crown_row;

	// try jumping in each legal direction
	for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
		if (!is_king && (is_whites_turn != (direction < NUM_DIRECTIONS / 2))) {
			continue; // non kings cannot move backwards
		}

		if (!(current_position & jump_mask[direction])) {
			continue; // jumping would put the piece out of bounds
		}

		u32 jumped_piece_position = signedBitshift(current_position,
			current_position & even_row ? -even_shift[direction] : -odd_shift[direction]);

		if (!(their_pieces & jumped_piece_position)) {
			continue; // one of their pieces isn't adjacent
		}

		u32 new_piece_position = signedBitshift(current_position, -jump_shift[direction]);

		if (!(empty_squares & new_piece_position)) {
			continue; // the square we're jumping to isn't vacant
		}

		// a jump in this direction is legal

		move_t new_partial_move = partial_move;
		// add new direction to end of move directions list
		new_partial_move |= direction << (MOVE_TYPE_DIRECTION_LIST_SHIFT + current_num_jumps * MOVE_TYPE_DIRECTION_LIST_ITEM_WIDTH);
		 // increment num_jumps field in move
		new_partial_move += 1u << MOVE_TYPE_NUM_JUMPS_SHIFT;

		// create copy of board to operate on
		u32 my_new_pieces = my_pieces;
		u32 their_new_pieces = their_pieces;
		u32 new_king_pieces = king_pieces;

		// make the move
		my_new_pieces &= ~current_position; // delete piece from start position
		new_king_pieces &= ~current_position; // clear king state
		my_new_pieces |= new_piece_position;
		// make the new piece a king if it was before or it reached the crown row
		if ((current_position & king_pieces) || (new_piece_position & my_crown_row)) {
			new_king_pieces |= new_piece_position;
		} // assume no phantom king on empty square so no need to clear bit
		their_new_pieces &= ~jumped_piece_position; // remove jumped piece
		new_king_pieces &= ~jumped_piece_position; // clear king state

		// TODO: recursively find jump paths
	}
}


// returns number of moves found
// next_positions and moves are output parameters pointing to arrays
// either may be null if that type of output is not needed
// assumes output arrays pointed to are large enough to hold result
int generateMoves(const Bitboard &board, bool is_whites_turn, Bitboard *next_positions, move_t *moves) {
	// get piece types from perspective of player to move
	const u32 my_pieces = is_whites_turn ? board.white_pieces : board.black_pieces;
	const u32 their_pieces = is_whites_turn ? board.black_pieces : board.white_pieces;
	const u32 king_pieces = board.king_pieces;
	const u32 empty_squares = ~(my_pieces | their_pieces);
	const u32 my_crown_row = is_whites_turn ? white_crown_row : black_crown_row;
	
	u32 movables[NUM_DIRECTIONS];
	bool found_moves = false;
	bool is_jumping_move = false;

	for (int checking_for_jumps = 1; checking_for_jumps >= 0; checking_for_jumps--) {
		for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
			movables[direction] = my_pieces; // only my pieces can move
			if (is_whites_turn != (direction < NUM_DIRECTIONS / 2)) { // checking for moves in backwards direction
				movables[direction] &= king_pieces; // only kings can go this way
				if (!movables[direction]) {
					continue;
				}
			}
			if (checking_for_jumps) {
				movables[direction] &= jump_mask[direction]; // don't allow moving outside of board
				movables[direction] &= // their piece is adjacent
					(signedBitshift(their_pieces, even_shift[direction]) & even_row)
					| (signedBitshift(their_pieces, odd_shift[direction]) & odd_row);
				movables[direction] &= // square beyond is empty
					signedBitshift(empty_squares, jump_shift[direction]);
			} else { // not checking for jumps
				movables[direction] &= move_mask[direction]; // don't allow moving outside of board
				movables[direction] &= // adjacent square is empty
					(signedBitshift(empty_squares, even_shift[direction]) & even_row)
					| (signedBitshift(empty_squares, odd_shift[direction]) & odd_row);
			}
			if (movables[direction]) {
				found_moves = true;
			}
		}
		if (checking_for_jumps && found_moves) {
			is_jumping_move = true;
			break;
		}
	}

	int moves_found = 0;

	for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
		u32 movable = movables[direction];
		// process each movable piece in this direction
		while (movable) {
			const u32 piece = movable & (~movable + 1); // get least significant bit of movable

			if (is_jumping_move) {
				// check for double jump
				// TODO
			}

			// calculate board state after this move (if needed)
			if (next_positions != nullptr) {
				// create copy of board to operate on
				u32 my_new_pieces = my_pieces;
				u32 their_new_pieces = their_pieces;
				u32 new_king_pieces = king_pieces;

				// delete piece from starting position
				my_new_pieces &= ~piece;
				new_king_pieces &= ~piece;

				u32 adjacent_position = signedBitshift(piece,
					piece & even_row ? -even_shift[direction] : -odd_shift[direction]);
				u32 new_piece_position;
				if (is_jumping_move) {
					// remove jumped piece
					their_new_pieces &= ~adjacent_position;
					new_king_pieces &= ~adjacent_position;

					// add piece at landing position
					new_piece_position = signedBitshift(piece, -jump_shift[direction]);
					my_new_pieces |= new_piece_position;
				} else { // normal move
					new_piece_position = adjacent_position;
					my_new_pieces |= new_piece_position;
				}
				// make the new piece a king if it was before or if it reached the crown row
				if ((piece & king_pieces) || (new_piece_position & my_crown_row)) {
					new_king_pieces |= new_piece_position;
				} // assume no phantom king on empty square so no need to clear bit

				// store move
				Bitboard new_board;
				if (is_whites_turn) {
					new_board.black_pieces = their_new_pieces;
					new_board.white_pieces = my_new_pieces;
				} else {
					new_board.black_pieces = my_new_pieces;
					new_board.white_pieces = their_new_pieces;
				}
				new_board.king_pieces = new_king_pieces;

				next_positions[moves_found] = new_board;
			}

			// store data needed to execute this move (if needed)
			if (moves != nullptr) {
				if (is_jumping_move) {
					move_t move = 0;

					// TODO: consider multi-jumps
					move = getBitPosition(piece)
						| (1 << MOVE_TYPE_NUM_JUMPS_SHIFT)
						| (direction << MOVE_TYPE_DIRECTION_LIST_SHIFT);

					moves[moves_found] = move;
				} else { // normal move
					move_t move = getBitPosition(piece)
						// num_jumps set to zero implicitly
						| (direction << MOVE_TYPE_DIRECTION_LIST_SHIFT);

					moves[moves_found] = move;
				}
			}

			moves_found++;
			movable &= (movable - 1); // clear least significant bit of movable
		}
	}

	return moves_found;
}
