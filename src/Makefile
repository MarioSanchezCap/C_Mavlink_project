TARGET_MODELO = $(BIN_DIR)/myprog
TARGET_MAVLINKTX = $(BIN_DIR)/mavlink
OBJ_DIR = ../obj
BIN_DIR = ../bin
LIB_DIR = ../lib
INC_DIR = ../include
SRC_DIR = .
OBJS_MODELO = $(OBJ_DIR)/Modelo.o \
	   		  $(OBJ_DIR)/memComp.o 

OBJS_MAVLINKTX = $(OBJ_DIR)/mavlinkTX.o \
	   		     $(OBJ_DIR)/memComp.o 


CFLAGS = -Wall -I$(INC_DIR)
LIB_NAME = memComp.a
LIB = $(LIB_DIR)/$(LIB_NAME)
PATH_MAVLINK = -I/c_library_v2-master/common

all:  modelo mavlink

modelo : $(OBJS_MODELO)
	mkdir -p $(BIN_DIR)
	@gcc $(CFLAGS) $(OBJS_MODELO) $(LIB) -o $(TARGET_MODELO) -lm

mavlink : $(OBJS_MAVLINKTX)
	@gcc $(CFLAGS) $(PATH_MAVLINK) $(OBJS_MAVLINKTX) $(LIB) -o $(TARGET_MAVLINKTX) -lm

$(OBJ_DIR)/mavlinkTX.o: $(SRC_DIR)/mavlinkTX.c
	gcc -c $(CFLAGS) $(PATH_MAVLINK) $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	gcc -c -MD $(CFLAGS) $< -o $@


-include $(OBJ_DIR)*.d

.PHONY: clean
clean:
	rm -r $(OBJ_DIR) $(BIN_DIR) 

