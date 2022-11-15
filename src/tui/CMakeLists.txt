set(
	TUI_SOURCES
		tui.cpp
		tui.h
)

add_library(tui STATIC ${TUI_SOURCES})

target_link_libraries(tui LINK_PUBLIC game)
