#include <stdio.h>
#include <string.h>

#include "figlet-helper.h"
#include "display.h"

bool hanle_args(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		char *current_arg = argv[i];
		int arg_length = strlen(current_arg);

		if (strcmp(current_arg, "--list") == 0) {
			printf("List of all fonts\n");
			break;
		}
		else if (strcmp(current_arg, "--help") == 0) {
			printf("Help\n");
			break;
		}

		if (current_arg[0] != '-' || arg_length != 2) {
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
				i += 2;
				break;
			default:
				printf("Unknown command: %s\n", current_arg);
				return false;
		}
	}
	
	return true;
}

int string_to_time(char *text) {
	int num_count = 0;
	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] < 48 || text[i] > 58) return -1;

		if (text[i] == ':' || num_count == 0) {
			num_count++;
		}
	}

	if (num_count > 4) {
		printf("Wrong time format.");
		return -1;
	}

	int *vals = malloc(num_count * sizeof(int));
	int scan_result = 0;

	switch (num_count) {
		case 1:
			scan_result = sscanf(text, "%d", vals + 0);
			break;
		case 2:
			scan_result = sscanf(text, "%d:%d", vals + 0, vals + 1);
			break;
		case 3:
			scan_result = sscanf(text, "%d:%d:%d", vals + 0, vals + 1, vals + 2);
			break;
		case 4:
			scan_result = sscanf(text, "%d:%d:%d:%d", vals + 0, vals + 1, vals + 2, vals + 3);
			break;
	}

	if (num_count != scan_result) {
		return -1;
	}

	int to_second[] = {1, 60, 3600, 3600 * 24};

	int result = 0;

	for (int i = 0; i < num_count; i++) {
		result += vals[num_count - i - 1] * to_second[i];
	}

	return result;
}

char *time_to_string(int time) {
	int divs[] = [60, 60, 24, 1];

	int vals[4];
	int result_size = 0;

	for (int i = 0; i < 4; i++) {
		vals[i] = time % divs[i];
		int num_len = sprintf(NULL, "%d", vals[i]);
		if (num_len < 0) return NULL;

		result_size += num_len + (i > 0);

		time /= divs[i];
	}

	char *result = malloc(result_size);

	if (result == NULL) return NULL;

}

int main(int argc, char **argv) {
	if (!hanle_args(argc, argv)) return 1;

	size_t font_dir_count = sizeof(font_dirs) / sizeof(font_dirs[0]);
	char *font_file_path = find_font_file(font_name, font_dirs, font_dir_count);
	if (font_file_path == NULL) {
		printf("Couldn't find font with name %s\n", font_name);
		return 1;
	}

	//printf("font_file_path: %s\n", font_file_path);

	char *test_time = "1:30";
	printf("secs: %d\n", string_to_time(test_time));

	char *char_set = "0123456789:";
	int char_set_length = strlen(char_set);
	int text_width = strlen(test_time);

	Big_Char *font = get_font_from_file(char_set, font_file_path);
	monospace(font, char_set_length);

	int max_width = 0;
	for (int i = 0; i < char_set_length; i++) {
		if (font[i].width > max_width) max_width = font[i].width;
	}

	char *result = cat_big_chars(test_time, font);

	
	print_text(result);

	return 0;
}
