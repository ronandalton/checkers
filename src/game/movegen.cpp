#include "game/movegen.h"

#include "game/board.h"
#include "game/turn.h"
#include "game/move.h"
#include "game/position.h"
#include "game/coord.h"
#include "game/direction.h"
#include "game/piece.h"


// returns list of moves available for the player whose turn it is on board
std::vector<Move> MoveGen::generateMoves(const Board &board, Turn turn) {
	std::vector<Move> moves;
	
	bool jumping = true;
	
	for (int loop = 0; loop < 2; loop++) {
		for (Position position : Position::ALL_POSITIONS) {
			if (board.pieceAt(position).belongsTo(turn)) {
				std::vector<Move> new_moves;
				if (jumping) {
					new_moves = findJumpingMoves(board, position);
				} else {
					new_moves = findNormalMoves(board, position);
				}
				moves.insert(moves.end(), new_moves.begin(), new_moves.end()); // add moves just found to moves list
			}
		}
		
		if (moves.empty()) {
			jumping = false;
			continue;
		} else {
			break;
		}
	}
	
	return moves;
}


// returns the board position after making a move
// it is assumed that the move is valid
Board MoveGen::getBoardAfterMove(Board board, const Move &move) {
	Position start_position = move.getStartPosition();
	
	Piece piece = board.pieceAt(start_position);
	board.pieceAt(start_position).remove();
	
	Position end_position = move.getEndPosition();
	
	Turn turn = piece.getTurn();
	
	if (end_position.isCrowningSquare(turn)) {
		piece.crown();
	}
	
	board.pieceAt(end_position) = piece;
	
	for (Position jumped_piece_position : move.getJumpedPiecePositions()) {
		board.pieceAt(jumped_piece_position).remove();
	}

	return board;
}


// returns the board position after making a move defined by a start and end position
// it is assumed that the move is valid
// the move can be either a normal move or a jumping move, but a not a multi-jump move
Board MoveGen::getBoardAfterSimpleMove(Board board, Position start_position, Position end_position) {
	// try all possible moves from start_position to find one with matching end_position
	// needed in order to find jumped piece in the case of jumping moves
	for (int is_jumping = 0; is_jumping < 2; is_jumping++) {
		for (Direction direction : Direction::ALL_DIRECTIONS) {
			if (start_position.offsetIsValid(direction, is_jumping)) {
				const Move move = getMove(start_position, direction, is_jumping);
				if (move.getEndPosition() == end_position) {
					board = getBoardAfterMove(board, move);
					break;
				}
			}
		}
	}

	return board;
}


// returns true if the move described is legal in isolation (eg. ignoring potential compulsory jumps elsewhere on board)
bool MoveGen::moveIsLegal(const Board &board, Position position, Direction direction, bool jumping) {
	if (board.pieceAt(position).canMoveInDirection(direction)) {
		if (position.offsetIsValid(direction, jumping)) { // move would not go outside boundaries
			Position adjacent_position = position.getOffset(direction, false);
			
			if (jumping) {
				Position beyond_position = position.getOffset(direction, true);
				
				Turn opposite_turn = board.pieceAt(position).getOppositeTurn();
				
				if (board.pieceAt(adjacent_position).belongsTo(opposite_turn)) {
					if (board.pieceAt(beyond_position).exists() == false) {
						return true;
					}
				}
			} else {
				if (board.pieceAt(adjacent_position).exists() == false) {
					return true;
				}
			}
		}
	}
	
	return false;
}


// construct a simple move from position in direction which may be jumping
// it is assumed that the landing square will be valid
Move MoveGen::getMove(Position position, Direction direction, bool jumping) {
	Move move;
	
	move.addPosition(position);
	move.addPosition(position.getOffset(direction, jumping));
	
	if (jumping) {
		move.addJumpedPiecePosition(position.getOffset(direction, false));
	}
	
	return move;
}


// returns true if making the move will result in a non-crowned piece being crowned
// it is assumed that the move is valid
bool MoveGen::moveCrownsPiece(const Board &board, const Move &move) {
	Piece piece = board.pieceAt(move.getStartPosition());
	if (piece.isCrowned() == false) {
		if (move.getEndPosition().isCrowningSquare(piece.getTurn())) {
			return true;
		}
	}
	
	return false;
}


// returns a list of non-jumping moves that the piece at position can do on board
std::vector<Move> MoveGen::findNormalMoves(const Board &board, Position position) {
	std::vector<Move> moves;

	for (Direction direction : Direction::ALL_DIRECTIONS) {
		if (moveIsLegal(board, position, direction, false)) {
			moves.push_back(getMove(position, direction, false));
		}
	}

	return moves;
}


// returns a list of completed jumping moves which are valid extensions of partial_move
// partial_move should contain the single starting position in the initial call to this function
// this function operates recursively by updating board and partial_move until the entire jumping tree has been explored
std::vector<Move> MoveGen::findJumpingMoves(const Board &board, const Move &partial_move) {
	std::vector<Move> moves;

	Position position = partial_move.getEndPosition();
	
	for (Direction direction : Direction::ALL_DIRECTIONS) {
		if (moveIsLegal(board, position, direction, true)) {
			Position end_position = position.getOffset(direction, true);
			Position jumped_piece_position = position.getOffset(direction, false);
			
			Move new_partial_move = partial_move;
			new_partial_move.addPosition(end_position);
			new_partial_move.addJumpedPiecePosition(jumped_piece_position);
			
			Move simple_move({position, end_position}, {jumped_piece_position});
			
			if (!moveCrownsPiece(board, simple_move)) {
				std::vector<Move> new_moves = findJumpingMoves(getBoardAfterMove(board, simple_move), new_partial_move);
				if (new_moves.empty()) {
					new_moves.push_back(new_partial_move);
				}
				moves.insert(moves.end(), new_moves.begin(), new_moves.end()); // add double jumps to moves found
			} else {
				moves.push_back(new_partial_move);
			}
		}
	}

	return moves;
}
