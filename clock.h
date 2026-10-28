#define TB_IMPL
#include "libs/termbox2.h"

void display_init() {
	tb_init();
}

void print_text(char *text) {
	int width = 0;
	int text_length = strlen(text);

	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\n') break;
		width++;
	}

	int height = text_length / (width + 1);

	int y_start = (tb_height() - height) / 2;
	int x_start = (tb_width() - width) / 2;

	tb_clear();

	tb_print(x_start, y_start, 0, 0, text);
	tb_present();
}
