/* 
Universidade de Brasília - UnB
Teoria e Aplicação de Grafos 117901 - CIC 
Alunos: Gabriel Levi - 16/0006490 e Léo Moraes - 16/0011795 
2/2017
link do repositorio : https://github.com/gabrielevi10/graph-project2
*/

#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>

using namespace std;

/*
Classe que representa cada uma das disciplinas na lista de adjacencia, faz o papel de struct.
*/
class Disciplines{
	public:
		std::string name;
		std::string cod;
		int weight;
		std::vector<std::string> list_req;
};

/*
Classe que contem a lista de disciplinas, o construtor ja inicia a lista com nomes, peso e etc
*/
class Graph{
	public:
		
		std::vector<std::string> txt_lines;
		std::vector<Disciplines> Adj_list;
		Disciplines discipline;

		Graph(){ //construtor do grafo, inicia a lista de adjacencia a partir do arquivo txt.
			std::fstream pFile;
			std::string str_aux, cod_mk, req_mk, p_mk, name_mk, str;
			std::vector<std::string> list_aux;
			int BarCount;
			int i = 0;

			pFile.open("disciplines.txt");

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
				list_aux.clear();
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
					if(BarCount >= 3){
						req_mk.push_back(l[k]);
					}
				}
				// salva cada um dos codigos de requisito em um vector, para ser mais facil de utilizar
				for(char s : req_mk){
					if(s != '|'){
						str += s;
					}
					else{
						list_aux.push_back(str);
						str.clear();
					}
				}

				discipline.cod = cod_mk;
				discipline.list_req = list_aux;
				discipline.weight = stoi(p_mk);
				discipline.name = name_mk;

				Adj_list.push_back(discipline);
			}

		}

		void printGraph(){
			cout << "Codigo|Nome|Peso|Pre-requisito de"<< endl << endl;
			for(std::string l : txt_lines){
				cout << l << endl;
			}
		}

};

map<int,int> associa; //associa codigo com posião no vetor;
array<vector<int>,35> adjacentes;

