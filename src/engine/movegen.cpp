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


// modifies board in place to apply the specified move
// piece_position should have a single bit set corresponding to which piece should be moved
// this function assumes we are given a valid move
// returns the new piece position
static u32 makeMove(Bitboard *board, u32 piece_position, int direction, bool is_jump) {
	const bool is_whites_turn = board->white_pieces & piece_position;
	const bool was_a_king_before_move = piece_position & board->king_pieces;
	const u32 my_crown_row = is_whites_turn ? white_crown_row : black_crown_row;
	u32 &my_pieces = is_whites_turn ? board->white_pieces : board->black_pieces;
	u32 &their_pieces = is_whites_turn ? board->black_pieces : board->white_pieces;
	u32 &king_pieces = board->king_pieces;

	// delete piece from starting position
	my_pieces &= ~piece_position;
	king_pieces &= ~piece_position;

	u32 adjacent_position = signedBitshift(piece_position,
		piece_position & even_row ? -even_shift[direction] : -odd_shift[direction]);

	u32 new_piece_position;
	if (!is_jump) {
		new_piece_position = adjacent_position;
	} else {
		new_piece_position = signedBitshift(piece_position, -jump_shift[direction]);

		// we also need to remove the piece that was jumped
		their_pieces &= ~adjacent_position;
		king_pieces &= ~adjacent_position;
	}
	my_pieces |= new_piece_position;

	// apply the correct crown state of the piece moved
	bool entered_crown_row = new_piece_position & my_crown_row;
	if (was_a_king_before_move || entered_crown_row) {
		king_pieces |= new_piece_position;
	} // assume no phantom king on empty square, so no need to clear bit if not king

	return new_piece_position;
}


// piece_position should have a single bit set corresponding to which piece we are testing
static bool jumpIsLegal(const Bitboard &board, u32 piece_position, int direction) {
	bool is_whites_turn = board.white_pieces & piece_position;
	bool is_a_king = board.king_pieces & piece_position;

	bool moving_forwards = (is_whites_turn == (direction < NUM_DIRECTIONS / 2));
	if (!is_a_king && !moving_forwards) {
		return false; // non kings cannot move backwards
	}

	if (!(piece_position & jump_mask[direction])) {
		return false; // jumping would put the piece out of bounds
	}

	u32 adjacent_position = signedBitshift(piece_position,
		piece_position & even_row ? -even_shift[direction] : -odd_shift[direction]);
	u32 their_pieces = is_whites_turn ? board.black_pieces : board.white_pieces;

	if (!(their_pieces & adjacent_position)) {
		return false; // one of their pieces isn't adjacent
	}

	u32 new_piece_position = signedBitshift(piece_position, -jump_shift[direction]);
	u32 empty_squares = ~(board.black_pieces | board.white_pieces);

	if (!(empty_squares & new_piece_position)) {
		return false; // the square we want to jump to isn't vacant
	}

	return true;
}


// populates the list pointed to by next_positions recursively
// returns number of moves found
static int findDoubleJumps(const Bitboard &board, const u32 piece_position, Bitboard *next_positions) {
	int moves_found = 0;

	// try jumping in each direction
	for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
		if (jumpIsLegal(board, piece_position, direction)) {
			Bitboard new_board = board;

			u32 new_piece_position = makeMove(&new_board, piece_position, direction, true);

			bool was_a_king_before_move = piece_position & board.king_pieces;
			bool is_a_king_now = new_piece_position & new_board.king_pieces;
			bool piece_was_crowned = !was_a_king_before_move && is_a_king_now;

			if (!piece_was_crowned) {
				moves_found += findDoubleJumps(new_board, new_piece_position, next_positions + moves_found);
			} else {
				next_positions[moves_found++] = new_board;
			}

		}
	}

	if (!moves_found) {
		next_positions[moves_found++] = board;
	}

	return moves_found;
}


// returns number of moves found
// next_positions is an out parameters pointing to an array to populate
// assumes array is large enough to hold result
int generateMoves(const Bitboard &board, bool is_whites_turn, Bitboard *next_positions) {
	// get piece types from perspective of player to move
	const u32 my_pieces = is_whites_turn ? board.white_pieces : board.black_pieces;
	const u32 their_pieces = is_whites_turn ? board.black_pieces : board.white_pieces;
	const u32 empty_squares = ~(my_pieces | their_pieces);
	
	u32 movables[NUM_DIRECTIONS];
	bool found_moves = false;
	bool is_jumping_move = false;

	for (int checking_for_jumps = 1; checking_for_jumps >= 0; checking_for_jumps--) {
		for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
			movables[direction] = my_pieces; // only my pieces can move
			if (is_whites_turn != (direction < NUM_DIRECTIONS / 2)) { // checking for moves in backwards direction
				movables[direction] &= board.king_pieces; // only kings can go this way
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
			const u32 piece_position = movable & (~movable + 1); // get least significant bit of movable

			Bitboard new_board = board;

			u32 new_piece_position = makeMove(&new_board, piece_position, direction, is_jumping_move);

			bool was_a_king_before_move = piece_position & board.king_pieces;
			bool is_a_king_now = new_piece_position & new_board.king_pieces;
			bool piece_was_crowned = !was_a_king_before_move && is_a_king_now;

			if (is_jumping_move && !piece_was_crowned) {
				moves_found += findDoubleJumps(new_board, new_piece_position, next_positions + moves_found);
			} else {
				next_positions[moves_found++] = new_board;
			}

			movable &= (movable - 1); // clear least significant bit of movable
		}
	}

	return moves_found;
}
