/*
    Distância da origem em relação a todos os vértices é igual ao
    algoritmo de djisktra executado da origem para todos os vértices

*/

/*-- BIBLIOTECAS --*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*-- ARQUIVO --*/
FILE *arquivo;

/*-- ESTRUTURAS --*/
struct grafo{
    int vertices;
    int *grau;
    int **arestas; //matriz alocada dinamicamente - linhas = vertices
                        // dentro de cada linha quantas conexoes ele tem
    int **peso;
    int **menorCaminho;
};
typedef struct grafo Grafo;

/*-- FUNÇÕES --*/
Grafo* criaGrafo(int quantidadeVertices,int quantidadeArestas);
void linha1(int *quantidadeVertices, int *quantidadeArestas);
void valoresArestas(Grafo *gr,int quantidadeVertices,int quantidadeArestas);
void leArestas(char linha[], int *vertice1,int *vertice2,int *peso);
void dados_dijkstra(Grafo *grafo,int quantidadeArestas);
int function_vizinhos(Grafo *gr, int origem, int *vizinhos);

void main(){//char *nomeArquivo[], int *origem, int *destino){
    Grafo *grafo;
    int quantidadeVertices= 0, quantidadeArestas=0;

    //lerArquivo(nomeArquivo);
    linha1(&quantidadeVertices,&quantidadeArestas);

    grafo = criaGrafo(quantidadeVertices,quantidadeArestas);
    valoresArestas(grafo,quantidadeVertices,quantidadeArestas);
    dados_dijkstra(grafo,quantidadeArestas);
}


Grafo* criaGrafo(int quantidadeVertices,int quantidadeArestas){
    Grafo *grafo = (Grafo*) malloc(sizeof(struct grafo));
    int i=0,j=0;

    if(grafo != NULL){ // se tinha espaço na memória para a alocação
        grafo->vertices = quantidadeVertices;
        grafo->grau = (int*) calloc(quantidadeVertices, sizeof(int));
        grafo->arestas =  (int**)calloc(9900,sizeof(int*));
        for(i=0; i<quantidadeArestas; i++){
                grafo->arestas[i]= (long*)calloc(9900,sizeof(long));
        }
        grafo->peso =  (int**)calloc(9900,sizeof(int*));
        for(i=0; i<quantidadeArestas; i++){
                grafo->peso[i]= (long*)calloc(9900,sizeof(long));
        }
        grafo->menorCaminho =  (int**)calloc(9900,sizeof(int*));
        for(i=0; i<quantidadeArestas; i++){
                grafo->menorCaminho[i]= (long*)calloc(9900,sizeof(long));
        }
    }
    return grafo;
}


//apagavel
void lerArquivo(char *nomeArquivo){
    char linha[20];

    /* abrir arquivo apenas para leitura */
    arquivo = fopen("teste.txt","r");

    /* verifica se o arquivo existe e se pode ser aberto */
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo.\n");
        exit(0);
    }else{
        /* lê o arquivo, ate o final e depois imprime o que tem em cada linha dele */
        while(fgets(linha,sizeof(linha),arquivo) != NULL){
            printf("%s",linha);
        }
    }
    fclose(arquivo);
}

/*
    O arquivo é lido, mas apenas a primeira linha importa, visto que essa
    linha contém a quantidade de vértices e arestas do grafo
*/
void linha1(int *quantidadeVertices, int *quantidadeArestas){

    char stringValores[20],linha[20]; //corresponde a linha lida do arquivo
    int contador=1, i=0,j=0;



    /* abrir arquivo apenas para leitura */
    arquivo = fopen("teste.txt","r");

    if(arquivo == NULL) {
        printf("Erro na abertura do arquivo.\n");
        exit(0);
    }else{
        while (!feof(arquivo)){
            fgets(linha, 20, arquivo);
            if(contador == 1){
               /*
               /   # Parte 1 do Arquivo
               /   Corresponde a quantidade de vértices que o grafo apresenta
              */
                while(linha[i] != ' '){ // enquanto não tiver o espaço entre os numeros
                   stringValores[j++]=linha[i++];
                }
                stringValores[j] = '\0';
                *quantidadeVertices = atoi(stringValores);

                /*
                /   # Parte 2 do Arquivo
                /   Corresponde a quantidade de arestas que o grafo apresenta
               */
                i +=1;
                j=0;
                while(linha[i] != '\0'){ // enquanto não tiver o espaço entre os numeros
                   stringValores[j++]=linha[i++];
                }
                 stringValores[j] = '\0';
                *quantidadeArestas = atoi(stringValores);
            }
            contador++;
        }
    }
    fclose(arquivo);
}

