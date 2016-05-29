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

struct resultado {
    struct numPrimo *value1;
    struct numPrimo *value2;
    struct numPrimo *value3;
    struct numPrimo *value4;
};

typedef struct numPrimo Numprimo;
typedef struct resultado Resultado;

//FUNÇÕES
int geraNumeroMax(int n);
int numDigitos(Numprimo * a);
int primalidade(int n);
int deslocaEsquerda(Numprimo * a);
int ePar(Numprimo * a);
Numprimo *geraPrimoGrande();
Numprimo * criaLista(int valor);
Numprimo * copia(Numprimo * a);
void geraSeed();
void geraLista(Numprimo *head, int numeros);
void printLista(Numprimo *head);
void insereLista(Numprimo *head, int num);
void pedeEmprestado(Numprimo * a);
void deslocaDireita(Numprimo * a);
void insereZeros(Numprimo *head, int n, int digito);
int numMaior(Numprimo *head1, Numprimo *head2);
void limpaZero(Numprimo *head);
Numprimo *copiaNum(Numprimo *head);
int numIgual(Numprimo *head1, Numprimo *head2);

//OPERAÇÕES
Numprimo * soma(Numprimo *head1, Numprimo *head2);
Numprimo * maior(Numprimo * a, Numprimo * b);
Numprimo * subtrai(Numprimo * a, Numprimo * b);
Numprimo * multiplica(Numprimo *head1, Numprimo *head2);
Numprimo * multiplica2(Numprimo *head1, Numprimo *head2);
Numprimo * exponenciacao(Numprimo * a, Numprimo * b);
Resultado *divide(Numprimo *head1, Numprimo *head2);

int main() {
    Numprimo * head1, * head2, *aux;

    head1 = criaLista(1); // 0 indica o sinal do numero
    head2 = criaLista(1);

    geraSeed();
    geraLista(head1, 2);
    //geraLista(head2, 1);
    insereLista(head2, 4);

    printLista(head1);
    printf("\n");
    //printLista(soma(head1, head2));
    //printf("\n");
    printLista(head2);
    printf("\n\n ");

    printf("Exponenciacao:\n");
    printLista(multiplica(head1, head1) );
    printf("multiplica novo: \n");
    printLista(multiplica2(head1, head1));
    //printLista(soma(head1, head2));
    return 0;
}

//Ressaltando a entrada NUMERO 1 DEVE SER MAIOR OU IGUAL QUE 2
//Desempenho péssimo com numeros que variam muito a quantidade de casas decimais exemplo: 234384734 / 2
//Implementado com o principio da soma iterativa até atingir o numero ou passar
Resultado *divide(Numprimo *head1, Numprimo *head2) {
    /*Entrada: Dois numeros inteiro de n bits head1 e head2, onde head2>= 1
    Saida: O quociente e o resto de head1 dividido por head2
    */
    Resultado *result = (Resultado*) malloc(sizeof(Resultado));
    int i = 1; //Talvez precisa usar um unsigned long long para armazenar
    result->value1 = criaLista(0);
    Numprimo *copia = copiaNum(head2);
    Numprimo *quociente = result->value1;
    Numprimo *resto;
    if(numIgual(head1, head2) == 1) { //Tratando numeros iguais
        insereLista(quociente, 1);
        resto = criaLista(0);
        insereLista(resto, 0);
        result->value2 = resto;
        return result;
    }
    while(numMaior(copia, head1) != 1)  {
        copia = soma(copia, head2); //Somando diversas vezes o quociente com ele mesmo
        i++;
        printLista(copia);
    }
    //Inserindo quociente transformando de inteiro para celula
    while(i != 0){
        int num = i%10;
        i = i / 10;
        insereLista(quociente, num);
    }
    //Numero ultrapassou seu dividendo
    if(numMaior(copia, head1) == 1 && numIgual(copia, head1) != 1) {
        quociente->ante->num -= 1;
        copia = subtrai(copia, head2);
    }
    //Dividendo e divisor são iguais
    if(numIgual(copia, head1) == 1){
        resto = criaLista(0);
        insereLista(resto, 0);
        result->value2 = resto;
        return result;
    }
    resto = subtrai(head1, copia);
    result->value2 = resto;
    return result;
}

