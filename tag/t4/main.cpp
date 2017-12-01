#include <bits/stdc++.h>

using namespace std;

class Deputy{
//classe que representa um deputado.
public:
    string name,state,party;
    string description,id,date,value;

    explicit Deputy(std::vector<string> input){
        name =input[0];
        state = input[1];
        party = input[2];
        description = input[3];
        id = input[4];
        date = input[5];
        value = input[6];
    }
};
//Representa os gastos de cada deputado
std::map<string, vector<float> > deputy_spent;
std::map<string, string> deputy_filiation;

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
    ifstream file;
    string line;
    std::vector<string> all_lines;
    std::vector<string> splited_line;
    //representa se o deputado lido não tem elementos nulos
    bool is_valid;

    file.open("dirty_deputies_v2.csv");
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

    for (int j =0 ; j < all_lines.size() ; j++) {
        splited_line = split(all_lines[j],',');

        std::vector<string> aux;

        for(int i = 0; i < 8; i++){
            if(i != 6) {
                aux.push_back(splited_line[i]);
            }
        }
        is_valid = true;
        for(const string &atribute: aux){
            if(atribute.empty()){
                is_valid = false;
                continue;
            }
        }

        if(splited_line.size() != 18){
            //verifica validade
            is_valid = false;
            continue;
        }

        if(stof(aux[6]) <= 0 ){
            //verifica validade
            is_valid = false;
            continue;
        }

        if(is_valid) {
            //apenas insira se for válido
            Deputy new_deputy = *new Deputy(aux);
            deputy_spent[new_deputy.name].push_back(stof(new_deputy.value));
            deputy_filiation[new_deputy.name] = new_deputy.party;        
        }
    }
}
//mapa que agrupa por partido
std::map <string, std::vector<pair < string, float >>> comunity_filiation;
//mapa que agrupa quanto cada deputado gastou em total
std::map <string, float> comunity_member_spent;

double entropy(pair<string,vector<float>> suspected_deputy){
    //calcula entropia de um deputado a partir do total gasto por ele
    double total_spent=0;
    double somatory = 0;
    unsigned long size = suspected_deputy.second.size();

    if(size == 1) {
        return 1.0;
    }
    for(float spending : suspected_deputy.second){
        //calcula o total gasto
        total_spent += spending;
    }

    for(float spending : suspected_deputy.second){
        //somatorio da entropia
        double pk = spending/total_spent;
        somatory += pk * log10(1.0/pk);
    }
    auto ret =(somatory/log10(size));
    if(ret > 0.85 && ret < 0.9){
        //entropia que abriga os maiores anômalos(por gasto)
        comunity_member_spent[suspected_deputy.first] = total_spent;
        for(auto x : deputy_filiation){
            static pair <string, float> temp;
            temp.first = {};
            temp.second = 0.0; 
            if(suspected_deputy.first == x.first){
                //agrupa em comunidade
                temp.first = suspected_deputy.first; 
                temp.second = ret;
                comunity_filiation[x.second].push_back(temp);
            }
        }
    }
    return ret;
}


bool cmp(const pair < string, float > &a, const pair < string, float > &b){
    //compara dois deputados por entropia(second)
    return a.second > b.second;
}

void getComunity(){
    for(auto c : comunity_filiation){
        //compara dois deputados e ordena por entropia
        sort(c.second.begin(), c.second.end(), cmp);
        cout << c.first << " : " << endl;
        for(auto x : c.second){
            cout << "    " << x.first << " possui entropia suspeita(" << x.second << ") gastando R$ ";
            cout << comunity_member_spent[x.first] << endl;
        }
    }
}

void suspect(){    
    for (const auto &k : deputy_spent){
        //calcula a entropia de cada deputado
        //verificando se alguma é suspeita
        entropy(k);                
    }    
}

int main(){
    read();
    suspect();
    getComunity();

    return 0;
}