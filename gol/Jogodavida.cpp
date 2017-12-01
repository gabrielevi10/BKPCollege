#include "Jogodavida.hpp"
#include "Cells.hpp"
#include <iostream>


using namespace std;

Jogodavida::Jogodavida(){
	int i, j;


	for (i=0; i<30; i++){
		for (j=0; j<60; j++){
			printcelulas[i][j]= '-';
			celulas[i][j] = cell;
		}
	}

	celulas[15][10].setAlive();
	celulas[15][11].setAlive();	
	celulas[15][12].setAlive();
	celulas[14][12].setAlive();
	celulas[13][11].setAlive();
	
	printcelulas[15][10] = '$';
	printcelulas[15][11] = '$';	
	printcelulas[15][12] = '$';
	printcelulas[14][12] = '$';
	printcelulas[13][11] = '$';

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

void Jogodavida::AttField(){
	int i,j;
	for(int l = 0; l < mustLivei.size(); l++){
		i = mustLivei[l];
		j = mustLivej[l];
		celulas[i][j].setAlive();
		printcelulas[i][j] = '$';
	}
	mustLivei.clear();
	mustLivej.clear();
	for(int k = 0; k < mustDiei.size(); k++){
		i = mustDiei[k];
		j = mustDiej[k];
		celulas[i][j].setKill();
		printcelulas[i][j] = '-';		
	}
	mustDiei.clear();
	mustDiej.clear();
}

void Jogodavida::regras(int i, int j){
	int aux = 0;
	
	int previ,prevj,posi,posj;
	previ = i-1;
	posi = i+1;
	prevj = j-1;
	posj = j+1;

	if(i == 0) previ = 29;
	else if(i == 29) posi = 0;

	if(j == 0) prevj = 59;
	else if(j == 59) posj = 0;

	if(celulas[previ][prevj].getCells()) aux++; 
	if(celulas[previ][j].getCells()) aux++;
	if(celulas[previ][posj].getCells()) aux++;
	if(celulas[i][posj].getCells()) aux++;
	if(celulas[i][prevj].getCells()) aux++;
	if(celulas[posi][prevj].getCells()) aux++;
	if(celulas[posi][j].getCells()) aux++;
	if(celulas[posi][posj].getCells()) aux++;
	
	if(celulas[i][j].getCells()){
		if(aux < 2 || aux > 3){
			mustDiei.push_back(i);
			mustDiej.push_back(j);
		}
	}
	else{
		if(aux == 3) {
			mustLivei.push_back(i);
			mustLivej.push_back(j);	
		}	
	}
}