Numprimo * multiplica2(Numprimo * a, Numprimo * b) {
    Numprimo * c, * aux;
    int resto, i;

    aux = criaLista(1);

    if(numDigitos(b) == 1) {
        switch(b->prox->num) {
            case 0:
                c = criaLista(1);
                insereLista(c, 0);
                break;
            case 1:
                c = copia(a);
                break;
            case 2:
                c = soma(a, a);
                break;
            default:
                c = soma(a, a);
                for(i = 2; i < b->prox->num; i++) {
                    c = soma(c, a);
                }
                break;
        }
        return c;
    }

    resto = deslocaEsquerda(b);
    c = multiplica(a, b);

    insereLista(aux, resto);
    deslocaDireita(c);

    return soma(c, multiplica(a, aux));
}

// recebe n = quantia de zeros, digito = valor total da mult de n1*n2
// @TODO: poder reaproveitar parte da funcao deslocaDireita que faz a mesma coisa só que uma vez
void insereZeros(Numprimo *head, int n, int digito) {
    int auxNum;
    while(n != 0) { //Inserindo primeiros todos os 0
        insereLista(head, 0);
        n--;
    }
    //Variavel digito sempre tera 2 casas decimais apenas
    auxNum = digito%10;
    digito = (digito - auxNum)/10;
    insereLista(head, auxNum);
    insereLista(head, digito);
}

// Pode dale que o algoritmo tá zika agora
Numprimo * exponenciacao(Numprimo * a, Numprimo * b) {
    Numprimo * c, * aux, * um;
    Numprimo *tmp = copiaNum(a);

    if(numDigitos(b) == 1) {
        switch(b->prox->num) {
            case 0: // retorna um
                c = criaLista(1);
                insereLista(c, 1);
                return c;
            case 1: // retorna uma cópia do próprio a
                return tmp;
            case 2: // retorna multiplica(a, a);
                return multiplica(a, tmp);
        }
    }

    c = multiplica(a, tmp);

    um = criaLista(1);  // o valor desse com o de um auxiliar precisa uma hora ser igual ao expoente
    insereLista(um, 1); // um representa uma lista com o valor igual a 1

    aux = criaLista(1);
    insereLista(aux, 2); // ja fizemos a * a = duas vezes a^2, entao devemos 2

    while(aux->prox->num != b->prox->num) { // quando os expoente auxiliar for igual o expoente real, temos o resultado das multiplicacoes
        c = multiplica(a, c);
        aux = soma(aux, um);
    }
    return c;
}

//Depende da funcao insereZeros
Numprimo *multiplica(Numprimo *head1, Numprimo *head2) {
    Numprimo *aux1 = head1->ante;
    Numprimo *aux2 = head2->ante;
    Numprimo *result = criaLista(1); //recebe o valor da soma total
    Numprimo *tmp = criaLista(1); // recebe num1*num2..seguido de zeros
    Numprimo *exclude;
    int total = 0;
    int qntZeros1 = 0; //CONTANDO A QUANTIA DE ZEROS PARA A SOMA
    int qntZeros2 = 0; //CONTANDO A QUANTIA DE ZEROS PARA A SOMA
    while(aux2 != head2) {
        aux1 = head1->ante; //voltando sempre pro final
        while(aux1 != head1) {
            total = aux2->num*aux1->num;
            tmp = criaLista(1); //Precisa sempre ser renovada
            insereZeros(tmp, qntZeros1+qntZeros2, total); //A multiplicacao recebe uma quantia de zeros = zero1+zero2
            if(result != result->prox) { // SE RESULTE NÃO ESTIVER VAZIA
                result = soma(result, tmp); //Soma vai ser incrementado sempre
            }
            else {
                result = tmp; //Se nao o result sera apenas a primeira multiplicacao
            }
            qntZeros1++;
            aux1 = aux1->ante;
        }
        aux2 = aux2->ante;
        total = 0; //zerando o total
        qntZeros1 = 0; //apenas o qntzeros do numero 1 precisa ser limpo
        qntZeros2++;
    }
    if(result->prox->num == 0) { //Apagando o zero que aparece de vez em quando
        exclude = result->prox;
        result->prox = exclude->prox;
        free(exclude); //liberando a memoria que estava alocada pra aquela celular
    }
    return result;
}

// Recebe um Numprimo e retorna verdadeiro se se o resto da divisao por 2 é zero do último dígito
int ePar(Numprimo * a) {
    return a->ante->num  % 2 == 0 ? 1 : 0;
}

// Recebe um Numprimo a e retorna Numprimo igual com outro endereço
Numprimo * copia(Numprimo * a) {

    Numprimo * nova, * aux;

    aux = a->ante;
    nova = criaLista(a->num); // cria nova cabeça igual a cabeça de a

    while(aux != a) { // percorre do final até o começo
        insereLista(nova, aux->num);
        aux = aux->ante;
    }
    return aux; // devolve a cabeça do numero copiado
}

