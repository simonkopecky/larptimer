#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#ifndef DEFAULT_FONT_NAME
#define DEFAULT_FONT_NAME "standard"
#endif

#ifndef FONT_DIRECTORIES
#define FONT_DIRECTORIES
char *font_dirs[] = {
	"/usr/share/figlet/",
	"/usr/share/figlet/fonts/",
	"figlet-fonts/"
};
#endif

#define FIRST_CHAR_ASCII 32


char *font_name = DEFAULT_FONT_NAME;

int lower(char *input) {
	int i = 0;
	while (input[i] != '\0') {
		input[i] = tolower(input[i]);
		i++;
	}

	return i + 1;
}

bool compare_case_insensitive(char *s1, char *s2) {
	// WARNING: compares only to the length of the shorter string, so strings with different lengths can result in return true
	for (size_t i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
		if (tolower(s1[i]) != tolower(s2[i])) return false;
	}

	return true;
}

bool hanle_args(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		char *current_arg = argv[i];
		int arg_length = lower(current_arg);

		if (strcmp(current_arg, "--list") == 0) {
			printf("List of all fonts\n");
			continue;
		}
		else if (strcmp(current_arg, "--help") == 0) {
			printf("Help\n");
			continue;
		}

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
			default:
				printf("Unknown command: %s\n", current_arg);
				return false;
		}
	}
	
	return true;
}

char *find_font_file(char *font_name, char **font_dirs, size_t font_dir_count) {

	for (size_t i = 0; i < font_dir_count; i++) {
		char *current_dir_name = font_dirs[i];
		size_t current_dir_name_length = strlen(current_dir_name);
		DIR *current_dir = opendir(current_dir_name);
		struct dirent *file;

		if (current_dir == NULL) {
			//printf("Couldn't open folder %s. Skipping...\n", current_dir_name);
			continue;
		}

		while ((file = readdir(current_dir)) != NULL) {
			char *file_name = file->d_name;
			size_t file_name_length = strlen(file_name);
			
			if (file_name_length < 4 || strcmp(&file_name[file_name_length-4], ".flf") != 0) continue;

			if (compare_case_insensitive(font_name, file_name)) {
				char *result = malloc((current_dir_name_length + file_name_length) * sizeof(char));
				if (result == NULL) {
					printf("Not enough memory.");
					return NULL;
				}
				strcpy(result, current_dir_name);
				strcat(result, file_name);
				return result;
			}
		}
	}

	return NULL;
}

int get_char_start_index(char target_char, char *file_buffer, int height, int comment_lines) {
	int line_index = 0;
	int start_line_index = + height * ((int)target_char - FIRST_CHAR_ASCII) + comment_lines + 1; // +1 for header

	for (int i = 0; file_buffer[i] != '\0'; i++) {
		char current_file_char = file_buffer[i];

		if (current_file_char != '\n') continue;

		line_index += 1;

		if (line_index == start_line_index) {
			return i + 1;
		}
	}

	return -1;
}

char *get_big_char(char *file_buffer, int index, int height) {
	int width = 0; // +1 for the new line character
	
	printf("%d\n", index);

	for (int i = index; file_buffer[i] != '\n' && file_buffer[i] != '\r'; i++) width += 1;

	if (width <= 1) {
		return NULL;
	}

	int result_char_length = width * height + 1; // +1 for \0

	char* result_char = malloc(result_char_length);

	int result_char_index = 0;

	for (int i = index; result_char_index < result_char_length - 1; i++) {
		if (file_buffer[i] == '\0') return NULL;

		if (file_buffer[i + 1] == '\n' || file_buffer[i + 1] == '\r') {
			continue;
		}
		else {
			result_char[result_char_index] = file_buffer[i];
		}

		result_char_index += 1;
	}

	result_char[result_char_length - 1] = '\0';
	result_char[result_char_length - 2] = '\n';

	return result_char;
}

char *file_to_string(char* path) {
	FILE *file;
	int file_size;

	file = fopen(path, "r");
	if (file == NULL) return NULL;

	fseek(file, 0L, SEEK_END);
	file_size = ftell(file);
	rewind(file);

	char *file_buffer = calloc(1, file_size + 1);

	fread(file_buffer, sizeof(char), file_size, file);
	file_buffer[file_size] = '\0';
	fclose(file);

	return file_buffer;
}
int main(int argc, char **argv) {
	
	if (!hanle_args(argc, argv)) return 1;

	size_t font_dir_count = sizeof(font_dirs) / sizeof(font_dirs[0]);
	char *font_file_path = find_font_file(font_name, font_dirs, font_dir_count);
	if (font_file_path == NULL) {
		printf("Couldn't find font with name %s\n", font_name);
		return 1;
	}

	printf("%s\n", font_file_path);


	return 0;
}
