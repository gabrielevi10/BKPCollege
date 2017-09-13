#ifndef JOGODAVIDA_HPP
#define JOGODAVIDA_HPP

#include "Cells.hpp"
#include <vector>

class Jogodavida {

public:
	Jogodavida();

	void menu();
	void coleta();
	void print();
	void regras(int i , int j);
	void AttField();

	std::vector<int> mustLivei, mustDiei, mustLivej, mustDiej;
	char printcelulas[30][60];
	Cells cell;
	Cells celulas[30][60];
};


#endif
