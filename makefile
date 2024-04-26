CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =
TARGET = time_comparison
SRCS = time_comparison.c

all: $(TARGET)

$(TARGET): $(SRCS)
        $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRCS)

clean:
        $(RM) $(TARGET)
