set(
	ENGINE_SOURCES
		bitboard.h
		convert.cpp
		convert.h
		engine.cpp
		engine.h
		movegen.cpp
		movegen.h
)

add_library(engine STATIC ${ENGINE_SOURCES})
