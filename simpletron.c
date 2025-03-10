#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <math.h>


#define MEMORY_SIZE 100
#define SENTINELA -9999
#define MAX_NUMBER 9999

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

/*Erros Fatais*/
enum Errors {
    OPERAND_CODE_ERROR = 1,
    INTERVAL_ERROR,
    ADD_ERROR,
    SUBTRACT_ERROR,
    DIVIDE_ERROR,
    DIVIDE_ZERO,
    MULTIPLY_ERROR
};

typedef struct simpletron {
    int memory[MEMORY_SIZE]; // A memória do Simpletron.
    int accumulator; // O acumulador do Simpletron.
    int instructionCounter; // Registra o local que estará na memória.

    int operationCode; // Operação que está sendo executada.
    int operand; // Local da memória da palavra de instrução.

    int instructionRegister; // Registrador da memória.
} Simpletron;


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


bool instrucaoValida(int instrucao)
{
    if (instrucao < SENTINELA || instrucao > MAX_NUMBER)
    {
        printf("*** Instrução Inválida: %+04d***\n", instrucao);

        return false;
    }

    return true;

}

bool errosFatais(int codigoErro)
{
    dump(); // Dump da memória.

    switch (codigoErro)
    {
        case OPERAND_CODE_ERROR:
            decorador("Código de Operação inválido.");
        break;

        case INTERVAL_ERROR:
            decorador("Número fora do intervalo -9999 a +9999.");
        break;

        case ADD_ERROR:
            decorador("A soma no acumulador ultrapassou os limites do registrador.");
        break;

        case SUBTRACT_ERROR:
            decorador("A subtração no acumulador ultrapassou os limites do registrador.");
        break;

        case DIVIDE_ERROR:
            decorador("A divisão no acumulador ultrapassou os limites do registrador.");
        break;

        case DIVIDE_ZERO:
            decorador("Tentativa de divisão por zero.");
        break;

        case MULTIPLY_ERROR:
            decorador("A multiplicação no acumulador ultrapassou os limites do registrador.");
        break;

        default:
            return false;
        break;
    }

    return true;
}

bool verificarOperacao(int operationCode, Simpletron * simp)
{
    int * memory = &(simp->memory);
    int operand = simp->operand;

    simp->instructionCounter++;

    switch(operationCode)
    {
        case READ:
            decorador("Digite um número");

            fflush(stdin); // Limpa o buffer

            scanf("%d", &(simp->memory)[operand]);

            if (!instrucaoValida(simp->memory[operand]))
            {
                return !errosFatais(INTERVAL_ERROR);
            }

            printf("\n");
        break;

        case WRITE:
            decorador("Resultado");

            printf("%d", memory[operand]);

            printf("\n");
        break;


        case LOAD:
            simp->accumulator = memory[operand];
        break;

        case STORE:
            memory[operand] = simp->accumulator;
        break;


        case ADD:
            simp->accumulator += memory[operand];

            if (!instrucaoValida(simp->accumulator))
            {
                return !errosFatais(ADD_ERROR);
            }
        break;

        case SUBTRACT:
            simp->accumulator -= memory[operand];

            if(!instrucaoValida(simp->accumulator))
            {
                return !errosFatais(SUBTRACT_ERROR);
            }
        break;

        case DIVIDE:
            if (memory[operand] != 0)
            {
                simp->accumulator /= memory[operand];
                return true;
            }
            else if (!instrucaoValida(memory[operand]))
            {
                return !errosFatais(DIVIDE_ERROR);
            }
            else{
                return !errosFatais(DIVIDE_ZERO);
            }
        break;

        case MULTIPLY:
            simp->accumulator *= memory[operand];

            if(!instrucaoValida(simp->accumulator))
            {
                return !errosFatais(MULTIPLY_ERROR);
            }
        break;


        case BRANCH:
            simp->instructionCounter = operand;
        break;

        case BRANCHNEG:
            if (simp->accumulator < 0)
            {
                simp->instructionCounter = operand;
            }
        break;

        case BRANCHZERO:
            if (simp->accumulator == 0)
            {
                simp->instructionCounter = operand;
            }
        break;

        case HALT:
            dump(); // Dump da memória.
            return false;
        break;

        default:
            return !errosFatais(OPERAND_CODE_ERROR);
        break;
    }

    return true;
}

void dump(Simpletron * simp)
{
    printf("\nREGISTERS:\n");

    printf("accumulator              %+05d\n", simp->accumulator);
    printf("instructionCounter          %02d\n", simp->instructionCounter);
    printf("instructionRegister      %+05d\n", simp->instructionRegister);
    printf("operationCode               %02d\n", simp->operationCode);
    printf("operand                     %02d\n", simp->operand);

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
            printf(" %+05d ", simp->memory[i+j]);
        }
        printf("\n");
    }

    printf("\n");
}

void armazenarPrograma(Simpletron * simp)
{
    do
    {
        // Pede uma instrução do usuário.
        printf("%02d ? ", simp->instructionCounter);

        fflush(stdin); // Limpa o buffer.

        // Pede a instrução de novo em caso de erro.
        if ((scanf("%d", &simp->instructionRegister) == false) ||
            (instrucaoValida(simp->instructionRegister) == false)) continue;

        // Armazena a instrução.
        if (simp->instructionRegister != SENTINELA)
        {
            simp->memory[simp->instructionCounter++] = simp->instructionRegister;
        }
    }while(simp->instructionRegister != SENTINELA && simp->instructionCounter < MEMORY_SIZE);

    simp->instructionRegister = 0;
    simp->instructionCounter = 0;
}

void executarPrograma(Simpletron * simp)
{
    do
    {
        simp->instructionRegister = simp->memory[simp->instructionCounter];

        simp->operationCode = simp->instructionRegister / 100;
        simp->operand = simp->instructionRegister % 100;

    }while(verificarOperacao(simp->operationCode, simp) == true && simp->instructionCounter < MEMORY_SIZE);


    if (simp->operationCode == HALT)
    {
        decorador("Execução do Simpletron Encerrada.");
    }
    else
    {
        decorador("Execução do Simpletron Encerrada de forma anormal.");
    }

}


int main()
{
    setlocale(LC_ALL, "Portuguese");

    Simpletron simp;
    boasVindas();

    armazenarPrograma(&simp);

    executarPrograma(&simp);

    return 0;
}
