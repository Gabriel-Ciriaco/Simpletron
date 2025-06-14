#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <math.h>


#define MEMORY_SIZE 100
#define SENTINELA -9999
#define MAX_NUMBER 9999

/*Opera��es de Entrada/Sa�da*/
#define READ 10
#define WRITE 11

/*Opera��es de Carregamento/Armazenamento*/
#define LOAD 20
#define STORE 21

/*Opera��es Aritm�ticas*/
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define EXPONENTIATION 34
#define MODULO 35

/*Opera��es de Transfer�ncia de Controle*/
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
    MULTIPLY_ERROR,
    EXPONENTIATION_ERROR,
    MODULO_ERROR
};

int memory[MEMORY_SIZE]; // A mem�ria do Simpletron.
int accumulator = 0; // O acumulador do Simpletron.

int instructionCounter = 0; // Registra o local que estar� na mem�ria.
int instructionRegister = 0; // Registrador da mem�ria.

int operationCode = 0; // Opera��o que est� sendo executada.
int operand = 0; // Local da mem�ria da palavra de instru��o.


void decorador(char * mensagem)
{
    printf("*** %s ***\n", mensagem);
}

void boasVindas()
{
    decorador("Bem vindo ao Simpletron!");

    decorador("Caso deseje utilizar um programa .simpletron,");
    decorador("digite o caminho dele a seguir.");
    decorador("Caso contr�rio, digite \"nao\" e pressione ENTER.");
}

void instrucoesManuais()
{
    decorador("Favor digitar seu programa, uma instru��o");
    decorador("(ou palavra de dados) por vez.");
    decorador("Mostrarei o n�mero do local e uma interroga��o (?).");
    decorador("Voc�, ent�o, dever� digitar a palavra para esse local.");
    decorador("Digite a sentinela -9999 para encerrar a entrada do seu programa.");
}

bool estaNosLimites(int valor)
{
    return (valor >= SENTINELA && valor <= MAX_NUMBER);
}

bool instrucaoValida(int instrucao)
{
    if (!estaNosLimites(instrucao))
    {
        printf("*** Instru��o Inv�lida: %+04d ***\n", instrucao);

        return false;
    }

    return true;
}

bool errosFatais(int codigoErro)
{
    dump(); // Dump da mem�ria.

    switch (codigoErro)
    {
        case OPERAND_CODE_ERROR:
            decorador("C�digo de Opera��o inv�lido.");
        break;

        case INTERVAL_ERROR:
            decorador("N�mero fora do intervalo -9999 a +9999.");
        break;

        case ADD_ERROR:
            decorador("A soma no acumulador ultrapassou os limites do registrador.");
        break;

        case SUBTRACT_ERROR:
            decorador("A subtra��o no acumulador ultrapassou os limites do registrador.");
        break;

        case DIVIDE_ERROR:
            decorador("A divis�o no acumulador ultrapassou os limites do registrador.");
        break;

        case DIVIDE_ZERO:
            decorador("Tentativa de divis�o por zero.");
        break;

        case MULTIPLY_ERROR:
            decorador("A multiplica��o no acumulador ultrapassou os limites do registrador.");
        break;

        case EXPONENTIATION_ERROR:
            decorador("A exponencia��o no acumulador ultrapassou os limites do registrador.");
        break;

        case MODULO_ERROR:
            decorador("A divis�o para o c�lculo do modulo ultrapassou os limites do registrador.");
        break;

        default:
            return false; // N�o ocorreu um erro fatal.
        break;
    }

    return true; // Ocorreu um erro fatal.
}

