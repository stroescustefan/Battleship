build: battleship.c
	gcc battleship.c -g -Wall -o battleship -lncurses
run: 
	./battleship fisier.txt
clean: battleship
	rm battleship
