#include <string.h>

char* start_duration_text;
bool use_as_clock = true;

bool hanle_args(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		char *current_arg = argv[i];

		if (strcmp(current_arg, "--list") == 0) {
			printf("List of all fonts\n");
			return false;
		}
		else if (strcmp(current_arg, "--help") == 0) {
			printf("Help\n");
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

		printf("%s\n", font_name);
	}
	
	return true;
}
