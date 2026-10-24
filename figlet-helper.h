
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

typedef struct {
	char *data;
	char character;
	int height;
	int width;
} Big_Char;

char* tolower_string(char *input) {
	int i = 0;
	size_t input_length = strlen(input) + 1;

	if (input_length == 0) return NULL;

	char* result = malloc(input_length);

	if (result == NULL) return NULL;

	while (input[i] != '\0') {
		result[i] = tolower(input[i]);
		i++;
	}

	result[input_length - 1] = '\0';

	return result;
}

bool compare_case_insensitive(char *s1, char *s2) {
	int i = 0;
	while (s1[i] != '\0' && s2[i] != '\0') {
		if (tolower(s1[i]) != tolower(s2[i])) return false;

		i++;
	}

	if ((s1[i] != '\0') != (s2[i] != '\0')) return false;

	return true;
}


char *find_font_file(char *font_name, char **font_dirs, size_t font_dir_count) {
	char *lower_case_name = tolower_string(font_name);

	if (lower_case_name == NULL) return NULL;

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
			char *lower_case_file_name = tolower_string(file_name);
			
			if (file_name_length < 4 || strcmp(&file_name[file_name_length-4], ".flf") != 0) continue;

			lower_case_file_name[file_name_length-4] = '\0';

			if (strcmp(lower_case_name, lower_case_file_name) == 0) {
				size_t result_length = current_dir_name_length + file_name_length + 1;
				char *result = malloc(result_length);
				if (result == NULL) {
					printf("Not enough memory.");
					return NULL;
				}
				strcpy(result, current_dir_name);
				strcat(result, file_name);
				result[result_length - 1] = '\0';
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

char *get_big_char(char *file_buffer, int index, int height, int *width) {
	*width = 0;

	for (int i = index; file_buffer[i] != '\n' && file_buffer[i] != '\r'; i++) *width += 1;

	if (*width <= 1) {
		return NULL;
	}

	int result_char_length = *width * height + 1; // +1 for \0

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

Big_Char *get_font_from_file(char *char_list, char *font_file_path) {
	char* file_buffer = file_to_string(font_file_path);

	if (file_buffer == NULL) {
		printf("Couldn't open font file: %s.\n", font_file_path);
		return NULL;
	}

	char hardblank;
	int height, baseline, max_length, smushmode, comment_lines;

	if (6 != sscanf(file_buffer, "flf2a%c %d %d %d %d %d",
		&hardblank, &height, &baseline, &max_length, &smushmode, &comment_lines)) {
		printf("Couldn't read header line from file %s\n", font_file_path);
		return NULL;
	}

	int font_chars_length = strlen(char_list);
	Big_Char *font_chars = malloc(font_chars_length * sizeof(Big_Char));
	Big_Char *current_font_char = font_chars;
	
	for (char *current_char = char_list; *current_char != '\0'; current_char++) {
		int start_index = get_char_start_index(*current_char, file_buffer, height, comment_lines);

		int width = 0;
		char *data = get_big_char(file_buffer, start_index, height, &width);

		current_font_char->data = data;
		current_font_char->character = *current_char;
		current_font_char->height = height;
		current_font_char->width = width;

		if (data == NULL) {
			//printf("Font %s doesn't contain character for: %c\n", font_file_path, *current_char);
			printf("\"%s\" ", font_file_path);
			return NULL;
		}
		current_font_char += 1;
	}

	return font_chars;
}

void add_spaces(Big_Char *bc, int left, int right) {
	int result_width = bc->width + left + right;
	int result_size = result_width * bc->height;
	char *result = malloc(result_size + 1);
	
	char *data = bc->data;
	for (int line = 0; line < bc->height; line++) {
		int ch = 0;
		for (; ch < result_width - 1; ch++) {
			if (ch < left) {
				result[ch + line * result_width] = ' ';
			}
			else if (ch < left + bc->width - 1) {
				result[ch + line * result_width] = data[ch - left + line * bc->width];
			}
			else {
				result[ch + line * result_width] = ' ';
			}

		}

		result[ch + line * result_width] = '\n';
	}

	result[result_size] = '\0';

	bc->data = result;
}

void monospace(Big_Char *font, int font_size) {
	int max_width = 0;
	for (int i = 0; i < font_size; i++) {
		if (font[i].width > max_width) max_width = font[i].width;
	}

	for (int i = 0; i < font_size; i++) {
		if (font[i].width == max_width) continue;

		int diff = max_width - font[i].width;

		if (diff % 2 == 0) {
			add_spaces(font + i, diff / 2, diff / 2);
		}
		else {
			add_spaces(font + i, diff / 2 + 1, diff / 2);
		}

		font[i].width = max_width;

	}
}

char *get_big_char_line(char *big_char, int width) {
	char *result = malloc(width + 1);

	if (result == NULL) return NULL;

	for (int i = 0; big_char[i] != '\n'; i++) result[i] = big_char[i];

	result[width] = '\0';

	return result;
}

int set_big_char_line(char *dest, char *big_char, int width) {
	char *result = get_big_char_line(big_char, width);

	if (result == NULL) return -1;

	int i = 0;
	for (; result[i] != '\0'; i++) {
		if (i >= width) return -1;

		dest[i] = result[i];
	}

	return i;
}

char *cat_big_chars(char *text, Big_Char *font) {
	if (*text == '\0') return NULL;

	char first_char = font[0].character;
	int height = font[0].height;
	int line_index = 0;

	int result_length = 0;
	for (int i = 0; text[i] != '\0'; i++) {
		result_length += font[text[i] - first_char].width - 1;
	}

	result_length *= height;
	result_length += height;


	char *result = malloc(result_length + 1);
	int current_result_index = 0;

	for (int i = 0; ; i++) {
		if (line_index == height) break;

		if (text[i] == '\0') {
			line_index++;
			i = -1;
			result[current_result_index] = '\n';
			current_result_index += 1;
			continue;
		}

		Big_Char current_big_char = font[text[i] - first_char];

		int added_char_count = set_big_char_line(
			result + current_result_index,
			current_big_char.data + line_index * current_big_char.width,
			current_big_char.width - 1
		);


		if (added_char_count <= 0) {
			printf("bruh");
			return NULL;
		}
		
		current_result_index += added_char_count;
	}

	result[result_length] = '\0';

	return result;
}

