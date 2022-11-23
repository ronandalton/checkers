set(
	ENGINE_SOURCES
		bitboard.h
		bitboard_movegen.cpp
		bitboard_movegen.h
		compact_move.cpp
		compact_move.h
		engine.cpp
		engine.h
)

add_library(engine STATIC ${ENGINE_SOURCES})
