CC = gcc
CFLAGS = -Wall -Wextra -static
TARGET = dbt

RELEASE_CFLAGS = -O2 -DNDEBUG
DEBUG_CFLAGS = -O0 -g

all: release

release: $(TARGET)

debug: clean
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -o $(TARGET) dbt.c

$(TARGET): dbt.c
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) -o $(TARGET) dbt.c

clean:
	rm -f $(TARGET)
