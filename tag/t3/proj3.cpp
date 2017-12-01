/*
Universidade de Brasília - UnB
Teoria e Aplicação de Grafos 117901 - CIC 
Alunos: Gabriel Levi - 16/0006490 e Léo Moraes - 16/0011795 
2/2017
link do repositorio : https://github.com/leooleo/Graph-project3
compile with g++ -std=c++11 proj3.cpp 
*/

#include <bits/stdc++.h>
#define debug cout
#define gc getchar()

using namespace std;

class Professor {
public:
	vector<int> escolhas;
	//escolhas que o professor fez.
	int habilitation = -1;
	//indica a habilitação do professor.
    bool employed = false;
    //indica se está empregado ou não.
    int choice = -1;    
    //choice é a escolha que o professor fará.
	int this_priority = 0; 
	//this_priority é posição que o professor está buscando
	//dentre suas escolhas.
    //quanto menor for, mais o professor quer aquela escola.



	Professor() = default;
	explicit Professor(vector<int> vec,int hab,bool f,int c){
		//construtor da classe
		escolhas = std::move(vec);
		habilitation = hab;
        employed = f;
        choice = c;
		this_priority = 0;

	}

	void levetor(){
		//DEBUG APENAS
		cout << employed << '-' << choice << ' ' << habilitation << ": ";
		for (int escolha : escolhas) {
			cout << escolha << ' ';
		}
		cout << endl;
	}

};

class School{

public:
	int choiceA;
	int choiceB;
	int habilitation;

	School() = default;
	explicit School(int hab){
		choiceA = -1;
		choiceB = -1;
		habilitation = hab;
	}
};
array<Professor,100> professores;
//Array de todos os professores contendo habilitação, lista de opções
// um booleano que indica se o prof. está empregado ou não e a escolha do mesmo

array<vector<int>,3> habilitations;
//lista de quais professores se encaixam em cada habilitação
// p.e. os de 3 hab entraram na lista de 3,2,1, os de 2 em 2,1 e os de 1 em 1.

array<School,50> schools;
//schools contém as duas escolhas de cada escola e a habilitação que esta busca

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

void read_file(){
	ifstream file;
	string line;
	std::vector<string> all_lines;
	std::vector<string> splited_line;
	file.open("entradaProj3TAG.txt");
	if (file.is_open()){
		while ( getline (file,line) ){
	      all_lines.push_back(line);
	    }
	}
	//all_lines é um vetor que contém todas as linhas do arquivo
	else{
		cout << "File didnt open.\n";
		//caso o arquivo não tenha sido encontrado, notifique.
	}
    file.close();


    for(int i = 0; i < 100 ; i ++){
    	splited_line = split(all_lines[i],',');
    	//separa as linhas a partir da vírgula separadora.
    	splited_line[0].erase(0, 1); 
    	//retira primeiro elemento(caracter inútil)
    	int hab = stoi(splited_line[1]);
    	//hab recebe a habilitação lida
    	std::vector<int> aux;
    	//aux recebe as esolhas lidas
    	aux.empty();
    	for(int j = 2 ; j < splited_line.size() ; j++){
    		splited_line[j].erase(0, 1); //retira primeiro elemento
    		int school_index = stoi(splited_line[j]);
    		school_index--;
    		aux.push_back(school_index);
    	}
		professores[i] = *new Professor(aux,hab,false,-1); 
		//constroi novo professor no vetor
        switch(hab){
            case 3:
                habilitations[2].push_back(i);
                habilitations[1].push_back(i);
                habilitations[0].push_back(i);
                break;
            case 2:
                habilitations[1].push_back(i);
                habilitations[0].push_back(i);
                break;
            case 1:
                habilitations[0].push_back(i);
                break;
            default:
                break;

        }        
    }

	int j = 0;

    for(int i = 100; i < 150 ; i ++){
    	splited_line = split(all_lines[i],',');
		int hab_choice = stoi(splited_line[1]);
		schools[j] = *new School(hab_choice);
		//Constrói nova escola.
		j++;
    }
}

int exist_unemployed(){
	//retorna primeira posição encontrada de um desempregado
    int i = 0;
    for(auto prof : professores){
        if(!prof.employed){
            return i;
        	//caso o professor encontrado não seja empregado retorne sua posição
        }
        i += 1;
    }
    return -1;
}

