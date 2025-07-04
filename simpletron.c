#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <math.h>


#define MEMORY_SIZE 100
#define SENTINELA -99999
#define MIN_NUMBER -9999
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

typedef struct simpletron
{
    int memory[MEMORY_SIZE]; // A mem�ria do Simpletron.
    int accumulator; // O acumulador do Simpletron.

    int instructionCounter; // Registra o local que estar� na mem�ria.
    int instructionRegister; // Registrador da mem�ria.

    int operationCode; // Opera��o que est� sendo executada.
    int operand; // Local da mem�ria da palavra de instru��o.

} Simpletron;

Simpletron criarSimpletron()
{
    Simpletron novoSimpletron;

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        novoSimpletron.memory[i] = 0;
    }

    novoSimpletron.accumulator = 0;

    novoSimpletron.instructionCounter = 0;
    novoSimpletron.instructionRegister = 0;

    novoSimpletron.operationCode = 0;
    novoSimpletron.operand = 0;

    return novoSimpletron;
}

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
    decorador("Digite a sentinela -99999 para encerrar a entrada do seu programa.");
}

bool estaNosLimites(int valor)
{
    return (valor >= MIN_NUMBER && valor <= MAX_NUMBER);
}

bool instrucaoValida(int instrucao)
{
    if (!estaNosLimites(instrucao) && instrucao != SENTINELA)
    {
        printf("*** Instru��o Inv�lida: %+04d ***\n", instrucao);

        return false;
    }

    return true;
}

void dump(Simpletron * simpletron)
{
    printf("\nREGISTERS:\n");

    printf("accumulator              %+05d\n", simpletron->accumulator);
    printf("instructionCounter          %02d\n", simpletron->instructionCounter);
    printf("instructionRegister      %+05d\n", simpletron->instructionRegister);
    printf("operationCode               %02d\n", simpletron->operationCode);
    printf("operand                     %02d\n", simpletron->operand);

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

            printf(" %+05d ", simpletron->memory[i+j]);
        }
        printf("\n");
    }

    printf("\n");
}

