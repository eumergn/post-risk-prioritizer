CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lncurses 
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Create directories if not exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard include/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

EXEC = $(BIN_DIR)/risk_manager

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean files
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

# Clean files and directories
clean_all: clean
	rm -rf $(BIN_DIR) $(OBJ_DIR)



# ----------- Test units ------------
TEST_SRC = tests/test_tree.c
TEST_OBJ = $(OBJ_DIR)/test_tree.o
TEST_EXEC = $(BIN_DIR)/test_tree

$(TEST_OBJ): $(TEST_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_EXEC): $(TEST_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(TEST_EXEC)
	$(TEST_EXEC)

# Valgrind
valgrind: $(TEST_EXEC)
	valgrind --leak-check=full $(TEST_EXEC)