bool operacaoInvalida(int operationCode)
{
    instructionCounter++;

    switch(operationCode)
    {
        case READ:
            decorador("Digite um n�mero");

            fflush(stdin); // Limpa o buffer

            scanf("%d", &memory[operand]);

            if (!estaNosLimites(memory[operand]))
            {
                return errosFatais(INTERVAL_ERROR);
            }

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

            if (!estaNosLimites(accumulator))
            {
                return errosFatais(ADD_ERROR);
            }
        break;

        case SUBTRACT:
            accumulator -= memory[operand];

            if(!estaNosLimites(accumulator))
            {
                return errosFatais(SUBTRACT_ERROR);
            }
        break;

        case DIVIDE:
            if (memory[operand] != 0)
            {
                accumulator /= memory[operand];

                if (!estaNosLimites(memory[operand]))
                {
                    return errosFatais(DIVIDE_ERROR);
                }

                return false;
            }

            return errosFatais(DIVIDE_ZERO);
        break;

        case MULTIPLY:
            accumulator *= memory[operand];

            if(!estaNosLimites(accumulator))
            {
                return errosFatais(MULTIPLY_ERROR);
            }
        break;

        case EXPONENTIATION:
            accumulator = pow(accumulator, memory[operand]);

            if (!estaNosLimites(accumulator))
            {
                return errosFatais(EXPONENTIATION_ERROR);
            }
        break;

        case MODULO:
            if (memory[operand] != 0)
            {
                accumulator = accumulator % memory[operand];

                if (!estaNosLimites(memory[operand]))
                {
                    return errosFatais(MODULO_ERROR);
                }
            }

            return errosFatais(DIVIDE_ZERO);
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
            dump(); // Dump da mem�ria.
            /*
                Apesar desta opera��o ser v�lida,
                retornaremos como se fosse inv�lida
                para sairmos do loop de execu��o.
            */
            return true;
        break;

        default:
            return errosFatais(OPERAND_CODE_ERROR);
        break;
    }

    return false; // A opera��o � v�lida.
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

    /*Imprimir valores da mem�ria*/
    for (int i = 0; i < MEMORY_SIZE; i += quantidade)
    {
        printf("%02d  ", i);
        for (int j = 0; j < quantidade; j++)
        {
            if (i + j >= MEMORY_SIZE) break;

            printf(" %+05d ", memory[i+j]);
        }
        printf("\n");
    }

    printf("\n");
}

bool lerPrograma(const char * PROGRAMA_PATH)
{
    FILE * programa;

    programa = fopen(PROGRAMA_PATH, "r");

    if (!programa)
    {
        printf("\nN�o existe tal arquivo ou diret�rio: %s\n",
               PROGRAMA_PATH);
    }

    const int INSTRUCTION_LEN = 7;

    char linha[INSTRUCTION_LEN];

    while(fgets(linha, sizeof(linha), programa)
          && instructionCounter < MEMORY_SIZE)
    {
        // Procura e remove a quebra de linha.
        linha[strcspn(linha, "\r\n")] = '\0';

        int instrucao = atoi(linha);

        if (!instrucaoValida(instrucao))
        {
            break;
        }

        instructionRegister = instrucao;

        if (instructionRegister != SENTINELA)
        {
            memory[instructionCounter++] = instructionRegister;
        }
        else
        {
            return true;
        }
    }

    return false;
}

bool armazenarPrograma(const char * PROGRAMA_PATH)
{
    if (PROGRAMA_PATH)
    {
        bool leituraPrograma = lerPrograma(PROGRAMA_PATH);

        if (!leituraPrograma)
        {
            decorador("Ocorreu um erro ao armazenar o programa.");

            return false;
        }
    }
    else
    {
        do
        {
            // Pede uma instru��o do usu�rio.
            printf("%02d ? ", instructionCounter);

            fflush(stdin); // Limpa o buffer.

            // Pede a instru��o de novo em caso de erro.
            if ((scanf("%d", &instructionRegister) == false) ||
                (instrucaoValida(instructionRegister) == false)) continue;

            // Armazena a instru��o.
            if (instructionRegister != SENTINELA)
            {
                memory[instructionCounter++] = instructionRegister;
            }
        }while(instructionRegister != SENTINELA && instructionCounter < MEMORY_SIZE);
    }

    instructionRegister = 0;
    instructionCounter = 0;

    return true;
}

void executarPrograma()
{
    do
    {
        instructionRegister = memory[instructionCounter];

        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

    }while(operacaoInvalida(operationCode) == false && instructionCounter < MEMORY_SIZE);


    if (operationCode == HALT)
    {
        decorador("Execu��o do Simpletron Encerrada.");
    }
    else
    {
        decorador("Execu��o do Simpletron Encerrada de forma anormal.");
    }

}

void rodarPrograma(const char * PROGRAMA_PATH)
{
    if (armazenarPrograma(PROGRAMA_PATH))
    {
        executarPrograma();
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    boasVindas();

    char arquivoSimpletron[100];

    printf("\nArquivo .simpletron: ");

    scanf("%s", arquivoSimpletron);

    if (strcasecmp(arquivoSimpletron, "nao") == 0)
    {
        instrucoesManuais();

        rodarPrograma(NULL);
    }
    else
    {
        rodarPrograma(arquivoSimpletron);
    }


    return 0;
}
