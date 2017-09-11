#include "Jogodavida.hpp"
#include "Cells.hpp"
#include <iostream>

using namespace std;

Jogodavida::Jogodavida(){
int i, j;

	for (i=0; i<30; i++)
	{
		for (j=0; j<60; j++){
			printcelulas[i][j]= '-';
			celulas[i][j] = cell;
		}
	}



}

void Jogodavida::menu(){
	cout<< "Aqui vai ficar o menu " << endl;

}

void Jogodavida::coleta(){
	cout << "Aqui vai ficar a coleta" << endl;
}

void Jogodavida::print(){
	int i,j;

	for(i=0; i<30; i++)
	{
		for(j=0;j<60;j++){
		cout<< printcelulas[i][j] << " ";
		}
		cout << endl;
	}
}
void Jogodavida::regras(int i, int j){
	int aux = 0;
	i = i % 30;
	j = j % 60;

	if(celulas[i-1][j-1].getCells()) aux++;
	if(celulas[i-1][j].getCells()) aux++;
	if(celulas[i-1][j+1].getCells()) aux++;
	if(celulas[i][j+1].getCells()) aux++;
	if(celulas[i][j-1].getCells()) aux++;
	if(celulas[i+1][j-1].getCells()) aux++;
	if(celulas[i+1][j].getCells()) aux++;
	if(celulas[i+1][j+1].getCells()) aux++;
	
	if(celulas[i][j].getCells()){
		if(aux < 2) celulas[i][j].setKill();
		if(aux > 3) celulas[i][j].setKill();
	}
	else{
		if(aux == 3) celulas[i][j].setAlive();
	}


}