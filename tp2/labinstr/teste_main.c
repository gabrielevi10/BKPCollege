/* Este programa ilustra o uso do padr�o
   para a reda��o e uso de m�dulos de instrumenta��o.

   o programa compilado receb dois par�metros de linha
   de comando o 1o. indica o n�mero de n�meros inteiros
   a somar (a soma � caculada pela f�rmula de Gauss)
   o segundo � o erro a ser adicionado de modo que o
   resultado obtido fique diferente do resultado
   esperado.

   Na vers�o sem instrumenta��o (compilado sem -DDEBUG
   dados negativos, erro != 0 n�o s�o controlados

   Na vers�o com instrumenta��o (compilado com -DDEBUG)
   dados negativos, erro != 0 s�o controlados pela
   fun��o assert gerando uma mensagem padr�o da plataforma
   MS C/C++

*/

#include <stdio.h>
#ifndef DEBUG
   #include "teste_modulo.h"
#else
   #include "teste_modulo_instr.h"
#endif

int main( int numParm , char ** vtParm )
{
   int numLidos = -1 ;
   int numSoma  = -1 ; 
   int erro     = -1 ;
   int res      = -1 ;
   if ( numParm != 3 )
   {
      printf( "\nNumero de parametros errado %d" , numParm ) ;
      return 4 ;
   } /* if */
   numLidos = sscanf(  vtParm[ 1 ] , " %d" , &numSoma ) ;
   if ( numLidos != 1 )
   {
      printf( "\nParametro numSoma errado \"%s\"" , vtParm[ 1 ] ) ;
      return 4 ;
   } /* if */
   numLidos = sscanf(  vtParm[ 2 ] , " %d" , &erro ) ;
   if ( numLidos != 1 )
   {
      printf( "\nParametro erro errado \"%s\"" , vtParm[ 2 ] ) ;
      return 4 ;
   } /* if */
   printf( "\nSoma esperada: %d" , numSoma * ( numSoma + 1 ) / 2 ) ;
   res = Somar( numSoma , erro ) ;
   printf( "\nSoma obtida:   %d\n\n" , res ) ;

} /* fim main */
