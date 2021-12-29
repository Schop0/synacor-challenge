BIN := synacor-challenge

SRCS=$(wildcard src/*.cpp)

OBJS=$(SRCS:.cpp=.o)

${BIN}: $(OBJS)
	$(CXX) -o ${BIN} ${OBJS}

.PHONY: clean
clean:
	$(RM) ${BIN} $(OBJS)
