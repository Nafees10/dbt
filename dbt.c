#include <stdio.h>
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
	while (fread(&ev, sizeof(struct input_event), 1, stdin) == 1) {
		if (find(ev.code)) continue;
		if (ev.value == UP)
			push(ev.code);
		fwrite(&ev, sizeof(struct input_event), 1, stdout);
	}
	fprintf(stderr, "stdin got EOF. Bye Bye\n");
}
