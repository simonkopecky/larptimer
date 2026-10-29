#include <stdio.h>
#include <string.h>
#include <time.h>

#include "figlet-helper.h"
#include "clock.h"
#include "args.h"

int main(int argc, char **argv) {
	if (!hanle_args(argc, argv)) return 1;

	size_t font_dir_count = sizeof(font_dirs) / sizeof(font_dirs[0]);
	char *font_file_path = find_font_file(font_name, font_dirs, font_dir_count);
	if (font_file_path == NULL) {
		printf("Couldn't find font with name %s\n", font_name);
		return 1;
	}

	//printf("font_file_path: %s\n", font_file_path);

	char *char_set = "0123456789:";
	int char_set_length = strlen(char_set);

	Figlet_Char *font = get_font_from_file(char_set, font_file_path);
	monospace(font, char_set_length);

	if (use_as_clock) {
		lt_clock(font);
	}
	else {
		int duration = string_to_time(start_duration_text);

		lt_timer(duration, font);
	}

	return 0;
}
