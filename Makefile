CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET   := food_delivery_system

SRCS := main.cpp \
        src/ui.cpp \
        src/models.cpp \
        src/bst.cpp \
        src/graph.cpp \
        src/hashtable.cpp \
        src/sorting.cpp \
        src/admin.cpp \
        src/customer.cpp \
        src/system.cpp

OBJS := $(patsubst %.cpp, build/%.o, $(SRCS))

all: $(TARGET)
	@echo ""
	@echo "  Build successful.  Run:  ./$(TARGET)"
	@echo ""

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/main.o: main.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/src/%.o: src/%.cpp
	@mkdir -p build/src
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
