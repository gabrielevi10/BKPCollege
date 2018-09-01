/*Universidade de Brasilia 
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmo e Programacao de Computadores 1/2016
Aluno(a): Gabriel Levi
Versao do Compilador: gcc (Ubuntu 5.3.1-14ubuntu2.1) 5.3.1 20160413

Descricao: Jogo EliminaLetras - parte 3,no qual era pedido para implementar o rankig do jogo mostrando as dez maiores pontuaçoes
com a possibilidade do usuario excluir o ranking.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

typedef struct {
	char nick[21];
	int score;
} Player;

/*variaveis globais*/
char caracteres[11]; 			/* caracteres para preencher a matriz do jogo */
int linhas, colunas; 			/* dimensoes da matriz */
float base_pontos;				/* base para a pontuacao no jogo */
char matriz[9][10];				/*matriz de jogo*/
int pontos;						/*pontos do jogador*/
int elementos;
int eliminadas;					/*contagem para a parada do loop*/
Player player[11];
char nick[21];

/*retirando os warnings de Implicit Declaration*/
void MenuPrincipal();
void Config();
void ChangeScoreBase();
void ChangeChars();
void ChangeRows();
void ChangeCols();
void ValidaChar();
void ValidaRows();
void ValidaCols();
void readSettings();
void writeSettings();

/*Funcao para dar os parametros para a ordenacao do vetor de struct no qsort*/
int compara(const void*a, const void*b) {
	const Player *x = (const Player *) a;
	const Player *y = (const Player *) b;
	if(x->score < y->score) {
		return 1;
	}
	if(y->score < x->score) {
		return -1;
	}
	return 0;
}

/*Funcao que abre o arquivo binario e grava o ranking*/
void registerMatch() {
	FILE *fp;
	int i;
	char NomeArquivo[30] = "EliminaLetras_ranking.bin";
	fp = fopen(NomeArquivo,"rb");
	if(fp == NULL) {
		strcpy(player[0].nick,nick);
		player[0].score = pontos;
		fp = fopen(NomeArquivo,"wb");
		for(i=1;i < 11;i++)	{
			player[i].nick[0] = '\0';
			player[i].score = 0;
		}
	fclose(fp);			
	}
	else{
		player[10].score = pontos;
		strcpy(player[10].nick,nick);
		qsort(player,11,sizeof(Player),&compara);
		fp = fopen(NomeArquivo,"wb");
		fwrite(player,sizeof(player),1,fp);
		fclose(fp);
	}
	printf("erro\n");
}

/*funcao para printar a matriz na tela de jogo*/
void printMatrix() {
	system(CLEAR);
	int x=1,i,y=1;
	printf("    ");
	for(i=0;i < colunas;i++){
		printf("%d",y);
		y++;
	}
	printf("     PONTUACAO: %d",pontos);
	printf("\n");
	printf("    ");
	for(i=0;i < colunas;i++) {
		printf("|");
	}
	printf("\n");
	for (i = 0;i < linhas;i++){
		printf("%d - ",x);
		printf("%s",matriz[i]);
		printf(" - %d\n",x);
		x++; 
	}
	y = 1;
	printf("    ");
	for(i=0;i < colunas;i++) {
		printf("|");
	}
	printf("\n");
	printf("    ");
	for(i=0;i < colunas;i++){
		printf("%d",y);
		y++;
	}
	printf("\n");
}

/*mostra a mensagem de fim de jogo e a pontuacao do jogador*/
void gameover() {
	printf("==================\n");
	printf("=== GAME OVER! ===\n");
	printf("==================");
	printf("\n\n");
	printf("Sua pontuacao: %d",pontos);
	printf("\n\n");
	printf("Aperte <ENTER> para continuar...");
	getchar();
	registerMatch();
	system(CLEAR);
}

/*funcao que junta as colunas*/
void juntaColuna() {
	int i,j,k,l;
	i = linhas-1;
	k=0;
	for(l=0;l<colunas;l++){	
		for(j=0;j < colunas;j++) {
			if(matriz[i][j] == ' ' && j > 0) {
				for(k=0;k < linhas;k++) {
					matriz[k][j] = matriz[k][j-1];
					matriz[k][j-1] = ' ';
			   	}
			}
		}
	}
}
			
/*funcao que desce os elementos na matriz*/
void downElements() { 
	int i,j,k;
	for(k=0;k < linhas;k++) {
		for(i=0;i < linhas;i++) {
			for(j=0;j < colunas;j++){
				if(matriz[i][j] == ' ' && i != 0){
					matriz[i][j] = matriz[i-1][j];
					matriz[i-1][j] = ' ';
				}
			}
		}
	}
}

