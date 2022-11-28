#include "engine/engine.h"

#include "game/game.h"
#include "game/board.h"
#include "game/move.h"
#include "game/turn.h"
#include "game/piece.h"
#include "game/position.h"
#include "game/direction.h"
#include "engine/bitboard.h"
#include "engine/bitboard_movegen.h"
#include "engine/compact_move.h"

#include <algorithm> // for std::max
#include <climits>


static Bitboard convertBoardToBitboard(const Board &board) {
	Bitboard bitboard;

	bitboard.black_pieces = 0;
	bitboard.white_pieces = 0;
	bitboard.king_pieces = 0;

	for (int position = 0; position < 32; position++) {
		Piece piece = board.pieceAt(position);

		if (piece.exists()) {
			u32 mask = 1u << position;

			if (piece.belongsTo(Turn::BLACK)) {
				bitboard.black_pieces |= mask;
			} else {
				bitboard.white_pieces |= mask;
			}

			if (piece.isCrowned()) {
				bitboard.king_pieces |= mask;
			}
		}
	}

	return bitboard;
}


static Move convertCompactMovetoNormalMove(const CompactMove compact_move) {
	if (!compact_move.exists()) {
		return Move();
	}

	Move normal_move;

	Position position = compact_move.getStartingPosition();

	normal_move.addPosition(position);

	int num_hops = std::max(1, compact_move.getNumberOfJumps());
	bool is_jumping_move = compact_move.isJump();

	for (int i = 0; i < num_hops; i++) {
		Direction direction = Direction::ALL_DIRECTIONS[compact_move.getDirection(i)];

		// record jumped piece of applicable
		if (is_jumping_move) {
			normal_move.addJumpedPiecePosition(position.getOffset(direction, false));
		}

		// add next position
		position = position.getOffset(direction, is_jumping_move);
		normal_move.addPosition(position);
	}

	return normal_move;
}


Move Engine::findBestMove(const Game &game) {
	// initialize engine internal state
	Bitboard board = convertBoardToBitboard(game.getBoard());
	bool is_whites_turn = (game.getTurn() == Turn::WHITE);

	CompactMove best_move;

	negamax(board, is_whites_turn, MAX_DEPTH, &best_move);

	return convertCompactMovetoNormalMove(best_move);
}


// best_move is optional - used for root call to this function
int Engine::negamax(const Bitboard &board, bool is_whites_turn, int depth, CompactMove *best_move) {
	if (depth == 0) {
		return evaluate(board) * (is_whites_turn ? -1 : 1);
	}

	Bitboard next_positions[MAX_MOVES];
	CompactMove moves_available[MAX_MOVES];

	int moves_found = generateMoves(board, is_whites_turn, next_positions, best_move != nullptr ? moves_available : nullptr);

	int value = -INT_MAX;

	for (int i = 0; i < moves_found; i++) {
		int new_value = -negamax(next_positions[i], !is_whites_turn, depth - 1, nullptr);

		if (new_value > value) {
			value = new_value;
			if (best_move != nullptr) {
				*best_move = moves_available[i];
			}
		}
	}

	return value;
}


// returns a value representing how good the given board position is for black
// positive values mean that black is winning and negative values mean white is winning
int Engine::evaluate(const Bitboard &board) {
	constexpr int piece_value = 100;
	constexpr int king_value = 140;

	int black_men = 0;
	int black_kings = 0;
	int white_men = 0;
	int white_kings = 0;

	for (int i = 0; i < 32; i++) {
		bool is_black_piece = board.black_pieces & (1u << i);
		bool is_white_piece = board.white_pieces & (1u << i);
		bool is_king_piece = board.king_pieces & (1u << i);

		if (is_black_piece) {
			if (is_king_piece) {
				black_kings++;
			} else {
				black_men++;
			}
		} else if (is_white_piece) {
			if (is_king_piece) {
				white_kings++;
			} else {
				white_men++;
			}
		}
	}

	int value = (black_men - white_men) * piece_value
		+ (black_kings - white_kings) * king_value;

 	// TODO: devise method to encourage exchanges when in a winning position

	return value;
}

