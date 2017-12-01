all: programa

programa: main.o teste_modulo.o teste_modulo_instr.o
	gcc main.o teste_modulo.o teste_modulo_instr.o -o programa

debug: dmain.o teste_modulo.o teste_modulo_instr.o
	gcc dmain.o teste_modulo.o teste_modulo_instr.o -o Dprograma


main.o: teste_main.c teste_modulo.c teste_modulo.h teste_modulo_instr.c teste_modulo_instr.h
	gcc -c teste_main.c -o main.o

dmain.o: teste_main.c teste_modulo.c teste_modulo.h teste_modulo_instr.c teste_modulo_instr.h
	gcc -c teste_main.c -o dmain.o -DDEBUG

teste_modulo.o: teste_modulo.c teste_modulo.h
	gcc -c teste_modulo.c

teste_modulo_instr.o: teste_modulo_instr.c teste_modulo_instr.h
	gcc -c teste_modulo_instr.c

limpa: 
	rm *.o