bool try_assign(int professor, int school){
	//função para realizar contrato entre professor e escola desejada.
	//retorna true se contrato foi realizado com sucesso, false caso contrário
	if(schools[school].choiceA == -1){
		//caso a primeira escolha esteja disponível...
		professores[professor].choice = school;
		professores[professor].employed = true;
		schools[school].choiceA = professor;
		
		return true;
	}
	if(schools[school].choiceB == -1){
		//caso a segunda escolha esteja disponível...
		professores[professor].choice = school;
		professores[professor].employed = true;
		schools[school].choiceB = professor;
		
		return true;
	}
	else{
		return false;
		//se nenhuma estiver disponível retorne falso
	}

}

/*
Algoritmo implementado com base no SPA-Student encontrado no artigo
disponibilizado no moodle da disciplina: Two Algorithms for the
Student-Project Allocation Problem, David J. Abraham, Robert W. Irving, and David F. Manlove.
*/
void professors_optimized_match(){
	int problema = 0;
	int first_unemployed;
	int first_choice;
	bool success;


	first_unemployed = exist_unemployed();
	while (first_unemployed != -1 && !professores[first_unemployed].escolhas.empty()){
		//enquanto houver um desempregado...
		int priority_index = professores[first_unemployed].this_priority;
		//recebe a prioridade com a qual está sendo lidada.
		//ou seja o quanto este professor quer esta escolha.
		first_choice = professores[first_unemployed].escolhas[priority_index];
		//first choice recebe o índice da escolha.
		

		if(professores[first_unemployed].habilitation < schools[first_choice].habilitation){
			//caso sua habilitação não seja suficiente, passe para próxima escolha
			problema++;
			professores[first_unemployed].this_priority++;

		}

		success = try_assign(first_unemployed,first_choice);

		if(!success){
			
			//caso o contrato não tenha sido estabelecido...
			int a_prof = schools[first_choice].choiceA;
			auto vet = professores[a_prof].escolhas;
			auto aux = find(vet.begin(), vet.end(),first_choice);			
			auto a_priority = distance(vet.begin(),aux);
			
			//procura qual a prioridade da primeira escolha para fins de comparação			
			int b_prof = schools[first_choice].choiceB;
			vet = professores[b_prof].escolhas;
			aux = find(vet.begin(), vet.end(),first_choice);
			auto b_priority = distance(vet.begin(),aux);
			
			//procura qual a prioridade da segunda escolha para fins de comparação
			if(priority_index >= a_priority && priority_index >= b_priority){
				//Se as duas escolhas forem de maior prioridade sobre o elemento em questão,
				//passe para sua próxima escolha.
				
				professores[first_unemployed].this_priority++;
				if(professores[first_unemployed].this_priority > 4){
					//caso todas as prioridades tenham esgotado, 
					//considere o professor como empregado para 
					//depois ser realocado
					professores[first_unemployed].employed = true;
					

				}				
			}
			else if(priority_index < a_priority){
				//Se o elemento em questão querer mais a escola que a primeira opção desta
				//Realize contrato com o elemento mais desejado e desempregue o outro
				
				professores[a_prof].choice = -1;
				professores[a_prof].employed = false;
				schools[first_choice].choiceA = -1;
				//professores[a_prof].this_priority = 0;
				bool auxx = try_assign(first_unemployed,first_choice);
				if(!auxx){
					
					gc;
				}
			}
			else if(priority_index < b_priority){
				//Se o elemento em questão querer mais a escola que a segunda opção desta
				//Realize contrato com o elemento mais desejado e desempregue o outro
				
				professores[b_prof].choice = -1;
				professores[b_prof].employed = false;
				schools[first_choice].choiceB = -1;
				bool auxx = try_assign(first_unemployed,first_choice);
				if(!auxx){
					
					gc;
				}
			}


		}

		first_unemployed = exist_unemployed();
		//passa para o próximo desempregado
	}

	int i = 0;
	//loop para realocar os professores que tiveram suas escolhas esgotadas.
    for(auto prof: professores){
        if(prof.choice == -1){
            for(int j=0; j < schools.size(); j++){
                if(schools[j].choiceA == -1){
                	//busca posição livre
                    
                    schools[j].choiceA = i;
                    professores[i].choice = j;
                    break;
                }
                else if(schools[j].choiceB == -1){
                	//busca posição livre
                    
                    schools[j].choiceB = i;
                    professores[i].choice = j;
                    break;
                }
            }
        }
        i++;
    }
}

