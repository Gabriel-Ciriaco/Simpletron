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
    decorador("Favor digitar seu programa, uma instru��o");
    decorador("(ou palavra de dados) por vez.");
    decorador("Mostrarei o n�mero do local e uma interroga��o (?).");
    decorador("Voc�, ent�o, dever� digitar a palavra para esse local.");
    decorador("Digite a sentinela -9999 para encerrar a entrada do seu programa.");
}


int main()
{
    setlocale(LC_ALL, "Portuguese");

    boas_vindas();


    int memory[100]; // A mem�ria do Simpletron.
    int accumulator = 0; // O acumulador do Simpletron.

    int instructionCounter = 0; // Registra o local que estar� na mem�ria.

    int operationCode = 0; // Opera��o que est� sendo executada.
    int operand = 0; // Local da mem�ria da palavra de instru��o.

    int Register = 0; // Registrador da mem�ria.



    return 0;
}
