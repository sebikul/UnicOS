#include "video.h"
#include "types.h"
#include "keyboard.h"
#include "kernel.h"

#define SCREENSAVER_CONSOLE VIRTUAL_CONSOLES

static task_t *screensaver = NULL;
uint64_t screensaver_wait_ms = 20000;

void screensaver_set_wait(uint64_t s) {
	screensaver_wait_ms = s * 1000;
}

void screensaver_trigger() {
	if (screensaver != NULL) {
		task_ready(screensaver);
	}
}

static void screensaver_kbdhandler(uint64_t s) {

	if (screensaver->state == TASK_PAUSED) {
		//El screensaver esta activo
		kdebug("Restaurando consola\n");
		video_trigger_restore();
		task_ready(screensaver);
	} else {
		//El screensaver no esta activo, incrementamos el contador
		screensaver->sleep_limit += screensaver_wait_ms;
		kdebug("Actualizando contador del screensaver\n");
	}
}

static void screensaver_task(int argc, char **argv) {

	while (TRUE) {
		kdebug("Durmiendo la tarea del screensaver\n");
		task_sleep(screensaver, screensaver_wait_ms);
		kdebug("Activando el screensaver\n");
		video_trigger_screensaver();
		task_pause(screensaver);
	}
}

void screensaver_init() {
	video_set_color(SCREENSAVER_CONSOLE, COLOR_LIGHT_BROWN, COLOR_RED);

	video_write_string(SCREENSAVER_CONSOLE,
	                   "                  ___                                                           "
	                   "      ___        /  /\\                                                          "
	                   "     /  /\\      /  /::\\                                                         "
	                   "    /  /:/     /  /:/\\:\\                                                        "
	                   "   /  /:/     /  /:/~/:/                                                        "
	                   "  /  /::\\    /__/:/ /:/                                                         "
	                   " /__/:/\\:\\   \\  \\:\\/:/                                                          "
	                   " \\__\\/  \\:\\   \\  \\::/                                                           "
	                   "      \\  \\:\\   \\  \\:\\                                                           "
	                   "       \\__\\/    \\  \\:\\                                                          "
	                   "                 \\__\\/                                                          "
	                   "      ___           ___                         ___                             "
	                   "     /  /\\         /  /\\          ___          /__/\\        ___                 "
	                   "    /  /::\\       /  /::\\        /  /\\         \\  \\:\\      /  /\\                "
	                   "   /  /:/\\:\\     /  /:/\\:\\      /  /::\\         \\  \\:\\    /  /:/                "
	                   "  /  /:/~/::\\   /  /:/~/:/     /  /:/\\:\\    ___  \\  \\:\\  /__/::\\                "
	                   " /__/:/ /:/\\:\\ /__/:/ /:/___  /  /:/~/::\\  /__/\\  \\__\\:\\ \\__\\/\\:\\__             "
	                   " \\  \\:\\/:/__\\/ \\  \\:\\/:::::/ /__/:/ /:/\\:\\ \\  \\:\\ /  /:/    \\  \\:\\/\\            "
	                   "  \\  \\::/       \\  \\::/~~~~  \\  \\:\\/:/__\\/  \\  \\:\\  /:/      \\__\\::/            "
	                   "   \\  \\:\\        \\  \\:\\       \\  \\::/        \\  \\:\\/:/       /__/:/             "
	                   "    \\  \\:\\        \\  \\:\\       \\__\\/          \\  \\::/        \\__\\/              "
	                   "     \\__\\/         \\__\\/                       \\__\\/                            "
	                   "                                                                                "
	                   "                                                                                "
	                  );

	video_update_screen_color(VIRTUAL_CONSOLES);

	screensaver = task_create(screensaver_task, "screensaver", 0, NULL);
	task_setconsole(screensaver, SCREENSAVER_CONSOLE);
	task_ready(screensaver);

	keyboard_catch(0x00, screensaver_kbdhandler, NULL, KEYBOARD_ALLCONSOLES | KEYBOARD_WILDCARD | KEYBOARD_IGNORE, "screensaver");
}