/*
    Separa cada linha do arquivo, em que cada parte corresponde a um dado.

*/
void leArestas(char linha[],int *vertice1,int *vertice2,int *peso){

    int i=0,j=0;
    char stringAresta[20];

    /* Inicializa os valores */
    *vertice1 =0;
    *vertice2 =0;
    *peso = 0;

    /*
    /   # Parte 1 do Arquivo
    /   Corresponde ao valor do vértice 1
    */
    while(linha[i] != ' '){
       stringAresta[j++]=linha[i++];
    }
    stringAresta[j] = '\0'; /* insere o caractere de fim de linha na string */
    *vertice1 = atoi(stringAresta);

    /*
    /   # Parte 2 do Arquivo
    /   Corresponde ao valor do vértice 2
    */
    i +=1;
    j=0;
    while(linha[i] != ' '){
       stringAresta[j++]=linha[i++];
    }

    stringAresta[j] = '\0';
    *vertice2 = atoi(stringAresta);

    /*
    /   # Parte 3 do Arquivo
    /   Corresponde ao valor do peso
    */
    i +=1;
    j=0;
    while(linha[i] != '\0'){ // enquanto não tiver o espaço entre os numeros
       stringAresta[j++]=linha[i++];
    }
     stringAresta[j] = '\0';
    *peso= atoi(stringAresta);
}


void valoresArestas(Grafo *gr,int quantidadeVertices,int quantidadeArestas){

    char linha[20];
    int peso=0, vertice1=0, vertice2=0,contador=0,i=0,j=0;

    /* abrir arquivo apenas para leitura */
    arquivo = fopen("teste.txt","r");

    /* verifica se o arquivo existe e se pode ser aberto */
    if(arquivo == NULL){
        printf("Erro na abertura do arquivo.\n");
        exit(0);
    }else{
        /*
            Enquanto não chegar no fim do arquivo as arestas serão definidas.

        */
        while (!feof(arquivo)){
            fgets(linha, 20, arquivo);
            if(contador >= 1){
                leArestas(linha,&vertice1,&vertice2,&peso);

                gr->arestas[vertice1][vertice2] = 1;
                gr->arestas[vertice2][vertice1] = 1;

                /* Laços */
                if(vertice1 == vertice2){
                     gr->grau[vertice1] = 1;
                }
                else{
                    gr->grau[vertice1]++;
                    gr->grau[vertice2]++;
                }
                gr->peso[vertice1][vertice2] = peso;
                gr->peso[vertice2][vertice1] = peso;
            }
            contador++;
        }
    }
    fclose(arquivo);

    /*
        Arestas que não são conectadas, recebem valor infinito, bem como o peso entre elas.
    */
    for(i=0; i< quantidadeVertices; i++){
        for(j=0; j< quantidadeArestas;j++){
            if(gr->arestas[i][j] != 1 ){
                gr->arestas[i][j] = -1;
                gr->arestas[j][i] = -1;
                gr->peso[i][j] = -1;
                gr->peso[j][i] = -1;
            }
        }
    }
}

void dados_dijkstra(Grafo *grafo, int quantidadeArestas){
    int origem =0, destino =0, ant[grafo->vertices],i=0,j=0,caminho[grafo->vertices];


    /*
      Informar o vértice de origem.
      Será preciso inserir os dados novamente quando o vértice informado for inválido.
   */
    do{
        printf("Informe a origem: ");
        scanf("%d", &origem);

        if(origem >= grafo->vertices || origem < 0) /* vértices inválidos */
            printf("O vértice não existe. Informar novamente!\n");

    }while(origem >= grafo->vertices);

    /*
      Informar o vértice de destino.
      Será preciso inserir os dados novamente quando o vértice informado for inválido.
   */
    do{
        printf("Informe o destino: ");
        scanf("%d", &destino);

        if(destino >= grafo->vertices || destino < 0 ) /* vértices inválidos */
            printf("O vertice nao existe. Informar novamente!\n");

    }while(destino >= grafo->vertices || destino == origem);


    analise_dijkstra(grafo, origem, destino);
}

