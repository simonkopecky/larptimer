#include <stdio.h>
#include "figlet-helper.h"

int main(int argc, char **argv) {
	
	if (!hanle_args(argc, argv)) return 1;

	size_t font_dir_count = sizeof(font_dirs) / sizeof(font_dirs[0]);
	char *font_file_path = find_font_file(font_name, font_dirs, font_dir_count);
	if (font_file_path == NULL) {
		printf("Couldn't find font with name %s\n", font_name);
		return 1;
	}

	//printf("font_file_path: %s\n", font_file_path);

	char *test_time = "69:69";
	char *char_set = "0123456789:";

	Big_Char *font = get_font_from_file(char_set, font_file_path);

	print_big_chars(test_time, font);
	//printf("%s", font[1].data);

	return 0;
}
