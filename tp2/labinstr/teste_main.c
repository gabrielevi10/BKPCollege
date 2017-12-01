/* Este programa ilustra o uso do padrão
   para a redação e uso de módulos de instrumentação.

   o programa compilado receb dois parâmetros de linha
   de comando o 1o. indica o número de números inteiros
   a somar (a soma é caculada pela fórmula de Gauss)
   o segundo é o erro a ser adicionado de modo que o
   resultado obtido fique diferente do resultado
   esperado.

   Na versão sem instrumentação (compilado sem -DDEBUG
   dados negativos, erro != 0 não são controlados

   Na versão com instrumentação (compilado com -DDEBUG)
   dados negativos, erro != 0 são controlados pela
   função assert gerando uma mensagem padrão da plataforma
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