void analise_dijkstra(Grafo *grafo, int origem, int destino){

    int visitados[grafo->vertices], anterior[grafo->vertices], copiaAnterior[grafo->vertices],custo[grafo->vertices],caminho[grafo->vertices],proxNodo=0;
    int caminhoCerto[grafo->vertices];
    int primeiro = origem;
    int i=0,j=0, menorCusto=0;


    inicializaVetores(grafo,visitados, anterior, custo, caminho, caminhoCerto);
    custo[origem] = 0;
    menorCusto = dijkstra(grafo, origem, destino, &visitados,&custo,&anterior, primeiro,0);
    //printf("[%d] -- [%d] : %d\n",origem, destino,menorCusto);


    for(i=0; i<grafo->vertices; i++){
        copiaAnterior[i] = anterior[i];
    }

    for(i=0; i<grafo->vertices; i++){
        if(primeiro == i){
            //printf("ENTROU\n");
            if(grafo->peso[primeiro][i] == -1){
               // printf("\t>>>> [%3d] --- [%3d]:    0\n ", primeiro, i);
                grafo->menorCaminho[primeiro][i] = 0;
                grafo->menorCaminho[i][primeiro] = 0;
            }
        }else{
            inicializaVetores(grafo,visitados, anterior, custo, caminho, caminhoCerto);
            custo[origem] = 0;
            menorCusto = 0;
            menorCusto = dijkstra(grafo, origem, i,visitados,custo,anterior,primeiro, 1);
            if(menorCusto == 1001 && visitados[i] == -1){
               // printf("\t>>>> [%3d] --- [%3d]: Infinito\n ", primeiro, i);
                grafo->menorCaminho[primeiro][i] = -1;
                grafo->menorCaminho[i][primeiro] = -1;
            }else{
                //printf("\t>>>> [%3d] --- [%3d]: %4d\n ", primeiro, i, menorCusto);
                grafo->menorCaminho[primeiro][i] = menorCusto;
                grafo->menorCaminho[i][primeiro] = menorCusto;
            }
        }
    }

    printf("\n> Execucao Simples <\n");
    if(grafo->menorCaminho[primeiro][destino] == -1){
        printf("  >>> Custo Total  Infinito \n\n");
    }else{
        printf("  >>> Custo Total  %5d \n\n", grafo->menorCaminho[primeiro][destino]);
    }

    printf("\n> Execucao Passo a Passo <\n");
    //printf("grafo->menorCaminho[%d][%d] : %d\n", primeiro,destino,grafo->menorCaminho[primeiro][destino] );
    if(grafo->menorCaminho[primeiro][destino] != -1){
        conjuntoFechado(grafo, caminhoCerto,destino, origem, caminho,copiaAnterior);
        printf("  >>> \n");
        for(i=0; i<grafo->vertices; i++){
        if(caminhoCerto[i] != -1){
            printf(" \t> %4d < dist:\t", caminhoCerto[i]);
            printf(" %4d\n",grafo->menorCaminho[primeiro][caminhoCerto[i]]);
        }
    }
    }else{
        printf("  >>> Conjunto Fechado ");
        printf("Nao tem caminho possivel.\n");
    }
    conjuntoAberto(grafo,primeiro);

}

void inicializaVetores(Grafo *grafo, int *visitados, int *anterior, int *custo, int *caminho, int *caminhoCerto){
    int i=0;
    for(i=0; i< grafo->vertices; i++){
            custo[i] = 1001;       //lista de todos os custos (infinito)
            anterior[i] = -1;       // nenhum nodo é anterior
            visitados[i] = -1;     //ninguém foi visitado
            caminho[i] = -1;     // vetor do caminho de menor custo, que foi percorrido (ordem inversa)
            caminhoCerto[i] = -1;  // vetor do caminho de menor custo, que foi percorrido (ordem correta)
        }
 }

 void conjuntoAberto(Grafo *grafo, int primeiro){
    int i=0;
    printf("  >>> Conjunto Aberto \n");
    for(i=0; i<grafo->vertices; i++){
       if(grafo->menorCaminho[primeiro][i] == -1){
            printf("\t>>>> [%3d] --- [%3d]: Infinito\n ", primeiro, i);
        }else{
            printf("\t>>>> [%3d] --- [%3d]: %4d\n ", primeiro, i, grafo->menorCaminho[primeiro][i]);
        }
    }
 }

