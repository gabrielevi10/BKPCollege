#include <stdio.h>
#include <assert.h>


#include "teste_modulo.h"

#define teste_modulo_instr_OWN
   #include "teste_modulo_instr.h"
#undef  teste_modulo_instr_OWN


int SomarInstr( int numSoma , int erro ) 
{
   int res = -1 ; 

   printf( "\nVerificar assertiva de entrada\n" ) ;
   assert( numSoma > 1 ) ;

   res = Somar( numSoma , erro ) ;

   printf( "\nVerificar assertiva de saida\n" ) ;
   assert( res == numSoma * ( numSoma + 1 ) / 2 ) ;

   return res ;

}

