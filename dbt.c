#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include <time.h>
#define DEB_TIME_MS 40
#define DEB_TIME_ADD_MS 30
#define HIST_SIZE 64
#define UP 0
#define DOWN 1

static inline long long time_ms(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
}

long long qTime[HIST_SIZE] = {0};
__u16 qCode[HIST_SIZE] = {0};
int qIdx = 0;

long long time_of_event(__u16 code){
	long long ms = time_ms();
	switch (code){
		case KEY_ENTER:
		case KEY_DOT:
		case KEY_COMMA:
			return ms - DEB_TIME_ADD_MS;
		default:
			return ms;
	}
}

void push(__u16 code){
	qTime[qIdx] = time_of_event(code);
	qCode[qIdx] = code;
	qIdx = (qIdx + 1) % HIST_SIZE;
}

int find(__u16 code){
	int thresh = time_ms() - DEB_TIME_MS;
	for (int i = 0; i < HIST_SIZE; i ++){
		if (qTime[i] > thresh && qCode[i] == code)
			return 1;
	}
	return 0;
}

int main(void) {
	setbuf(stdin, NULL), setbuf(stdout, NULL);
	struct input_event ev;
	while (fread(&ev, sizeof(struct input_event), 1, stdin) == 1) {
		if (ev.value == DOWN){
			if (find(ev.code))
				continue;
			push(ev.code);
			fwrite(&ev, sizeof(struct input_event), 1, stdout);
			continue;
		}
		fwrite(&ev, sizeof(struct input_event), 1, stdout);
	}
	fprintf(stderr, "stdin got EOF. Bye Bye\n");
}
