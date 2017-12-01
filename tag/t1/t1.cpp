/* 
Universidade de Brasília - UnB
Teoria e Aplicação de Grafos 117901 - CIC 
Alunos: Gabriel Levi - 16/0006490 e Léo Moraes - 16/0011795 
2/2017
*/

#include <array>
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;
const int max_tam = 49; //TAMANHO MÁXIMO DO GRAFO

void busca_aluno(int no_busca);
void insert_node(int firstnode,int newnode);

class Graph_node {
  public:
  	int vertice;
    void set (int);
};//IMPLEMENTACAO DO NÓ DO GRAFO

void Graph_node::set (int x) {
  	//construtor do nó do grafo
	vertice = x;
}

bool compare(const vector<Graph_node>& a, const vector<Graph_node>& b) {
	/*função auxiliar para a ordenação dos alunos
	por tamanho da lista de adjacência a cada um */
	return a.size() > b.size();
}

bool cmp(const vector<int> &a, const vector<int> &b){
	/*função auxiliar para a ordenação dos maximais
	por tamanho da lista de adjacência a cada um. */
	return a.size() > b.size();
}

class aluno {
	//classe aluno contendo matrícula e nome
	public:
		string matricula,nome;
		void set(string mat,string nome_passado);
};

void aluno::set(string mat,string nome_passado){
	//construtor da classe aluno
	matricula = mat;
	nome = nome_passado;
}

array<aluno,max_tam> vet_alunos; //instancia global de um array de alunos 

void busca_aluno(int no_busca){
	//imprime um aluno a partir de um índice do array de alunos
	aluno c = vet_alunos[no_busca];
	cout << c.matricula << '-';	
	cout << c.nome;
}

class Graph {
	public:
		array<vector<Graph_node>,max_tam> vet;
		long unsigned int nos = 0;
		vector<Graph_node> aux;

		void retira_front(){
			//Retira redundância do primeiro nó.
			//Redundância esta usada para facilitar a ordenação
			//dos alunos a partir do grau de cada um
			for(int i=0; i < max_tam; i++){
				aux = this->vet[i];
				aux.erase(aux.begin());
				this->vet[i] = aux;
			}
		}

		bool vector_contains(vector<Graph_node> vet,int busca){
			//Procura se uma certa lista de adjacência
			//contém um elemento a ser buscado.
			for(Graph_node node : vet){
				if(node.vertice == busca){
					return true;
				}
			}
			return false;
		}

		void printa_grafo(bool if_sort){
			//Função para printar o grafo.
			//Passa como parâmetro um booleano que representa
			//se o grafo deve ser ordenado ou não.

			if(if_sort)
				sort(this->vet.begin(), this->vet.end(), compare);

			for(int i=0; i < max_tam; i++){
				cout << "(" << i+1 << "º): ";
				if(!if_sort){
					for(Graph_node j : this->vet[i]){
						cout << j.vertice << '-';
					}
				}
				else{				
					busca_aluno(this->vet[i].begin()->vertice);					
				}
				if(if_sort){
					cout <<"---Grau: " << this->vet[i].size() -1 <<  endl;	
				}
				else{
					cout <<"---Grau: " << this->vet[i].size() <<  endl;		
				}
				
			}
		}

		void forcabarra(){
			//Força amizades com o fim de tornar o grafo
			//não-direcionado
			for(int i = 0; i < max_tam; i++){
				for(Graph_node c : this->vet[i]){
					int aux = c.vertice;
					if(!vector_contains(vet[aux],i)){
						//Se um vértice não contém o caminho inverso
						//adicione-o.
						insert_node(aux,i);
					}

				}
			}
		}


};//CLASSE DO GRAFO EM SI. CONTEM TAMANHO DE NOS E VERTICES

Graph grafo; //cria uma instancia global do grafo

void insert_node(int firstnode,int newnode){
	//Função de inserção de nós no grafo
	//firstnode é o nó que receberá o 
	//adjacente new node.
	Graph_node no;
	no.set(newnode);
	grafo.vet[firstnode].push_back(no);
	grafo.nos++;
}

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