// Desloca um digito para a esquerda do número, divisão inteira por 10
int deslocaEsquerda(Numprimo * a) {
    Numprimo * aux;
    int numero;

    numero = a->ante->num;
    if(a->prox != a) { // fica  a critério deixar o número 0 ou somente a cabeça para representar um numero vazio
        aux = a->ante;
        aux->ante->prox = a;
        a->ante = aux->ante;
        free(aux);
    }
    if(a->prox == a) {
        insereLista(a, 0);
    }
    return numero;
}

// Desloca um digito para a direita do número, multiplicação por 10
void deslocaDireita(Numprimo * a) {
    if(a->prox->num != 0) { // se o número já é 0 nao adianta tentar colocar outro na frente
        insereLista(a->ante, 0); // finge que o elemento anterior a cabeça é a cabeça e faz a inserção nela
    }
}

Numprimo * soma(Numprimo *head1, Numprimo *head2) {
    Numprimo *headaux1 = head1->ante;
    Numprimo *headaux2 = head2->ante;
    Numprimo *soma = criaLista(1);
    int total = 0;
    int carry = 0; //ARMAZENA O EXCESSO DA SOMA
    int i = 0;
    while (headaux1 != head1 || headaux2 != head2) {

        if(headaux1 != head1 && headaux2 != head2) {
            total = headaux1->num + headaux2->num +carry;
        } else if (headaux1 == head1) {
            total = headaux2->num + carry;
        } else if (headaux2 == head2) {
            total = headaux1->num + carry;
        }


        if (total > 10) { //EXEMPLO 24
            carry = total/10; // CARRY RECEBE 2
            total = total%10; // TOTAL RECEBE 4
            insereLista(soma, total); //INSERE NA LISTA RESULTADO O 4
        }
        else if(total == 10) { //SE FOR TOTAL 10 PRECISAMOS TRATAR
            carry = total - 9; //CARRY RECEBE 1
            total = 0; //TOTAL 0
            insereLista(soma, total); //INSERE 0 NA SOMA
        }
        else {
            carry = 0; //ZERA O CARRY PORQUE JÁ FOI ADICIONADO NA SOMA E NAO EXCEDEU 10
            insereLista(soma, total); //INSERE O TOTAL
        }
        if (headaux1 != head1) {
            headaux1 = headaux1->ante;
        }
        if (headaux2 != head2) {
            headaux2 = headaux2->ante;
        }
    }
    if(carry != 0) {
        insereLista(soma, carry);
    }
    return soma;
}

/* Recebe dois Numprimos e retorna o endereço do maior */
Numprimo * maior(Numprimo * a, Numprimo * b) {
    Numprimo * aux1, * aux2;

    //tratamento numeros com sinais opostos
    if((a->num + b->num) == 0) { // se os dois tem sinais diferentes
        return a->num == 1 ? a : b;
    }

    // é maior o que tem mais digitos já que nao armazenamos zeros

    // tratar se ambos forem negativos
    if(numDigitos(a) > numDigitos(b)) {
        return a;
    }
    if(numDigitos(a) < numDigitos(b)) {
        return b;
    }

    aux1 = a->prox;
    aux2 = b->prox;

    while(aux1->prox != a) { // só entra no looping se possui mesma quantidade de digitos
        if(aux1->num > aux2->num) {
            return a;
        }
        if (aux1->num < aux2->num) {
            return b;
        }
        aux1 = aux1->prox;
        aux2 = aux2->prox;
    }
    return a;
}

/* Recebe um ponteiro para um Numprimo e conta a quantidades de
dígitos doo Numero
 */
int numDigitos(Numprimo * a) {
    Numprimo * aux;
    int i;

    i = 0;
    aux = a->prox;
    while(aux != a) {
        aux = aux->prox;
        i++;
    }
    return i;
}