template<typename Out>
void split(const string &s, char delim, Out result) {
	//Função de dividir uma string a partir de um char delimitador.
	//algoritmo retirado do stack overflow
	//link: https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

vector<string> split(const string &s, char delim) {
	//retorna um vetor com as strings delimitadas. 
	//algoritmo retirado do stack overflow
	//referencia: https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

void read(){
	//le o arquivo texto e o prepara para a ordenacao topologica
	ifstream infile; 
	infile.open("disciplines.txt");
	string string_read;
	int i;
	vector<string> all_lines;	//armazena todas as linhas lidas

	while(!infile.eof()){
		//le todas as linhas
		 getline(infile,string_read);
		 all_lines.push_back(string_read);
	}

	i=0;
	for(string c: all_lines){
		//loop que associa no mapa as posições no vetor
		vector<string> splited = split(c,'|');
		associa[stoi(splited[0])] = i;		
		i++;
	}

	i = 0;
	int j;
	//loop usa a funcao split para separar o que foi lido
	//caso tenha separado 4 ou mais elementos implica-se que 
	//existe uma lista de adjacencia nao nula para aquele vertice
	for(int i = 0; i < all_lines.size() ; i++){
		j = 3; 
		vector<string> splited = split(all_lines[i],'|');		
		if(splited.size() < 4){			
			continue;
		}
		else{		
			//ADD na lista de adj se houver algo para tal	
			for(int j = 3; j < splited.size();j++){				
				int aux = stoi(splited[j]);								
				aux = associa[aux];								
				adjacentes[i].push_back(aux);
			}						
		}
	}
}

int dfs_t;					//variavel tempo do dfs
//gradua de branco(não visitado)->cinza(visitando)->preto(terminado)
array<int,35> color;		
array<int,35> parents;		//armazena o pai de cada vertice
array<int,35> dfs_d;		//armazena o tempo de cada vertice
array<int,35> dfs_f;		//armazena o retorno do dfs
std::vector<int> ind_ord;	// auxiliar para usar os indices da ordenacao topologica

void visit_dfs(int u){
	//algoritmo dfs
	color[u] = 1; 		//seta para cinza(visitando)
	//aumenta o tempo
	dfs_t++;


	dfs_d[u] = dfs_t;
	vector<int> adj;
	adj = adjacentes[u];
	
	//para cada adjacente branco
	for(int v : adj){
		if(color[v] == 0){ 
			parents[v] = u;		//seta o pai de u para v
			//recursiva dfs
			visit_dfs(v);
		}
	}
	
	color[u] = 2 ; //seta para preto(terminado)
	dfs_t++;		//passa o tempo	
	dfs_f[u] = dfs_t;	//marca o tempo de u em dfs_f
}

void call_dfs(){

	for(int i=0; i < color.size() ; i++){
		//inicializa as variaveis
		color[i] = 0; 
		parents[i] = -1; 
		dfs_f[i] = 0;
		dfs_t = 0;
	}

	for(int i=0; i < color.size() ; i++){
		if(color[i] == 0){
			//se não tiver sido visitado(cor branca->0)			
			visit_dfs(i);
		}
	}
	
	//mapa associa as posicoes antigas do retorno do dfs
	//antes de seu ordenamento
	map<int,int> aux_map;
	
	for(int i = 0; i < dfs_f.size(); i++){
		//associa ao mapa
		aux_map[dfs_f[i]] = i;
	}

	//ordena o retorno
	sort(dfs_f.begin(), dfs_f.end());
	reverse(dfs_f.begin(), dfs_f.end());

	for(int i = 0; i < dfs_f.size(); i++){
		ind_ord.push_back(aux_map[dfs_f[i]]);
	}

	for(int i = 0; i < adjacentes.size(); i++){
		//garante que o algoritmo nao esta incorreto a partir da definicao de 
		//ordenacao topologica, ou seja, se existe uma aresta de "u" ate "v"
		//no resultado ordenado u deve vir antes de v
		for(int v: adjacentes[i]){
			int pos1 = std::find(dfs_f.begin(), dfs_f.end(), i) - dfs_f.begin();
			int pos2 = std::find(dfs_f.begin(), dfs_f.end(), v) - dfs_f.begin();
			//verifica caso a condicao nao tenha sido atingida
			if(!(i < v)){
				cout << "erro " << i << ' ' <<  v << endl;	
			}
		}
	}	
}
/*
Printa a ordenacao topologica correspondente em ind_ord
*/
void printOT(Graph d){
	cout << endl << "Ordenacao Topologica: " << endl << endl;
	for(int ind : ind_ord){
		cout << d.Adj_list[ind].cod << ' ' << d.Adj_list[ind].name << endl;
	}
}

/*
Funcao utilizada pelo sort para ordenar o vetor auxiliar de pesos acumulados em CPM()
*/
bool cmp(const Disciplines &a, const Disciplines &b){
	return a.critical < b.critical;
}

/*
Detecta o caminho critico a partir de um array contendo os custos para chegar em cada materia,
a partir dai, ordena um vetor auxiliar e utiliza o conceito de pilha para achar o caminho critico
saido da materia com o maior peso acumulado (fundamentos de sistemas operacionais - 44) ate a 
materia sem pre requisito em que se inicia o caminho.
*/
void CPM(Graph d){
	std::vector<std::string> aux;
	std::vector<Disciplines> critical_path;
	array<int, 35> finalize;
	Disciplines actual;
	std::vector<Disciplines> critical_aux; 

	for(int i : ind_ord){
		finalize[i] = 0;
	}
	for(int j : ind_ord){
		for(int k : adjacentes[j]){
			finalize[k] = std::max(finalize[k], finalize[j] + d.Adj_list[k].weight);		
		}
	}

	for(int i=0;i < 35; i++){
		d.Adj_list[i].critical = finalize[i];
		critical_aux.push_back(d.Adj_list[i]);
	}

	sort(critical_aux.begin(), critical_aux.end(), cmp);

	actual = critical_aux[critical_aux.size() - 1];
	critical_aux.pop_back();
	critical_path.push_back(actual);

	while(!critical_aux.empty()){
		actual = critical_aux[critical_aux.size() - 1];

		for(std::string eoq : actual.list_req){
			if(eoq == critical_path[critical_path.size() - 1].cod){
				critical_path.push_back(actual);
				break;
			}
		}
		critical_aux.pop_back();
	}

	cout << endl << "Caminho critico : " << endl << endl;
	for(int aaa = critical_path.size()-1; aaa >= 0; aaa--){
		cout << critical_path[aaa].cod << ' ' << critical_path[aaa].name << endl; 
	}
}

int main(){
	read();
	call_dfs();
	Graph disciplinas;

	cout << "Curso: Ciencia da Computacao" << endl;
	disciplinas.printGraph();
	printOT(disciplinas);
	CPM(disciplinas);

	return 0;
}