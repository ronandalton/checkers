include_directories(helper)

add_subdirectory(helper)

add_subdirectory(game)
add_subdirectory(tui)
add_subdirectory(engine)

set(
	BINARY
		${PROJECT_NAME}_test
)

add_executable(
	${BINARY}
		main.cpp
		${GAME_TEST_SOURCES}
		${TUI_TEST_SOURCES}
		${ENGINE_TEST_SOURCES}
		${HELPER_SOURCES}
)

target_link_libraries(
	${BINARY} PUBLIC
		gtest
		game
		tui
		engine
)
