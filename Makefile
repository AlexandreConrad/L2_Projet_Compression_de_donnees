CC = gcc
EXEC = program
CFLAGS = -W -Wall
OBJ = obj/

.PHONY: release clean

release: $(OBJ)main.o
	$(CC) $^ -g -o $(EXEC) $(CFLAGS)

$(OBJ)%.o: %.c
	$(CC) -c $< -g -o $@ $(CFLAGS)

clean:
	rm -Rf $(OBJ)*.o $(EXEC)
