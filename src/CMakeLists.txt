set(MAIN_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(game)
add_subdirectory(engine)
add_subdirectory(tui)
add_subdirectory(gui)

set(PROJECT_SOURCES
	${MAIN_SOURCES}
	${GAME_SOURCES}
	${ENGINE_SOURCES}
	${TUI_SOURCES}
	${GUI_SOURCES}
)

set(PROJECT_RESOURCES ../resources.qrc)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_RESOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
