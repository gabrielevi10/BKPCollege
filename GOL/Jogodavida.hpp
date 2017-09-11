#ifndef JOGODAVIDA_HPP
#define JOGODAVIDA_HPP

#include "Cells.hpp"

class Jogodavida {

public:
	Jogodavida();

	void menu();
	void coleta();
	void print();
	void regras(int i , int j);
	
	char printcelulas[30][60];
	Cells cell;
	Cells celulas[30][60];
};


#endif
