#include "tui/tui.h"
#include "gui/gui.h"

#include <cstring>


int main(int argc, char *argv[]) {
	bool run_tui = false;
	for (int i = 1; i < argc; i++) {
		if (std::strcmp(argv[i], "--tui") == 0) {
			run_tui = true;
			break;
		}
	}

	if (run_tui) {
		Tui tui;
		return tui.run(argc, argv);
	} else {
		Gui gui;
		return gui.run(argc, argv);
	}
}
