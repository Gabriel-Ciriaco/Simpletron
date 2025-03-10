#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>


#define MEMORY_SIZE 100
#define SENTINELA -9999
#define MAX_NUMBER 9999

#define TRUE 1
#define FALSE 0

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


int memory[MEMORY_SIZE]; // A memória do Simpletron.
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
    instructionCounter++;
    switch(operationCode)
    {
        case READ:
            decorador("Digite um número");

            fflush(stdin); // Limpa o buffer

            scanf("%d", &memory[operand]);

            printf("\n");
        break;

        case WRITE:
            decorador("Resultado");
            printf("%d", memory[operand]);
            printf("\n");
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

        case HALT:
            dump();
            printf("\n");
            decorador("Execução do Simpletron Encerrada");
        break;
    }
}

void dump()
{
    printf("\nREGISTERS:\n");

    printf("accumulator              %+05d\n", accumulator);
    printf("instructionCounter          %02d\n", instructionCounter);
    printf("instructionRegister      %+05d\n", instructionRegister);
    printf("operationCode               %02d\n", operationCode);
    printf("operand                     %02d\n", operand);

    printf("\nMEMORY:\n\n");

    int quantidade = (int) sqrt(MEMORY_SIZE);

    /*Imprimir linha*/
    printf("         ");
    for (int i = 0; i < quantidade; i++)
    {
        printf("%d      ", i);
    }
    printf("\n");

    /*Imprimir valores da memória*/
    for (int i = 0; i < MEMORY_SIZE; i += quantidade)
    {
        printf("%02d  ", i);
        for (int j = 0; j < quantidade; j++)
        {
            printf(" %+05d ", memory[i+j]);
        }
        printf("\n");
    }
}

int handleInput(int instruction)
{
    if ((instruction < SENTINELA || instruction > MAX_NUMBER))
    {
        return TRUE;
    }

    return FALSE;
}

void armazenarPrograma()
{
    do
    {
        // Pede uma instrução do usuário.
        printf("%02d ? ", instructionCounter);

        fflush(stdin); // Limpa o buffer.

        // Pede a instrução de novo em caso de erro.
        if ((scanf("%d", &instructionRegister) == FALSE) ||
            (handleInput(instructionRegister) == TRUE)) continue;

        // Armazena a instrução.
        memory[instructionCounter++] = instructionRegister;
    }while(instructionRegister != SENTINELA && instructionCounter < MEMORY_SIZE);

    instructionRegister = 0;
    instructionCounter = 0;
}

void executarPrograma()
{
    do
    {
        instructionRegister = memory[instructionCounter];

        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

        verificarOperacao(operationCode);
    }while(operationCode != HALT && instructionCounter < MEMORY_SIZE);
}


int main()
{
    setlocale(LC_ALL, "Portuguese");

    boasVindas();

    armazenarPrograma();

    executarPrograma();

    return 0;
}
