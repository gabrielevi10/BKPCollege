#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int ChecaData( char *data ){
	char dia[3], mes[3], ano[5];
	int tamanho, i, diaInt, mesInt, anoInt;
	tamanho = strlen( data );
	if (tamanho != 10 ) return 0;
	if ( data[2] != '/' || data[5] != '/') return 0;
	for ( i =0; i < 10; i++){
		if( i !=2 && i != 5 ){
			if ( data[i] != '0' && data[i] != '1' && data[i] != '2' && data[i] != '3' && data[i] != '4' && data[i] != '5' && data[i] != '6' && data[i] != '7' && data[i] != '8' && data[i] != '9'){
				return 0;
			}
		}
		if (i < 4) {
			ano[i] = data[i+6];
			if (i < 2) {
				dia[i] = data[i];
				mes[i] = data[i+3];
			}
		}
	}
	char TrintaDias[] = "04,06,09,11";	
	TrintaDias[4] = '\0'; 
	ano[4] = '\0';
	dia[2] = '\0';
	mes[2] = '\0';
	anoInt = atoi( ano );
	diaInt = atoi( dia );
	mesInt = atoi( mes );
	if ( anoInt < 1900 ) return 0;
	if ( diaInt > 31 ) return 0;
	if ( mesInt > 12 ) return 0;
	if ( mesInt < 1	 ) return 0;
	if ( diaInt < 1  ) return 0;
	if ( diaInt == 31 && (strstr(mes, TrintaDias) == NULL)) return 0;
	if ( diaInt == 29 && mesInt == 2 && !((anoInt % 4 == 0 && anoInt % 100 != 0 ) || anoInt % 400 == 0)) 
  		return 0; // verifica se o ano é bissexto
  	if ( mesInt == 2 && diaInt > 29) return 0;
	return 1;
}

void Nome( char *nome, char **primeiro, char **segundo ){
	int tamanho, i, tamanho1 = 0, tamanho2 = 0, flag = 1;
	tamanho = strlen ( nome );
	for ( i = 0; i < tamanho; i++){
		if ( flag == 1 )tamanho1++;
		else if( flag == 0) tamanho2++;
		if ( nome[i] == ' ') flag--;
	}
	(*primeiro) = calloc ( tamanho1, sizeof(char));
	(*segundo) = calloc ( tamanho2, sizeof(char));
	flag =1;
	for( i= 0; i< tamanho1; i++){
		(*primeiro)[i] = nome[i];
	}
	(*primeiro)[i] = '\0';
	for( i= 0; i< tamanho2; i++){
		(*segundo)[i] = nome[i + tamanho1];
	}
	(*segundo)[i] = '\0';
}

// int main(){
// 	char nome1[] = "Carlos Herrique",  nome2[] = "Rodrigo Freitas Santos", nome3[] = "    Er r o   ";
// 	char data1[] = "09/01/1998", data2[] = "32/10/1960", data3[] = "Er/ro/2001", data4[]="10/10/20100", data5[] = "10p05/1906";
// 	char *primeiro, *segundo;
// 	Nome ( nome1, &primeiro, &segundo );
// 	printf( "w%s lol %sw\n", primeiro, segundo);
// 	Nome ( nome2, &primeiro, &segundo );
// 	printf( "w%s lol %sw\n", primeiro, segundo);
// 	Nome ( nome3, &primeiro, &segundo );
// 	printf( "%s lol %s\n", primeiro, segundo);
// 	printf( " %d %d %d %d %d ", ChecaData ( data1 ), ChecaData ( data2 ), ChecaData ( data3 ), ChecaData ( data4 ), ChecaData ( data5 ));
// 	return 0;
// }