/*eecuta o movimento*/
void execMove() { 
	int i,j;
	for(i=0;i < linhas;i++){
		for(j=0;j < colunas;j++) {
			if(matriz[i][j] == '*') {
				matriz[i][j] = ' ';
				eliminadas++;
			}
		}
	}
	downElements();
	juntaColuna();
}

/*desmarca os elementos*/
void unmarkArea(char alvo) {
	int i,j;
	for(i=0;i < linhas;i++){
		for(j=0;j < colunas;j++) {
			if(matriz[i][j] == '*') {
				matriz[i][j] = alvo;
			}
		}
	}
}

/*verifica se o jogador deseja fazer o movimento*/
void VerifJogada(char alvo) {
	int marcados = 0;
	char opcao;
	marcados = (int)ceil(pow(base_pontos, elementos));
	printMatrix();
	printf("\nPontuacao do movimento: %d\n",marcados);
	printf("\n");
	printf("Confirmar movimento ? (S/N): ");
	scanf(" %c",&opcao);
	getchar();
	if(opcao == 'S' || opcao == 's') {
		pontos += marcados;
		execMove();
	}
	else if(opcao == 'N' || opcao == 'n') {
		marcados = 0;
		unmarkArea(alvo);
	}
	else{
		printf("Escolha sim ou nao !!\n");
		VerifJogada(alvo);
	}
}

/*funcao que marca os elementos iguais*/
void markCell(char alvo,int li,int col){
	elementos++;
	matriz[li][col] = '*';
	if(matriz[li-1][col] == alvo) {
		markCell(alvo,li-1,col);
	}
	if(matriz[li][col-1] == alvo) {
		markCell(alvo,li,col-1);
	} 
	if(matriz[li+1][col] == alvo) {
		markCell(alvo,li+1,col);
	}
	if(matriz[li][col+1] == alvo) {
		markCell(alvo,li,col+1);
	}
}

/*inicializa elementos e chama a funcao de marcar os elementos iguais*/
void markArea(char alvo,int li,int col) {
	elementos = 0;
	markCell(alvo,li,col);
	VerifJogada(alvo);
	printMatrix();
}

/*analisa o movimento feito pelo jogador*/
void readMove() {
	int li,col,alvo;
	printf("Insira o numero da linha (min. 1, max. %d): ",linhas);
	scanf("%d",&li);
	li = li-1;
	printf("\n \n");
	printf("Insira o numero da coluna (min. 1, max %d): ",colunas);
	scanf("%d",&col);
	col = col-1;
	printf("\n \n");
	if((li > linhas-1 || li < 0) || (col > colunas-1 || col < 0)) {
		col = 0;
		li = 0;
		readMove();
	}
	else if(matriz[li][col] == ' '){
		printf("Espaço vazio\n\n");
		readMove();
	}
	else {
		alvo = matriz[li][col];
		markArea(alvo,li,col);
	}
}

/*inicializa as variaveis para um novo jogo e inicia o loop do jogo */
void newplay() {
	while(eliminadas != (linhas*colunas)){	
		printMatrix(matriz);
		readMove();
	}
	system(CLEAR);
	gameover();
}

/*gera os caracteres "aleatorios" na matriz*/
void setupMatrix() {
	int i,j,x;
	srand(time(NULL));
	for (i = 0;i < linhas;i++){
		for(j = 0;j < colunas;j++){
			x = rand()%strlen(caracteres);
			matriz[i][j] = caracteres[x];
		}
		matriz[i][colunas] = '\0';
	}
}

/*funcao para ler o nickname do jogador*/
void nickname() {
	eliminadas = 0;
	pontos = 0;
	nick[0] = '\0';
	printf("Insira um apelido (min. 3, max. 20 caracteres): ");
	scanf("%[^\n]s",nick);
	getchar();
	printf("\n");
	if (strlen(nick) < 3 || strlen(nick) > 20) {
		nickname();
	}
	system(CLEAR);
}

/*Funcao para jogar*/
void Jogar() {
	printf("=====================\n");
	printf("=== EliminaLetras ===\n");
	printf("=====================\n");
	printf("\n");
	nickname();
	readSettings();
	setupMatrix();
	newplay();
}

/*Funcao do Ranking*/
void Ranking() {
	FILE *fp1;
	char NomeArquivo[30] = "EliminaLetras_ranking.bin";
	char op;
	fp1 = fopen(NomeArquivo,"rb");
	int k = 1,i;
	printf("===========================================\n");
	printf("========== EliminaLetras RANKING ==========\n");
	printf("===========================================\n");
	printf("\n");
	if(fp1 == NULL) {
		printf("O Ranking esta vazio !!\n");
		printf("Aperte <ENTER> para continuar ...");
		getchar();
	}
	else{
		fread(player,sizeof(Player),10,fp1);
		for(i=0;i<10;i++) {
			if(player[i].nick[0] != '\0'){
				printf("%d - ",k);
				printf("%s        %d",player[i].nick,player[i].score);
				k++;
			}
		}
		printf("\n");
		printf("Remover o ranking atual ? (S/N) ");
		scanf(" %c",&op);
		getchar();
		if(op == 'S' || op == 's') {
			remove(NomeArquivo);
		}
		else if(op == 'n' || op == 'N') {
		}
		fclose(fp1);
	}
	system(CLEAR);
}