void read_nodes(){
	//função que le os nós do arquivo texto de entrada.
	int firstnode,newnode;
	int line_counter,string_counter,vet_alunos_counter=0;
	string STRING;
	aluno new_aluno;
	string mat,nome;

	ifstream infile; 
	infile.open("amigos_tag20172.txt");
	
	while(!infile.eof()){
		 getline(infile,STRING);
		 //"STRING" recebe a linha lida
		 
		 vector<string> matricula_e_nome = split(STRING, ',');
		 //divide a string a partir do char ','

		 new_aluno.set(matricula_e_nome[1],matricula_e_nome[2]);
		 vet_alunos[vet_alunos_counter] = new_aluno;
		 vet_alunos_counter++;
		 //adiciona o nome e matrícula lidos ao vetor de alunos

		 if(!STRING.empty()){
		 	//lê os primeiros nós.
		 	if(STRING[1] == ','){
		 		firstnode = STRING[0] - 49;	
		 	}
		 	else{
		 		firstnode = STRING[0] - 48;
		 		firstnode *= 10;
		 		firstnode += STRING[1] - 49;
		 	}

		 	insert_node(firstnode,firstnode);
		 	//Insere a redundância do primeiro nó
		 	//para facilitar a ordenação a partir 
		 	//do grau de cada nó.
		 	if(STRING[1] == ','){
		 		line_counter = 2;	
		 	}
		 	else{
		 		line_counter = 3;
		 	}

		 	while(STRING[line_counter] != ','){
		 		line_counter++;
		 	}

		 	line_counter++;
		 	while(STRING[line_counter] != ',' && line_counter <= STRING.size()){				
		 		line_counter++;
		 	}
		 	line_counter++;

		 	//Line counter é usado para chegar à última vírgula
		 	//antes dos nós adjacentes. 		 	
		 	for(int i = line_counter ; i < STRING.size() ; i++){
		 		//loop que lê e insere os nós adjacentes
		 		int newnode;
		 		if(STRING[i+1] != ',' && i+1 < STRING.size()){
		 			newnode = STRING[i] - 48;
		 			newnode *= 10;
		 			newnode += STRING[i+1] - 49;
		 			i++;
		 		}
		 		else{
		 			newnode = STRING[i] - 49;
		 		}
		 		insert_node(firstnode,newnode);
		 		i++;
		 	}
		 }
	}
	infile.close();
}

vector<vector<int>> Maximais;			
// Vetor que guarda todos os maximais encontrados pelo algoritmo de BronKerborsch.

array<list<int>,max_tam> ListaAdjAux;
// Lista de adjacencia de inteiros auxiliar para utilizacao do algoritmo.	

void BronKerbosch(vector<int> R, vector<int> P, vector<int> X){
	/* Algoritmo de Bron Kerbosch que encontra os maximais no grafo. 
		Cada maximal encontrado eh guardado em um vetor. 
		Algoritmo implementado com base no encontrado no site :
		https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm 
		e nos slides de aula.
	*/
	if(P.size() == 0 && X.size() == 0){
		Maximais.push_back(R);
	
	}
	
	vector<int> Paux = P;
	for(int n : Paux){
		vector<int> interP = {}, interX = {};
		for(int n1 : ListaAdjAux[n]){
			for(int n2 : P){
				if(n1 == n2){
					interP.push_back(n1);
				}
			}
			for(int n3 : X){
				if(n1 == n3){
					interX.push_back(n1);
				}
			}
		}
		vector<int> Rcopy = R;
		Rcopy.push_back(n);
		BronKerbosch(Rcopy,interP,interX);
		P.erase(remove(P.begin(),P.end(),n),P.end());
		X.push_back(n);
	}
}

void PrintMaximais(){
	sort(Maximais.begin(), Maximais.end(), cmp);
	
	cout << endl<< "Maximo: " << endl;
	for(int e : Maximais[0]) {
		busca_aluno(e);
		cout << endl;
	}

	cout << endl << "Maximal diferente do Maximo: " << endl;
	for(int r : Maximais[1]){
		busca_aluno(r);
		cout << endl;	
	}
}

int main(){
	Graph grafo_ordenado; // cria uma instância auxiliar do grafo para ordenação
	read_nodes();
	grafo.forcabarra(); // FORÇA AS AMIZADES ( se a eh amigo de b entao b eh amigo de a )
	int i;

	grafo_ordenado = grafo;
	grafo_ordenado.printa_grafo(true);
	grafo.retira_front();

	for(i = 0; i < 49; i++){
		for(Graph_node nodeAux : grafo.vet[i]){
			ListaAdjAux[i].push_back(nodeAux.vertice);
		}
	}

	vector<int> R = {}, X = {} , P;

	for(i = 0;i < 49; i++){
		P.push_back(i);
	}

	BronKerbosch(R,P,X); //Função que determina os maximais que o grafo possui.

	PrintMaximais();

	return 0;
}
