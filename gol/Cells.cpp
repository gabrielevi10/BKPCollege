#include <iostream>
#include "Cells.hpp"

using namespace std;

Cells::Cells(){
 	this->Alive = false;
}

void Cells::setAlive(){
	this->Alive = true;
}

void Cells::setKill(){
	this->Alive = false;
}

bool Cells::getCells(){
	return Alive;
}


