#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <time.h>
#define DEB_TIME_MS 20
#define HIST_SIZE 64
#define UP 0
#define DOWN 1

static inline long long time_ms(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
}

void event_write(const struct input_event *event) {
	if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
		exit(EXIT_FAILURE);
}

int event_read(struct input_event *event) {
	while (1){
		const int ret = fread(event, sizeof(struct input_event), 1, stdin) == 1;
		if (!ret)
			return ret;
		return ret;
	}
}

long long qTime[HIST_SIZE] = {0};
__u16 qCode[HIST_SIZE] = {0};
int qIdx = 0;

void push(__u16 code){
	qTime[qIdx] = time_ms();
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
	while (event_read(&ev)) {
		if (ev.value == UP){
			event_write(&ev);
			push(ev.code);
			continue;
		}
		if (find(ev.code)) continue;
		event_write(&ev);
	}
	fprintf(stderr, "stdin got EOF. Bye Bye\n");
}
