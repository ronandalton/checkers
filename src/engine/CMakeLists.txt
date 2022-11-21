set(
	ENGINE_SOURCES
		bitboard.h
		engine.cpp
		engine.h
		movegen.cpp
		movegen.h
)

add_library(engine STATIC ${ENGINE_SOURCES})
