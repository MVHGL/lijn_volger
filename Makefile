CC := g++
SRCDIR_1 := src/course_one
SRCDIR_2 := src/course_two
LIBDIR := lib
TARGET := bin

SRCEXT := cpp
SOURCES_1 := $(shell find $(SRCDIR_1) -type f -name *.$(SRCEXT))
SOURCES_2 := $(shell find $(SRCDIR_2) -type f -name *.$(SRCEXT))
LIBRARIES := $(shell find $(LIBDIR) -type f -name *.$(SRCEXT))
CFLAGS :=  --std=c++14

all:
	@echo "Making all courses..."
	$(CC) $(CFLAGS) $(SOURCES_1) $(LIBRARIES) -o $(TARGET)/course_one
	$(CC) $(CFLAGS) $(SOURCES_2) $(LIBRARIES) -o $(TARGET)/course_two
	@echo "You can find the executable in $(TARGET)"

course_one:
	@echo "Making first course..."
	$(CC) $(CFLAGS) $(SOURCES_1) $(LIBRARIES) -o $(TARGET)/course_one
	@echo "You can find the executable in $(TARGET)"

course_two:
	@echo "Making matrix course..."
	$(CC) $(CFLAGS) $(SOURCES_2) $(LIBRARIES) -o $(TARGET)/course_two
	@echo "You can find the executable in $(TARGET)"

.PHONY: clean
clean:
	@echo " Cleaning..."
	@rm -f bin/*

