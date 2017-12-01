#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;


class Disciplines{
	public:
		std::string cod;
		std::string name;
		std::string req;
		int peso;

		std::string getName(){
			return this->name;
		}

		std::string getCod(){
			return this->cod;
		}

		std::string getReq(){
			return this->req;
		}

		int getPeso(){
			return this->peso;
		}
};

class Graph{
	public:
		
		std::vector<std::string> txt_lines;
		std::vector<Disciplines> Adj_list;
		Disciplines discipline;

		Graph(){ //construtor do grafo, inicia a lista de adjacencia a partir do arquivo txt.
			std::fstream pFile;
			std::string str_aux, cod_mk, req_mk, p_mk, name_mk;
			int BarCount;

			pFile.open("disciplinas.txt");

			if(pFile.is_open()){
				while(!pFile.eof()){
					std::getline(pFile, str_aux);
					txt_lines.push_back(str_aux);
				}
				pFile.close();
			}
			for(std::string l : txt_lines){
				BarCount = 0;
				name_mk.clear();
				cod_mk.clear();
				req_mk.clear();
				p_mk.clear();
				for(int k = 0; k < l.size(); k++){
					if(l[k] == '|'){
						BarCount ++;
					}
					if(BarCount == 0 && l[k] != '|' ){
						cod_mk.push_back(l[k]); 	
					}
					if(BarCount == 1 && l[k] != '|' ){
						name_mk.push_back(l[k]);
					}
					if(BarCount == 2 && l[k] != '|' ){
						p_mk.push_back(l[k]); 	
					}
					if(BarCount == 3 && l[k] != '|'){
						req_mk.push_back(l[k]);
					}
				}
				discipline.cod = cod_mk;
				discipline.req = req_mk;
				discipline.peso = stoi(p_mk);
				discipline.name = name_mk;
				Adj_list.push_back(discipline);
			}
		}

		void printGraph(){
			for(std::string l : txt_lines){
				cout << l << endl;
			}
		}
};


int main(){
	Graph disciplines;
	
	
	return 98;
}