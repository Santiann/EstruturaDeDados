#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

RBNode* createRBNode(Artist artist) {
    RBNode* newNode = (RBNode*)malloc(sizeof(RBNode));
    newNode->artist = artist;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->color = 1; // Red by default
    return newNode;
}

void rbInsert(RBNode** root, RBNode* newNode) {
    // Perform a standard BST insertion
    RBNode* parent = NULL;
    RBNode* current = *root;

    while (current != NULL) {
        parent = current;
        if (newNode->artist.id < current->artist.id)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;

    if (parent == NULL)
        *root = newNode;
    else if (newNode->artist.id < parent->artist.id)
        parent->left = newNode;
    else
        parent->right = newNode;

}

AdjListNode* createAdjListNode(int movieId) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->movieId = movieId;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int numMovies) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numMovies = numMovies;
    graph->array = (AdjList*)malloc(numMovies * sizeof(AdjList));
    for (int i = 0; i < numMovies; i++) {
        graph->array[i].head = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    AdjListNode* newNode = createAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

void printGraph(Graph* graph) {
    printf("graph {\n");
    for (int i = 0; i < graph->numMovies; i++) {
        AdjListNode* node = graph->array[i].head;
        while (node != NULL) {
            printf(" \"%s\" -- \"%s\"\n", graph->array[i].title, graph->array[node->movieId].title);
            node = node->next;
        }
    }
    printf("}\n");
}

void freeGraph(Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->numMovies; i++) {
        AdjListNode* node = graph->array[i].head;
        while (node != NULL) {
            AdjListNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(graph->array);
    free(graph);
}

void buildArtistTree(char* filename, RBNode** root) {

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    char line[1000];
    int lineCount = 0;
    fgets(line, sizeof(line), file); // Ignora o cabeçalho
    while (fgets(line, sizeof(line), file) && lineCount < 100) {
        Artist artist;
        sscanf(line, "%d %[^\t\n]s", &artist.id, artist.name);
        
        // Ler a lista de filmes e atribuí-la ao artista
        char* token = strtok(line, "\t"); // Pular o ID do artista
        token = strtok(NULL, "\t"); // Pular o nome do artista
        token = strtok(NULL, "\t"); // Pular o ano de nascimento
        token = strtok(NULL, "\t"); // Pular o ano de falecimento
        token = strtok(NULL, "\t"); // Pular a lista de profissões
        
        // Obter a lista de filmes do artista
        char* movieToken = strtok(token, ",");
        artist.numMovies = 0;
        while (movieToken != NULL) {
            artist.numMovies++;
            movieToken = strtok(NULL, ",");
        }
        artist.movies = (int*)malloc(artist.numMovies * sizeof(int));
        
        movieToken = strtok(token, ",");
        int i = 0;
        while (movieToken != NULL) {
            sscanf(movieToken, "tt%d", &artist.movies[i]);
            i++;
            movieToken = strtok(NULL, ",");
        }

        // Inserir o artista na árvore
        RBNode* newNode = createRBNode(artist);
        rbInsert(root, newNode);
        lineCount++;
    }

    fclose(file);
}

void buildMovieGraph(char* filename, Graph** graph) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("teste");
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    char line[1000];
    fgets(line, sizeof(line), file); // Ignora o cabeçalho
    int numMovies = 0;
    int lineCount = 0;
    while (fgets(line, sizeof(line), file) && lineCount < 100) {
        char type[100], title[100];
        sscanf(line, "%*s %[^\t\n]s\t%[^\t\n]s", type, title);
        if (strcmp(type, "movie") == 0) {
            numMovies++;
        }

        lineCount++;
    }
    *graph = createGraph(numMovies);

    rewind(file); // Voltar ao início do arquivo

    fgets(line, sizeof(line), file); // Ignora o cabeçalho
    int movieIndex = 0;
    while (fgets(line, sizeof(line), file)) {
        char type[100], title[100];
        sscanf(line, "%*s %[^\t\n]s\t%[^\t\n]s", type, title);
        if (strcmp(type, "movie") == 0) {
            strcpy((*graph)->array[movieIndex].title, title);
            movieIndex++;
        }
    }

    fclose(file);
}

int main() {
    RBNode* artistTree = NULL;
    Graph* movieGraph = NULL;

    buildArtistTree("C:/Versionamento/Estrutura/dados/name.basics.tsv", &artistTree);
    buildMovieGraph("C:/Versionamento/Estrutura/dados/title.basics.tsv", &movieGraph);

    // TODO: Percorrer a árvore e criar as arestas do grafo

    printGraph(movieGraph);
    freeGraph(movieGraph);

    return 0;
}