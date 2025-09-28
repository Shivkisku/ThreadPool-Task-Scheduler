CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c99 -O2 -g
TARGET = task_scheduler
SOURCES = main.c task_scheduler.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean debug release

debug: CFLAGS += -DDEBUG -g
debug: $(TARGET)

release: CFLAGS += -DNDEBUG -O3
release: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)