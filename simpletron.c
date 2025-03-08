#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


void decorador(char * mensagem)
{
    printf("*** %s ***\n", mensagem);
}

void boas_vindas()
{
    decorador("Bem vindo ao Simpletron!");
    decorador("Favor digitar seu programa, uma instrução");
    decorador("(ou palavra de dados) por vez.");
    decorador("Mostrarei o número do local e uma interrogação (?).");
    decorador("Você, então, deverá digitar a palavra para esse local.");
    decorador("Digite a sentinela -9999 para encerrar a entrada do seu programa.");
}


int main()
{
    setlocale(LC_ALL, "Portuguese");

    boas_vindas();


    int memory[100]; // A memória do Simpletron.
    int accumulator = 0; // O acumulador do Simpletron.

    int instructionCounter = 0; // Registra o local que estará na memória.

    int operationCode = 0; // Operação que está sendo executada.
    int operand = 0; // Local da memória da palavra de instrução.

    int Register = 0; // Registrador da memória.



    return 0;
}
