#if ! defined( teste_modulo_instr )
#define teste_modulo_instr

// #ifdef teste_modulo_inst_OWN
//    #define teste_modulor_inst_EXT
// #else
//    #define teste_modulo__instr_EXT extern
// #endif

int SomarInstr( int numSoma , int erro ) ;

#if !defined( teste_modulo_instr_OWN )
   #define  Somar  SomarInstr
#endif

// #undef teste_modulo_instr_EXT

#endif
