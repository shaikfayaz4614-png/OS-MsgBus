CC = gcc

CFLAGS = -Wall -Wextra
PTHREAD = -pthread
RT = -lrt

BIN = bin

all: directories broker publisher subscriber msgbus \
     race_test deadlock_test memory_test logger_test sync_test

directories:
	mkdir -p $(BIN)

broker: src/broker.c src/shared_memory.c
	$(CC) $(CFLAGS) src/broker.c src/shared_memory.c \
	-o $(BIN)/broker $(PTHREAD) $(RT)

publisher: src/publisher.c src/shared_memory.c
	$(CC) $(CFLAGS) src/publisher.c src/shared_memory.c \
	-o $(BIN)/publisher $(PTHREAD) $(RT)

subscriber: src/subscriber.c src/shared_memory.c
	$(CC) $(CFLAGS) src/subscriber.c src/shared_memory.c \
	-o $(BIN)/subscriber $(PTHREAD) $(RT)

msgbus: src/main.c src/synchronization.c src/logger.c
	$(CC) $(CFLAGS) src/main.c \
	src/synchronization.c src/logger.c \
	-o $(BIN)/msgbus $(PTHREAD)

race_test: tests/race_test.c
	$(CC) $(CFLAGS) tests/race_test.c \
	-o $(BIN)/race_test $(PTHREAD)

deadlock_test: tests/deadlock_test.c
	$(CC) $(CFLAGS) tests/deadlock_test.c \
	-o $(BIN)/deadlock_test $(PTHREAD)

memory_test: tests/memory_test.c
	$(CC) $(CFLAGS) tests/memory_test.c \
	-o $(BIN)/memory_test

logger_test: logger_test.c src/logger.c
	$(CC) $(CFLAGS) logger_test.c src/logger.c \
	-o $(BIN)/logger_test

sync_test: sync_test.c src/synchronization.c
	$(CC) $(CFLAGS) sync_test.c src/synchronization.c \
	-o $(BIN)/sync_test $(PTHREAD)

clean:
	rm -rf $(BIN)
	rm -f logger_test.c sync_test.c msgbus.log

.PHONY: all directories broker publisher subscriber msgbus \
race_test deadlock_test memory_test logger_test sync_test clean
