#include <stdio.h>
#include <ctype.h>

#define DEFAULT_FONT_NAME "standard"

char *font_name = DEFAULT_FONT_NAME;

int lower(char *input) {
	int i = 0;
	while (input[i] != '\0') {
		input[i] = tolower(input[i]);
		i++;
	}

	return i + 1;
}

bool hanle_args(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		char *current_arg = argv[i];
		int arg_length = lower(current_arg);
		if (current_arg[0] != '-' || arg_length != 3) {
			printf("Unexpected argument: %s\n", current_arg);
			return false;
		}

		switch (current_arg[1]) {
			case 'f':
				if (i == argc - 1 || argv[i + 1][0] == '-') {
					printf("Bad usage: -f expects a font name.\n");
					return false;
				}

				font_name = argv[i + 1];
				lower(font_name);
				i += 2;
				break;
		}
	}
	
	return true;
}

int main(int argc, char **argv) {
	
	if (!hanle_args(argc, argv)) return 1;

	printf("%s\n", font_name);

	return 0;
}
