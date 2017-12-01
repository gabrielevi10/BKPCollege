#define teste_modulo_OWN
   #include "teste_modulo.h"
#undef  teste_modulo_OWN

int Somar( int numSoma , int erro ) 
{
   return ( numSoma * ( numSoma + 1 ) / 2  + erro ) ;
} 
