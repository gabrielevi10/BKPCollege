#include <stdio.h>
#include "bdd-for-c.h"
#include "Exercicio.c"


    spec("Test Suite") {

        describe("Teste Data"){
            context("Caso feliz: Testando data"){
                it("Testando data correta"){
                    char data1[] = "09/01/1998";
                    check(ChecaData(data1));
                }
                it("Testando mes correto"){    
                    check(ChecaData("23/07/1998"));
                }
                it("Testando dia correto"){
                    check(ChecaData("11/05/1992"));
                }
                it("Testando ano correto"){
                    check(ChecaData("26/01/2000")); 
                }
                it("Testando ano bissexto"){
                    check(ChecaData("29/02/2016"));
                }
            }


            context("Caso triste: Testando data"){
                it("Testando range do mes"){
                        check(!ChecaData("10/13/2016"));
                        check(!ChecaData("02/00/2016"));
                }
                it("Testando range do dia para meses de 31 dias"){
                        check(!ChecaData("32/12/2019"));
                        check(!ChecaData("00/01/2017"));
                }
                it("Testando range do dia para meses de 30 dias"){
                        check(!ChecaData("31/11/2019"));
                        check(!ChecaData("00/04/2017"));
                }
                it("Testando ano maior que o atual"){
                        check(!ChecaData("10/10/20005"));
                }
                it("Testando dia 0") {
                        check(!ChecaData("0/10/2005"));
                }
                it("Testando ano bissexto"){
                        check(!ChecaData("29/02/2015"));
                }
                it("Testando dia negativo") {
                        check(!ChecaData("-5/06/2000"));
                }
                it("Testando dia invalido em fevereiro"){
                        check(!ChecaData("30/02/2010"));
                }
            }
          }//describe do teste de data

            describe("Teste do Nome"){ //nao sei bem o que é pra fazer :(

              context("Caso triste: Testando nome"){

              }

              context("Caso feliz: Testando nome") {
                //
              }
            }//describe caso feliz


    }//Spec file