/*Funcao para verificar os valores antes de jogar, para o caso do usuario colocar valores errados no arquivo txt*/
void ValidaTXT() {
	if(strlen(caracteres) < 3 || strlen(caracteres) > 10) {
		strcpy(caracteres,"OZXM");
	}
	if(linhas < 3 || linhas > 9) {
		linhas = 6;
	}
	if(colunas < 3 || colunas > 9) {
		colunas = 9;
	}
	if(base_pontos <= 1.0 || base_pontos > 2.0) {
		base_pontos = 1.5;
	}
	writeSettings();
}

/*Funcao para escrever no arquivo texto*/
void writeSettings() {
	FILE *fp;
	char nome[30] = "EliminaLetras_settings.txt";
	fp = fopen(nome, "w");
	fprintf(fp,"%s\n",caracteres);
	fprintf(fp,"%d\n",linhas);
	fprintf(fp,"%d\n",colunas);
	fprintf(fp,"%f\n",base_pontos);
	fflush(fp);
}

/*Funcao para ler do arquivo texto*/
void readSettings() {
	FILE *fp;
	char nome[30] = "EliminaLetras_settings.txt";
	fp = fopen(nome, "r");
	if(fp != NULL) {
		fscanf(fp,"%s",caracteres);
		fscanf(fp,"%d",&linhas);
		fscanf(fp,"%d",&colunas);
		fscanf(fp,"%f",&base_pontos);
		ValidaTXT();
		fclose(fp);
	}
	else {
		strcpy(caracteres,"OZXM");
		linhas = 6;
		colunas = 9;
		base_pontos = 1.5;
		writeSettings();
	}
}

/*Funcao de saida*/
void Saida() {
	printf("=========================================\n");
	printf("=== Obrigado por jogar EliminaLetras! ===\n");
	printf("=========================================\n");
	printf("\n");
	printf("Aperte <ENTER> para continuar...");
	getchar();
	system(CLEAR);
}

/*Funcao para validar os caracteres escolhidos*/
void ValidaChar(char car[]) {
	int tam;
	tam = strlen(car);
	if(tam > 2 && tam < 11) {
		strcpy(caracteres,car);
		tam = 0;
	}
	else {
		printf("Numero de caracteres invalidos\n");
		printf("\n");
		ChangeChars();
	}
}

/*Funcao para mudar os caracteres usados no jogo*/
void ChangeChars() {
	char car[999],x;
	printf("Insira os novos caracteres (min. 3, max. 10): " );
	scanf("%s",car);
	getchar();
	printf("Confirmar acao? (S/N)  ");
	scanf(" %c",&x);
	while(getchar() != '\n');
	if(x == 'S' || x == 's') {
		ValidaChar(car);
		system(CLEAR);
		writeSettings();
	}
	else if(x == 'N' || x == 'n') {
		system(CLEAR);
	}
	else {
		printf("Responda Sim ou Nao !!!\n");
		ChangeChars();
	}
}

/*Funcao para validar o numero de linhas*/
void ValidaRows(int new_rows) {
	if(new_rows > 2 && new_rows < 10) {
		linhas = new_rows;
	}
	else {
		printf("Numero de linhas invalido\n");
		printf("\n");
		ChangeRows();
	}
}

/*Funcao para mudar o numero de linhas*/
void ChangeRows() {
	int new_rows;
	char x;
	printf("Insira o novo numero de linhas (min. 3, max. 9): ");
	scanf("%d",&new_rows);
	getchar();
	printf("Confirmar acao? (S/N)  ");
	scanf(" %c",&x);
	while(getchar() != '\n');
	if(x == 'S' || x == 's') {
		ValidaRows(new_rows);
		system(CLEAR);
		writeSettings();
	}
	else if(x == 'N' || x == 'n') {
		system(CLEAR);
	}
	else {
		printf("Responda Sim ou Nao !!!\n");
		ChangeRows();
	}	
}

/*Funcao para validar as colunas*/
void ValidaCols( int new_cols) {
	if(new_cols > 2 && new_cols < 10) {
		colunas = new_cols;
	}
	else {
		printf("Numero de colunas invalido\n");
		printf("\n");
		ChangeCols();
	}
}

