set(MAIN_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(game)
add_subdirectory(engine)
add_subdirectory(tui)

set(PROJECT_SOURCES
	${MAIN_SOURCES}
	${GAME_SOURCES}
	${ENGINE_SOURCES}
	${TUI_SOURCES}
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
