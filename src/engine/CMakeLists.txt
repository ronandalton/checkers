set(
	ENGINE_SOURCES
		bitboard_access.cpp
		bitboard_access.h
		bitboard.h
		bitstring_list.cpp
		bitstring_list.h
		convert.cpp
		convert.h
		engine.cpp
		engine.h
		hash.cpp
		hash.h
		hash_table.cpp
		hash_table.h
		piece_codes.h
		random.cpp
		random.h
)

add_library(engine STATIC ${ENGINE_SOURCES})
