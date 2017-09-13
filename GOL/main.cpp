#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

#include "Jogodavida.hpp"
#include "Cells.hpp"

using namespace std;

int main(){

	Jogodavida jogo;
	jogo.menu();
	jogo.coleta();
	jogo.print();
	int i = 0, j = 0;
	for(int l = 0; l < 1400; l++){
		for(i=0; i < 30;i++){
			for(j=0; j < 60; j++){
				jogo.regras(i,j);
			}			
		}
		getchar();
		system("clear");
		jogo.AttField();	
		jogo.print();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
	return 0;

}