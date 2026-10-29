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
	int divs[] = {60, 60, 24, 1};
	int min_widths[] = {2, 2, 2, 1};

	int vals[4];
	int text_val_lens[4];
	int vals_len = 0;
	int result_size = 0;

	for (int i = 0; i < 4; i++) {
		vals[i] = time % divs[i];
		int num_len = snprintf(NULL, 0, "%d", vals[i]);
		if (num_len < 0) return NULL;

		vals_len++;
		text_val_lens[i] = num_len;

		time /= divs[i];

		result_size += time == 0 ? num_len + 1 : min_widths[i] + 1;

		if (time == 0) break;
	}

	char *result = malloc(result_size);

	if (result == NULL) return NULL;

	int result_index = 0;
	for (int i = vals_len - 1; i >= 0 ; i--) {
		
		if (i != vals_len - 1) {
			for (int j = 0; j < min_widths[i] - text_val_lens[i]; j++) {
				result[result_index] = '0';
				result_index++;
			}
		}

		int num_len = sprintf(result + result_index, "%d:", vals[i]);
		result_index += num_len;
	}

	result[result_size - 1] = '\0';

	return result;
}

long long get_time_ms() {
	struct timespec ts;

	timespec_get(&ts, TIME_UTC);

	return (long long)ts.tv_sec * 1000 + (long long)ts.tv_nsec / 1000000;
}

bool print_for_duration(char *text, long long duration_ms) {

	struct tb_event ev;

	while (duration_ms > 0) {

		long long prev_time = get_time_ms();
		print_text(text);

		if (tb_peek_event(&ev, duration_ms) == -1 || ev.type == TB_EVENT_KEY) {
			return false;
		}
		else {
			duration_ms -= get_time_ms() - prev_time;
		}
	} 

	return true;
}

void lt_timer(int duration, Figlet_Char *font) {
	display_init();

	while (duration > 0) {
		char *result = text_to_figlet(time_to_string(duration), font);

		if (!print_for_duration(result, 1000)) break;
		duration--;
	}

	tb_shutdown();
}

void lt_clock(Figlet_Char *font) {
	display_init();
	time_t current_time;

	while (true) {
		current_time = time(NULL);

		struct tm *current_localtime = localtime(&current_time);

		int buff_length = 8;
		char *buff = malloc(buff_length);

		strftime(buff, buff_length + 1, "%H:%M:%S", current_localtime);

		buff[buff_length] = '\0';

		char *result = text_to_figlet(buff, font);

		if (!print_for_duration(result, 250)) break;
	}

	tb_shutdown();
}
