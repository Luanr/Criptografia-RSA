#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct numPrimo {
    int num;
    struct numPrimo *prox;
    struct numPrimo *ante;
};

typedef struct numPrimo Numprimo;

//FUNÇÕES
int geraNumeroMax(int n);
Numprimo *geraPrimoGrande();
int primalidade(int n);
void geraSeed();
void geraLista(Numprimo *head);
void printLista(Numprimo *head);

//OPERAÇÕES
//*FALTA COMEÇAR IMPLEMENTAR AS OPERAÇÕES

int main() {
    Numprimo n1;
    Numprimo *head;

    head = &n1;
    head->num = 0; //Responsavel pelo sinal do numero
    head->prox = NULL;

    geraSeed();
    geraLista(head);
    printLista(head);
    
    return 0;
}

int geraNumeroMax(int n){
    return rand() % n + 1;
}

void geraSeed() { /* gera uma seed para nova sequencia de numeros aleatorios */
    srand((unsigned)time(NULL));
}

int insereLista (Numprimo *head, int num, int sin){
    Numprimo *novo;
    novo = (Numprimo*) malloc(sizeof(Numprimo));
    novo->num = num; //acessa nova e bota o endereço da pessoa1 lá dentro;
    novo->prox = head->prox; //é igual a NULL ESSE PTR_PROX sempre que adicionarmos bem recursivo a parada;
    novo->ante = head;
    head->prox = novo; //ligo o ponteiro prox na celula nova;
    if (sin == 0) { //ESTA SINALIZANDO SE É O PRIMEIRO ELEMENTO OU NAO
        head->ante = novo; //entao ele liga o anterior da cabeça ao ultimo elemento da lista
        sin = 1;
    }
    return sin;
}

void geraLista(Numprimo *head) {
    int i;
    int sin;
    i=0;
    sin=0;
    while (i != 256) {
        sin = insereLista(head, geraNumeroMax(9), sin); //GERANDO NUMEROS DE 1-9 E GUARDANDO NA LISTA
        i++;
    }
}

void printLista(Numprimo *head){
    int i;
    Numprimo *ini = head->prox;
    i=0;
    while(ini != NULL){
        printf("Indice: %d |Num: %d \n", i, ini->num);
        ini = ini->prox;
        printf("____________\n");
        i++;
    }
}



int primalidade(int n){
    int b = n-1;
    int i = 0;
    int a = 0;
    int result;
    while(i <= 100){ /*Rodando 100 vezes para diminuir em 1/2*100 a chance de erro*/
        a = geraNumeroMax(n-1); /*Gerando numero entre 1 e n-1*/
        //result = expModular(a, b, n);
        if (result != 1 % n){ /*Verificando inverso multiplicativo*/
            return 0;
        }
        i++;
    }
    return 1;
}