#if ! defined( teste_modulo_ )
#define teste_modulo_

 #ifdef teste_modulo_OWN
   #define teste_modulo_EXT
 #else
   #define teste_modulo_EXT extern
 #endif

int Somar( int numSoma , int erro ) ;

// #undef teste_modulo_EXT

#endif
