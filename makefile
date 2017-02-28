# Variables o Macros
EXE=main
OBJS=main.o crc16.o hamming.o
CC=g++
FLAGS=-Wall -g

# Reglas explícitas
# Para linkear todos los objetos y generar un ejecutable
all:$(OBJS)
	$(CC) $(FLAGS)  $(OBJS) -o $(EXE)
	./$(EXE) 

ddd:$(OBJS)
	$(CC) $(FLAGS)  $(OBJS) -o $(EXE)
	rm $(OBJS)
	ddd $(EXE)
# Generara todos los objetos apartir de las reglas implicitas
%.o:%.cpp
	$(CC) $(FLAGS) -c  $<
# Para eliminar .o y el ejecutable
clean:
	rm -rf $(EXE) $(OBJ)
# Reglas implícitas
main.o:main.cpp 
crc16.o:crc16.cpp
hamming.o:hamming.cpp