/*Funcao para mudas as colunas*/
void ChangeCols() {
	int new_cols;
	char x;
	printf("Insira o novo numero de colunas (min. 3, max. 9): ");
	scanf("%d",&new_cols);
	getchar();
	printf("Confirmar acao? (S/N)  ");
	scanf(" %c",&x);
	while(getchar() != '\n');
	if(x == 'S' || x == 's') {
		ValidaCols(new_cols);
		system(CLEAR);
		writeSettings();
	}
	else if(x == 'N' || x == 'n') {
		system(CLEAR);
	}
	else {
		printf("Responda Sim ou Nao !!!\n");
		ChangeCols();
	}	
}
/*Funcao para validar a base para a pontuacao*/
void ValidaScoreBase(float new_score) {
	if(new_score > 1.00000 && new_score <= 2.00000) {
		base_pontos = new_score;
	}
	else {
		printf("Numero de pontuacao invalido\n");
		printf("\n");
		ChangeScoreBase();
	}		
}

/*Funcao para mudar a base de score*/
void ChangeScoreBase() {
	float new_score;
	char x;
	printf("Insira o numero real para pontuacao base (min. 1, max. 2): ");
	scanf("%f",&new_score);
	getchar();
	printf("Confirmar acao? (S/N)  ");
	scanf(" %c",&x);
	while(getchar() != '\n');
	if(x == 'S' || x == 's') {
		ValidaScoreBase(new_score);
		system(CLEAR);
		writeSettings();
	}
	else if(x == 'N' || x == 'n') {
		system(CLEAR);
	}
	else {
		printf("Responda Sim ou Nao !!!\n");
		ChangeScoreBase();
	}	
}

/*Funcao para analisar a opcao escolhida no menu de configuracoes*/
void AnalisaOp(int op) {
	if(op == 1) { /*chama a funcao para mudar os caracteres*/
		ChangeChars();
	}
	if(op == 2) { /*chama a funcao para mudar as linhas*/
		ChangeRows();		
	}
	if(op == 3) { /*chama a funcao para mudar as colunas*/
		ChangeCols();	
	}
	if(op == 4) { /*chama a funcao para mudar a base de pontuacao*/
		ChangeScoreBase();
	}
}

/*Funcao da configuracao*/
void Config() {
	int opcao = 1; /*Inicializando para entrar no loop*/
	readSettings();
	while((opcao < 5)&&(opcao > 0)) {
		printf("============================================\n");
		printf("=== EliminaLetras  MENU DE CONFIGURACOES ===\n");
		printf("============================================\n");
		printf("1 - Mudar caracteres: %s\n",caracteres);
		printf("2 - Mudar linhas: %d\n",linhas);
		printf("3 - Mudar colunas: %d\n",colunas);
		printf("4 - Mudar pontuacao base: %f\n",base_pontos);
		printf("5 - Menu Principal\n");
		printf("\n");
		printf("Escolha uma opcao: ");
		scanf("%d",&opcao);
		getchar();
		printf("\n");
		AnalisaOp(opcao);
		system(CLEAR);
	}
	if(opcao == 5) { /*retorna para o menu principal*/
		system(CLEAR);
	}
	if(opcao > 5) {
		printf("Opcao Invalida!\n");
		printf("Aperte <ENTER> para continuar...");
		getchar();
		getchar();
		system(CLEAR);
		Config();
	}
}

/* Funcao do menu principal*/
void MenuPrincipal() {
	int opcao = 1; /*inicializando para forçar entrada no loop*/
	while((opcao < 4)&&(opcao > 0)) {
		printf("=====================================\n");
		printf("=== EliminaLetras  MENU PRINCIPAL ===\n");
		printf("=====================================\n");
		printf("\n");
		printf("1 - Jogar\n");
		printf("2 - Ranking\n");
		printf("3 - Configuraçoes\n");
		printf("4 - Sair\n");
		printf("\nEscolha uma opcao: ");
		scanf("%d",&opcao);
		getchar();
		if(opcao == 1) {
			system(CLEAR);
			Jogar();
		}
		if(opcao == 2) {
			system(CLEAR);
			Ranking();
		}
		if(opcao == 3) {
			system(CLEAR);
			Config();
		}
	}
	if(opcao == 4){
		system(CLEAR);
	}
	else {
		printf("Opcao invalida!\n");
		printf("Aperte <ENTER> para continuar...");
		getchar();
		system(CLEAR);
		MenuPrincipal();
	}
}

/* Funçao para msg inicial*/
void msginicio() {
	printf("==================================\n");
	printf("=== Bem-vindo a EliminaLetras! ===\n");
	printf("==================================\n");
	printf("\n");
	printf("Aperte <ENTER> para continuar...");
	getchar();
	system(CLEAR);
} 

int main () {
	system(CLEAR);
	readSettings();					/*chama a funcao para verificar os valores no txt*/
	msginicio();					/*Chama a mensagem do inicio*/
	MenuPrincipal();				/*Chama o Menu Principal*/
	Saida();						/*chama a funcao de saida, mostra na tela a msg e retorna*/
	return 0;
}