/*
Algoritmo Gale-Shapley que leva em conta o desejo do professor e o atual
empregado na escola, buscando estabilidade.
Implementado com base no algoritmo encontrado em : https://en.wikipedia.org/wiki/Stable_marriage_problem
*/
void GaleShapley(){
	int unemployed = exist_unemployed();
	int index_school;
	int count;
	std::vector<int> desempregados_no_fim;

	while(unemployed != -1){
		
		index_school = -1;
		count = 0;
		for(int e : professores[unemployed].escolhas){
			count++;
			if(e != -1){
				index_school = e;
				break;
			}
			if(count == 5){
				professores[unemployed].employed = true;
				desempregados_no_fim.push_back(unemployed);
			}	
		}

		if(index_school != -1 && !professores[unemployed].employed){
			if(schools[index_school].choiceA == -1){
				schools[index_school].choiceA = unemployed;
				professores[unemployed].employed = true;
				professores[unemployed].choice = index_school;
			}
			else if(schools[index_school].choiceB == -1){
				schools[index_school].choiceB = unemployed;
				professores[unemployed].employed = true;
				professores[unemployed].choice = index_school;	
			}
			else{
				if(professores[schools[index_school].choiceA].habilitation != schools[index_school].habilitation
								   && professores[unemployed].habilitation == schools[index_school].habilitation){

					professores[schools[index_school].choiceA].employed = false;

					for(int x = 0; x < professores[schools[index_school].choiceA].escolhas.size(); x++){
						if(professores[schools[index_school].choiceA].escolhas[x] == index_school){
							professores[schools[index_school].choiceA].escolhas[x] = -1;
							professores[schools[index_school].choiceA].choice = -1;
							break;
						}
					}	

					schools[index_school].choiceA = unemployed;
					professores[unemployed].employed = true;
					professores[unemployed].choice = index_school;
				}

				else if(professores[schools[index_school].choiceB].habilitation != schools[index_school].habilitation
								   && professores[unemployed].habilitation == schools[index_school].habilitation){

					professores[schools[index_school].choiceB].employed = false;

					for(int x = 0; x < professores[schools[index_school].choiceB].escolhas.size(); x++){
						if(professores[schools[index_school].choiceB].escolhas[x] == index_school){
							professores[schools[index_school].choiceB].escolhas[x] = -1;
							professores[schools[index_school].choiceB].choice = -1;
							break;
						}
					}

					schools[index_school].choiceB = unemployed;
					professores[unemployed].employed = true;
					professores[unemployed].choice = index_school;
				}

				else{
					for(int x = 0; x < professores[unemployed].escolhas.size(); x++){
						if(professores[unemployed].escolhas[x] == index_school){
							professores[unemployed].escolhas[x] = -1;
							break;
						}	
					}
				}
			}
		}
		unemployed = exist_unemployed();
	}
	
	int i = 0;
	//loop para realocar os professores que tiveram suas escolhas esgotadas.
	for(auto prof: professores){
        if(prof.choice == -1){
            for(int j=0; j < schools.size(); j++){
                if(schools[j].choiceA == -1){
                	//busca posição livre
                    schools[j].choiceA = i;
                    professores[i].choice = j;
                    break;
                }
                else if(schools[j].choiceB == -1){
                	//busca posição livre
                    schools[j].choiceB = i;
                    professores[i].choice = j;
                    break;
                }
            }
        }
        i++;
	}
}


void menu(){
    int choice;

    cout << "==========MENU=========\n";
    cout << "Opção 1- Otimização para professores(SPA-Student) \n";
    cout << "Opção 2- Algoritmo Gale-Shapley \n\n";
    cout << "Digite sua opção: \n";

    cin >> choice;

    switch (choice) {
        case 1:
            professors_optimized_match();
            break;
        case 2:
            GaleShapley();
            break;
        default:
            cout << "Opção inválida, por favor digite novamente\n";
            menu();
            break;
    }
}

int main(){
	
	read_file();
    menu();   
     int i = 1;
     for(Professor p : professores){
        cout << "Professor " << i << " empregado na Escola " <<  p.choice+1 << endl;
        i++;    
    }
 	return 0;
} 