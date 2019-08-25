all:
	gcc -std=c99 -c signal_generator.c
	gcc -std=c99 -o converter data_2_sc16q11.c -I. signal_generator.o -lm

senoidal:
	gcc -std=c99 -o senoide senoide.c -lm

bin2hex:
	gcc -std=c99 -o bin2hex bin2hex.c -I.

csv2hex:
	gcc -std=c99 -o csv2hex csv2hex.c -I.
