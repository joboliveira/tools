all:
	gcc -c signal_generator.c -std=c99
	gcc -o converter data_2_sc16q11.c -std=c99 -I. signal_generator.o -lm

senoidal:
	gcc -o senoide senoide.c -std=c99

bin2hex:
	gcc -o bin2hex bin2hex.c -I. -std=c99

csv2hex:
	gcc -o csv2hex csv2hex.c -I. -std=c99
