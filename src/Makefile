TARGET = $(BIN_DIR)/myprog
OBJ_DIR = ../obj
BIN_DIR = ../bin
LIB_DIR = ../lib
INC_DIR = ../include
SRC_DIR = .
OBJS = $(OBJ_DIR)/Modelo.o \
	   $(OBJ_DIR)/memComp.o 


CFLAGS = -Wall -I$(INC_DIR)
LIB_NAME = memComp.a
LIB = $(LIB_DIR)/$(LIB_NAME)


$(TARGET) : $(OBJS)
	mkdir -p $(BIN_DIR)
	@gcc $(CFLAGS) $(OBJS) $(LIB) -o $(TARGET) -lm

mavlink : mavlinkTX.o
	@gcc $(CFLAGS) mavlinkTX.o $(LIB) -o mavlink -lm

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	gcc -c -MD $(CFLAGS) $< -o $@


-include $(OBJ_DIR)*.d

.PHONY: clean
clean:
	rm -r $(OBJ_DIR) $(BIN_DIR) 

