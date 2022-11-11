set(
	GAME_SOURCES
		board.cpp
		board.h
		coord.cpp
		coord.h
		direction.cpp
		direction.h
		game.cpp
		game.h
		matchtype.h
		move.cpp
		move.h
		movegen.cpp
		movegen.h
		piece.cpp
		piece.h
		player.h
		position.cpp
		position.h
		turn.h
)

add_library(game STATIC ${GAME_SOURCES})
