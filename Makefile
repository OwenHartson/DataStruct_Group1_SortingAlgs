CXXFLAGS = -g -O3 -Wpedantic -Wall -Wextra -Wmisleading-indentation -Wunused -Wuninitialized -Wshadow -Wconversion -std=c++20

OBJS = main

main:

.PHONY: clean

clean: 
	rm -f *.o ${OBJS}