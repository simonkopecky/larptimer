#include <string.h>
#include "figlet-helper.h"

char* start_duration_text;
bool use_as_clock = true;

char *help_message =
	"Usage: larptimer [DURATION] [-f FONT] [--list] [--help]\n\n"
	"DURATION\n"
	"	If specified, the program will run as a timer otherwise it will show current time like a clock.\n"
	"	Format: D:H:M:S\n"
	"	Examples:\n"
	"		69 days, 1 hour, 2 minutes and 3 seconds --> 69:01:02:03 or 69:1:2:3)\n"
	"		5 minutes --> 5:00 or 0:0:5:0\n\n"
	"-f FONT\n"
	"	Select a font for the clock/timer. To see available fonts, use --list.\n\n"
	"--list\n"
	"	Shows all available fonts.\n\n"
	"--help\n"
	"	No one knows what this argument does. Not even me ¯\\_(ツ)_/¯"
;

void help() {
	printf("%s\n", help_message);
}

bool hanle_args(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		char *current_arg = argv[i];

		if (strcmp(current_arg, "--list") == 0) {
			print_fonts(font_dirs, sizeof(font_dirs) / sizeof(char*));
			return false;
		}
		else if (strcmp(current_arg, "--help") == 0) {
			help();
			return false;
		}

		if (current_arg[0] != '-') {
			start_duration_text = current_arg;
			use_as_clock = false;
			continue;
		}

		switch (current_arg[1]) {
			case 'f':
				if (i == argc - 1 || argv[i + 1][0] == '-') {
					printf("Bad usage: -f expects a font name.\n");
					return false;
				}

				font_name = argv[i + 1];
				i += 1;
				break;
			default:
				printf("Unknown command: %s\n", current_arg);
				return false;
		}
	}
	
	return true;
}
