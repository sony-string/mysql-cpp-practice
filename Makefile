CC = g++
CFLAGS = -fPIC -Wall -std=c++23 -g -DDEBUG
LDFLAGS = -Iinclude -Llibs `mysql_config --cflags --libs`

UNITTEST_LDFLAGS = -lmysqlcpp -lcunit -Wl,-rpath,libs

bin = sev
unittest = unittest # exists only for unittest

SRC_DIR = src
OUT_DIR = out

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OUT_DIR)/%.o, $(SRCS))

all: $(bin)

$(OUT_DIR):
	mkdir -p $@

arrange: $(OBJS)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OUT_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(bin): arrange
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

.PHONY: clean all test
clean:
	rm -f $(bin) $(OUT_DIR)/*.o $(OUT_DIR)/*.d
	rm -rf $(OUT_DIR)

-include $(OBJS:.o=.d)