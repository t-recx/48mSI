game: game.c
	gcc -Wall game.c -o game `allegro-config --libs`

clean:
	rm -f game
