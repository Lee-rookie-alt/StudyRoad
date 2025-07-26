SRCS := $(wildcard *.cpp)
OUTS := $(patsubst %.cpp,%,$(SRCS))
CC :=g++
COM_OP := -Wall -g
.PHONY: clean rebuild all

all: $(OUTS)
%: %.cpp
	$(CC) $^ -o $@ $(COM_OP)
clean:
	$(RM) $(OUTS)

rebuild: clean all
