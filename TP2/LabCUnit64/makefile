all: bdd1 gcovr-html

Exercicio:
	gcc Exercicio_teste.c -o Exercicio_teste
	./Exercicio_teste

bdd1:
	gcc -Wall -ftest-coverage -fprofile-arcs -fPIC -O0 Exercicio_teste.c -o Exercicio_teste
	./Exercicio_teste


gcovr-branch:
	gcovr -r . --branches

gcovr-xml:
	gcovr -r . --xml-pretty

gcovr-html:
	gcovr -r . --html --html-details -o Exercicio.html

limpa :
	rm *.gcda *.gcno *.o *.html Exercicio_teste 