bool errosFatais(int codigoErro, Simpletron * simpletron)
{
    dump(simpletron); // Dump da mem�ria.

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

bool operacaoInvalida(Simpletron * simpletron, int operationCode, int operand)
{
    switch(operationCode)
    {
        case READ:
            decorador("Digite um n�mero");

            fflush(stdin); // Limpa o buffer

            scanf("%d", &simpletron->memory[operand]);

            if (!estaNosLimites(simpletron->memory[operand]))
            {
                return errosFatais(INTERVAL_ERROR, simpletron);
            }

            printf("\n");
        break;

        case WRITE:
            decorador("Resultado");

            printf("%d", simpletron->memory[operand]);

            printf("\n");
        break;


        case LOAD:
            simpletron->accumulator = simpletron->memory[operand];
        break;

        case STORE:
            simpletron->memory[operand] = simpletron->accumulator;
        break;


        case ADD:
            simpletron->accumulator += simpletron->memory[operand];

            if (!estaNosLimites(simpletron->accumulator))
            {
                return errosFatais(ADD_ERROR, simpletron);
            }
        break;

        case SUBTRACT:
            simpletron->accumulator -= simpletron->memory[operand];

            if(!estaNosLimites(simpletron->accumulator))
            {
                return errosFatais(SUBTRACT_ERROR, simpletron);
            }
        break;

        case DIVIDE:
            if (simpletron->memory[operand] != 0)
            {
                simpletron->accumulator /= simpletron->memory[operand];

                if (!estaNosLimites(simpletron->memory[operand]))
                {
                    return errosFatais(DIVIDE_ERROR, simpletron);
                }

                return false;
            }

            return errosFatais(DIVIDE_ZERO, simpletron);
        break;

        case MULTIPLY:
            simpletron->accumulator *= simpletron->memory[operand];

            if(!estaNosLimites(simpletron->accumulator))
            {
                return errosFatais(MULTIPLY_ERROR, simpletron);
            }
        break;

        case EXPONENTIATION:
            simpletron->accumulator = pow(simpletron->accumulator, simpletron->memory[operand]);

            if (!estaNosLimites(simpletron->accumulator))
            {
                return errosFatais(EXPONENTIATION_ERROR, simpletron);
            }
        break;

        case MODULO:
            if (simpletron->memory[operand] != 0)
            {
                simpletron->accumulator = simpletron->accumulator % simpletron->memory[operand];

                if (!estaNosLimites(simpletron->memory[operand]))
                {
                    return errosFatais(MODULO_ERROR, simpletron);
                }
            }

            return errosFatais(DIVIDE_ZERO, simpletron);
        break;


        case BRANCH:
            simpletron->instructionCounter = operand;

            return false;
        break;

        case BRANCHNEG:
            if (simpletron->accumulator < 0)
            {
                simpletron->instructionCounter = operand;

                return false;
            }

        break;

        case BRANCHZERO:
            if (simpletron->accumulator == 0)
            {
                simpletron->instructionCounter = operand;

                return false;
            }
        break;

        case HALT:
            dump(simpletron); // Dump da mem�ria.
            /*
                Apesar desta opera��o ser v�lida,
                retornaremos como se fosse inv�lida
                para sairmos do loop de execu��o.
            */
            return true;
        break;

        default:
            return errosFatais(OPERAND_CODE_ERROR, simpletron);
        break;
    }

    simpletron->instructionCounter++; // Pr�xima instru��o.

    return false; // A opera��o � v�lida.
}

bool lerPrograma(const char * PROGRAMA_PATH, Simpletron * simpletron)
{
    FILE * programa;

    programa = fopen(PROGRAMA_PATH, "r");

    if (!programa)
    {
        printf("\nN�o existe tal arquivo ou diret�rio: %s\n",
               PROGRAMA_PATH);

        return false;
    }

    const int INSTRUCTION_LEN = 7;

    char linha[INSTRUCTION_LEN];

    while(fgets(linha, sizeof(linha), programa)
          && simpletron->instructionCounter < MEMORY_SIZE)
    {
        // Procura e remove a quebra de linha.
        linha[strcspn(linha, "\r\n")] = '\0';

        int instrucao = atoi(linha);

        simpletron->instructionRegister = instrucao;

        if (simpletron->instructionRegister != SENTINELA)
        {
            if (!instrucaoValida(instrucao))
            {
                break;
            }


            simpletron->memory[simpletron->instructionCounter++] = simpletron->instructionRegister;
        }
        else
        {
            return true;
        }
    }

    return false; // Caso o programa n�o possua SENTINELA.
}

bool armazenarPrograma(const char * PROGRAMA_PATH, Simpletron * simpletron)
{
    if (PROGRAMA_PATH)
    {
        bool leituraPrograma = lerPrograma(PROGRAMA_PATH, simpletron);

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
            printf("%02d ? ", simpletron->instructionCounter);

            fflush(stdin); // Limpa o buffer.

            // Pede a instru��o de novo em caso de erro.
            if ((scanf("%d", &simpletron->instructionRegister) == false) ||
                (instrucaoValida(simpletron->instructionRegister) == false)) continue;

            // Armazena a instru��o.
            if (simpletron->instructionRegister != SENTINELA)
            {
                simpletron->memory[simpletron->instructionCounter++] = simpletron->instructionRegister;
            }
        }while(simpletron->instructionRegister != SENTINELA && simpletron->instructionCounter < MEMORY_SIZE);
    }

    simpletron->instructionRegister = 0;
    simpletron->instructionCounter = 0;

    return true;
}

void executarPrograma(Simpletron * simpletron)
{
    do
    {
        simpletron->instructionRegister = simpletron->memory[simpletron->instructionCounter];

        simpletron->operationCode = simpletron->instructionRegister / 100;
        simpletron->operand = simpletron->instructionRegister % 100;

    }while(operacaoInvalida(simpletron, simpletron->operationCode, simpletron->operand) == false
           && simpletron->instructionCounter < MEMORY_SIZE);


    if (simpletron->operationCode == HALT)
    {
        decorador("Execu��o do Simpletron Encerrada.");
    }
    else
    {
        decorador("Execu��o do Simpletron Encerrada de forma anormal.");
    }

}

void rodarPrograma(const char * PROGRAMA_PATH, Simpletron * simpletron)
{
    if (armazenarPrograma(PROGRAMA_PATH, simpletron))
    {
        executarPrograma(simpletron);
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    boasVindas();

    char arquivoSimpletron[100];

    printf("\nArquivo .simpletron: ");

    scanf("%s", arquivoSimpletron);

    Simpletron simpletron = criarSimpletron();

    if (strcasecmp(arquivoSimpletron, "nao") == 0)
    {
        instrucoesManuais();

        rodarPrograma(NULL, &simpletron);
    }
    else
    {
        rodarPrograma(arquivoSimpletron, &simpletron);
    }


    return 0;
}
