set(
	BINARY
		${PROJECT_NAME}_run
)

add_executable(
	${BINARY}
		main.cpp
)

target_link_libraries(
	${BINARY}
		game
		tui
		engine
)

add_subdirectory(game)
add_subdirectory(tui)
add_subdirectory(engine)
