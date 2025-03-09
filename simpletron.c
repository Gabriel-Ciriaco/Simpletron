#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


/*Operações de Entrada/Saída*/
#define READ 10
#define WRITE 11

/*Operações de Carregamento/Armazenamento*/
#define LOAD 20
#define STORE 21

/*Operações Aritméticas*/
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33

/*Operações de Transferência de Controle*/
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43


int memory[100]; // A memória do Simpletron.
int accumulator = 0; // O acumulador do Simpletron.
int instructionCounter = 0; // Registra o local que estará na memória.

int operationCode = 0; // Operação que está sendo executada.
int operand = 0; // Local da memória da palavra de instrução.

int instructionRegister = 0; // Registrador da memória.


void decorador(char * mensagem)
{
    printf("*** %s ***\n", mensagem);
}

void boasVindas()
{
    decorador("Bem vindo ao Simpletron!");
    decorador("Favor digitar seu programa, uma instrução");
    decorador("(ou palavra de dados) por vez.");
    decorador("Mostrarei o número do local e uma interrogação (?).");
    decorador("Você, então, deverá digitar a palavra para esse local.");
    decorador("Digite a sentinela -9999 para encerrar a entrada do seu programa.");
}

void verificarOperacao(int operationCode)
{
    switch(operationCode)
    {
        case READ:
            scanf("%d", &memory[operand]);
        break;

        case WRITE:
            printf("%d", memory[operand]);
        break;


        case LOAD:
            accumulator = memory[operand];
        break;

        case STORE:
            memory[operand] = accumulator;
        break;


        case ADD:
            accumulator += memory[operand];
        break;

        case SUBTRACT:
            accumulator -= memory[operand];
        break;

        case DIVIDE:
            if (memory[operand] != 0)
            {
                accumulator /= memory[operand];
            }
        break;

        case MULTIPLY:
            accumulator *= memory[operand];
        break;


        case BRANCH:
            instructionCounter = operand;
        break;

        case BRANCHNEG:
            if (accumulator < 0)
            {
                instructionCounter = operand;
            }
        break;

        case BRANCHZERO:
            if (accumulator == 0)
            {
                instructionCounter = operand;
            }
        break;
    }

    // Se não for uma Operação de Controle
    if (operationCode < BRANCH)
    {
        ++instructionCounter;
    }
}


int main()
{
    setlocale(LC_ALL, "Portuguese");

    boasVindas();


    instructionRegister = memory[instructionCounter];

    operationCode = instructionRegister / 100;
    operand = instructionRegister % 100;

    decorador("Execução do Simpletron Encerrada");

    return 0;
}
