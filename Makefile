all:
	gcc -c signal_generator.c
	gcc -o converter data_2_sc16q11.c -I. signal_generator.o
	
senoidal:
	gcc -o senoide senoide.c

bin2hex:
	gcc -o bin2hex bin2hex.c -I.

