# VARIABLES
ANSI_RED=\x1b[31m
ANSI_GREEN=\x1b[32m
ANSI_RESET=\x1b[37m

CC=gcc
CFLAGS=-g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic -Wno-unused-function
CFLAGS_GCOV=$(CFLAGS) -fprofile-arcs -ftest-coverage -DNDEBUG
SHELL=/bin/bash

BASE_NAME=delay
SRC_C=$(BASE_NAME).c stack.c tree.c
SRC_H=$(BASE_NAME).h
EXPECTED=pa1_examples/3.pre
MAIN=pa1.c

TEST_C=test_$(BASE_NAME).c
TEST_EXPECTED=pa1_examples/5.pre

EXECUTABLE=pa1
EXECUTABLE_TEST=test_$(BASE_NAME)
EXECUTABLE_GCOV=$(EXECUTABLE)_gcov

# RULES
$(EXECUTABLE): $(SRC_C) $(MAIN) $(SRC_H) 
	$(CC) -o $(EXECUTABLE) $(SRC_C) $(MAIN) $(CFLAGS)

$(EXECUTABLE_TEST): $(SRC_C) $(TEST_C) $(SRC_H) 
	$(CC) -o $(EXECUTABLE_TEST) $(SRC_C) $(TEST_C) $(CFLAGS)

coverage: $(SRC_C) $(TEST_C) $(SRC_H) 
	$(CC) -o $(EXECUTABLE_GCOV) $(SRC_C) $(TEST_C) $(CFLAGS_GCOV)
	./${EXECUTABLE_GCOV}
	gcov -f ${SRC_C}

test: $(EXECUTABLE)
	valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full ./$(EXECUTABLE)
	@# If actual output matches expected output then count it as a success
	@if diff -a -B <("./$(EXECUTABLE)") $(EXPECTED) &> /dev/null ; then \
		if [ -t 2 ]; then echo -e "${ANSI_GREEN}Test passed: output of $(EXECUTABLE) matches $(EXPECTED)${ANSI_RESET}" >> /dev/stderr; fi \
	else \
		if [ -t 2 ]; then echo -e "${ANSI_RED}Test failed: output of $(EXECUTABLE) does not match $(EXPECTED)${ANSI_RESET}" >> /dev/stderr; fi \
	fi

unittest: $(EXECUTABLE_TEST)
	valgrind --track-origins=yes --leak-check=full ./$(EXECUTABLE_TEST)
	@# If actual output matches expected output then count it as a success
	@if diff -a -B <("./$(EXECUTABLE_TEST)") $(TEST_EXPECTED) &> /dev/null ; then \
		if [ -t 2 ]; then echo -e "${ANSI_GREEN}Test passed: output of $(EXECUTABLE_TEST) matches $(TEST_EXPECTED)${ANSI_RESET}" >> /dev/stderr; fi \
	else \
		if [ -t 2 ]; then echo -e "${ANSI_RED}Test failed: output of $(EXECUTABLE_TEST) does not match $(TEST_EXPECTED)${ANSI_RESET}" >> /dev/stderr; fi \
	fi

clean:
	rm -f $(EXECUTABLE) $(EXECUTABLE_TEST) $(EXECUTABLE_GCOV) *.c.gcov *.gcda *.gcno && rm -rf $(EXECUTABLE).dSYM && rm -rf $(EXECUTABLE_TEST).dSYM

.PHONY: clean 
.PHONY: coverage 
.PHONY: test
.PHONY: unittest 