Numprimo * subtrai(Numprimo * a, Numprimo * b) {
    Numprimo * aux1, * aux2, * c;
    int subtracao;

    // se b > a entao o algoritmo nao funciona, entao mudamos
    // o sinal do número e chamamos a funcao com os argumentos invertidos
    if(maior(a, b) == b) {
        c = subtrai(b, a);
        c->num *= -1; // negativa o resultado, já que b > a
        return c;
    }

    aux1 = a->ante; // elemento antes da cabeca = final
    aux2 = b->ante;
    c = criaLista(1);

    while(aux1 != a || aux2 != b) {
        subtracao = 0;

        if(aux1->num - aux2->num < 0) {
            pedeEmprestado(aux1); // pedimos emprestado para os dígitos anteriores
            subtracao = 10 + aux1->num - aux2->num ; // subtração feita com o empréstimo
        } else {
            subtracao = aux1->num - aux2->num; // não precisa fazer o empréstimo, subtração normal
        }
        insereLista(c, subtracao); // insere na lista o valor subtraido

        // os dois ponteiros percorrerão para a esquerda até encontrar a cabeça da lista
        if(aux1 != a) {
            aux1 = aux1->ante;
        }
        if(aux2 != b) {
            aux2 = aux2->ante;
        }

    }

    //agora uma das duas listas chegou na cabeça, a operação é só o próprio número
    while(aux1 != a) {
        insereLista(c, aux1->num);
        aux1 = aux1->ante;
    }

    while(aux2 != b) {
        insereLista(c, aux2->num);
        aux2 = aux2->ante;
    }
    limpaZero(c); //Estava gerando zeros ao começo do numero na saida
    return c;
}

//Limpa zero do inicio da subtração
void limpaZero(Numprimo *head) {
    Numprimo *exclude;
    while(head->prox->num == 0) {
        exclude = head->prox;
        head->prox = exclude->prox;
        exclude->prox->ante = head;
        free(exclude);
    }
}

//Recebe um numero e devolve um ponteiro novo alocado de uma copia identica
Numprimo *copiaNum(Numprimo *head) {
    Numprimo *aux = head->ante;
    Numprimo *copia = criaLista(0);
    while(aux != head){
        insereLista(copia, aux->num);
        aux = aux->ante;
    }
    return copia;
}


//Função otimizada para comparar se o numero é maior comparando digito a digito
int numMaior(Numprimo *head1, Numprimo *head2) {
    //Numero 1 é maior que 2 somente se todos digitos de 1 é maior que 2
    Numprimo *aux1 = head1->prox;
    Numprimo *aux2 = head2->prox;
    if(numDigitos(head1) > numDigitos(head2)) {
        return 1;
    }
    if(numDigitos(head1) == numDigitos(head2)){
        while(aux1 != head1) {
            if(aux1->num < aux2->num){
                return 0;
            }
            aux1 = aux1->prox;
            aux2 = aux2->prox;
        }
        return 1;
    }
    else{
        return 0;
    }
}

//Verifica se os numeros sao exatamente iguais
int numIgual(Numprimo *head1, Numprimo *head2){
    Numprimo *aux1 = head1->prox;
    Numprimo *aux2 = head2->prox;
    if(numDigitos(head1) != numDigitos(head2)){ //garantindo que a quantia de digito é igual
        return 0;
    }
    while(aux1 != head1) {
        if(aux1->num != aux2->num) {
            return 0;
        }
        aux1 = aux1->prox;
        aux2 = aux2->prox;
    }
    return 1;
}

/* Função utilizada na subtracao de um Numprimo
pede emprestado um numero de uma casa, balanceando o valor
na casa anterior
 */
void pedeEmprestado(Numprimo * a) {
    if((a->ante->num) == 0) {
        a->ante->num = 9;
        pedeEmprestado(a->ante);
    } else {
        a->ante->num -= 1;
    }
}

int geraNumeroMax(int n){
    return rand() % n + 1;
}

void geraSeed() { /* gera uma seed para nova sequencia de numeros aleatorios */
    srand((unsigned)time(NULL));
}

Numprimo * criaLista(int valor) {
    Numprimo * head;
    head = (Numprimo *) malloc(sizeof(Numprimo));
    head->num = valor;
    head->prox = head;
    return head;
}

void insereLista(Numprimo *head, int num){
    Numprimo *novo;
    novo = (Numprimo*) malloc(sizeof(Numprimo));
    novo->num = num; //acessa nova e bota o endereço da pessoa1 lá dentro;

    if(head->prox == head) {
        novo->prox = head;
        head->prox = novo;
        novo->ante = head;
        head->ante = novo;
        return;
    }
    novo->prox = head->prox;
    head->prox = novo;
    novo->prox->ante = novo;
    novo->ante = head;
}

void geraLista(Numprimo *head, int numeros) {
    int i;
    i=0;
    while (i != numeros) { // deve ser colocado 256 digitos
        insereLista(head, geraNumeroMax(9)); //GERANDO NUMEROS DE 1-9 E GUARDANDO NA LISTA
        i++;
    }
}

void printLista(Numprimo *head){
    int i;
    Numprimo *ini;


    ini = head->prox;
    i=0;

    if(head->num == -1) {
        printf("- ");
    }

    while(ini != head){
        printf("%i ", ini->num);
        ini = ini->prox;
        i++;
    }
}