void conjuntoFechado(Grafo *grafo,int *caminhoCerto,int destino,int origem, int *caminho, int *anterior){
    int j=0,i=0,proxNodo =0;

    do{
        if(i == destino){
            caminho[j] = destino;
            //printf("caminho[%d]: %d\n", j, caminho[j]);
            proxNodo = anterior[caminho[j]];
            //printf("proxNodo: %d\n", proxNodo);
            j++;

            while(proxNodo != origem){
            caminho[j] = proxNodo;
            proxNodo = anterior[caminho[j]];
            j++;
          }
        }
        i++;
    }while(i <= destino);


    caminhoCerto[0] = origem;
    j=1;
    for(i=grafo->vertices-1; i >= 0; i--){
        if(caminho[i]!= -1){
          caminhoCerto[j] = caminho[i];
          j++;
        }
    }

    printf("  >>> Conjunto Fechado ");
    for(j=0; j<grafo->vertices; j++){
        if(caminhoCerto[j] != -1)
            printf("%5d ",caminhoCerto[j]);
    }
    printf("\n");
}

//ver caso da componente conexa
int dijkstra(Grafo *grafo,int origem,int destino,int *visitados,
             int *custo,int *anterior, int primeiro, int conjAberto){

    int vizinhos[100]; // cada vértice pode ter 100 vizinhos
    int i =0, custoTotal=0,*verticeMenorCusto=0;

    /* Inicializa o vetor dos vizinhos */
    for(i=0; i<100; i++)
        vizinhos[i] = -1;

    visitados[origem] = 1;
    function_vizinhos(grafo,origem,&vizinhos); /* devolve os vizinhos do nodo */

    i=0;
    while(vizinhos[i] != -1 ){
        /* o custo do caminho será o valor que o vertice "origem"
            tem somado com o valor da aresta com o seu vizinho */
        custoTotal = custo[origem] + grafo->peso[origem][vizinhos[i]];

        /* Caso o custoTotal seja menor do que um custo ja definido entao:
            > o custo do vizinho passará a ser o custo total, visto que é o menor valor
            > o anterior desse vizinho passará a ser o nodo anterior, que proporcionou o menor
               caminho até o momento
        */
        if(custoTotal < custo[vizinhos[i]]){
            custo[vizinhos[i]] = custoTotal;
            //printf("custo[%d]: %d\n", vizinhos[i],custo[vizinhos[i]]);
            anterior[vizinhos[i]] = origem;
            //printf("anterior[%d]: %d\n", vizinhos[i],anterior[vizinhos[i]]);
        }
        /* Caso o custo seja maior do que um custo ja definido para aquele vizinho
            então ele não é alterado, permanecendo o menor valor de custo */
        else{
            custoTotal = custo[vizinhos[i]];
        }
        i++;
    }
    int menorCusto=0;

    menorCusto = menorDistancia(grafo,custo,&verticeMenorCusto,visitados,origem,primeiro);


    /* enquanto não chegar no vértice de destino */
    if(verticeMenorCusto != destino){
        if(menorCusto == 1001){
            //printf("############3");
            return menorCusto;
        }else{
            dijkstra(grafo, verticeMenorCusto, destino,visitados,custo,anterior,primeiro,conjAberto);
        }
    }
    else{
        //if(menorCusto == 1001){
          //  printf("############3");
            return menorCusto;
        //}
    }
}

int menorDistancia(Grafo *grafo, int *custo,int *verticeMenorCusto, int *visitado, int origem, int primeiro){
    int i, menor = 1001;
      for(i=0; i<grafo->vertices; i++){
        if(custo[i] != 1001 && i != primeiro && visitado[i] != 1){
            if(menor > custo[i]){
                menor = custo[i];
                *verticeMenorCusto = i;
            }
        }
    }
    return menor;
}

int function_vizinhos(Grafo *gr, int nodo, int vizinhos[]){
    int i=0,j=0;
    for(i=0; i< gr->vertices; i++){
        if(gr->arestas[nodo][i] == 1 || gr->arestas[i][nodo] == 1){
            vizinhos[j] = i;
           // printf("%5d",vizinhos[j]);
            j++;
        }
    }
    return j; /* quantidade de vizinhos do nodo */
}
