CC = g++
CFLAGS = -fPIC -Wall -std=c++23 -g -DDEBUG
LDFLAGS = -Iinclude -Llibs -lmysqlcppconn

UNITTEST_LDFLAGS = -lmysqlcpp -lcunit -Wl,-rpath,libs

bin = sev
unittest = unittest # exists only for unittest
shared = libmysqlcpp.so # exists only for debugging

SRC_DIR = src
OUT_DIR = out
INCLUDE_DIR = include/mysqlcpp
LIB_DIR = libs
TEST_DIR = test

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(SRCS:.cpp=.o)
OUT_OBJS = $(wildcard $(OUT_DIR)/*.o)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

all: $(shared) arrange

binary: all $(bin)

test: all $(shared) $(unittest)
	./test/unittest

test-no-run: all $(shared) $(unittest)

$(OUT_DIR):
	mkdir -p $@
	mkdir -p $@/libs
	mkdir -p $@/include
	mkdir -p $@/include/mysqlcpp

arrange: $(shared)
	mv *.o *.d $(OUT_DIR)

%.o: $(SRC_DIR)/%.cpp $(OUT_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(shared): $(OBJS)
	mkdir -p $(LIB_DIR)
	mkdir -p include
	mkdir -p $(INCLUDE_DIR)
	$(CC) -shared $(CFLAGS) -o libs/$@ $(OBJS) $(LDFLAGS)
	cp $(SRC_DIR)/*.h include/mysqlcpp	
	cp $(SRC_DIR)/*.h $(OUT_DIR)/include/mysqlcpp
	cp libs/$@ $(OUT_DIR)/libs


$(bin): arrange
	$(CC) $(CFLAGS) $(OUT_OBJS) -o $@ $(LDFLAGS)
	
$(unittest): arrange
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_DIR)/$@ $(LDFLAGS) $(UNITTEST_LDFLAGS)

.PHONY: clean all test
clean:
	rm -f $(bin) *.o *.d
	rm out -r
	rm test/$(unittest)

-include $(OBJS:.o=.d)