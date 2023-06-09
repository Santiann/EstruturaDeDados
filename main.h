#ifndef MAIN_H
#define MAIN_H

// Structs e protótipos de funções

typedef struct {
    int id;
    char name[100];
    int* movies;
    int numMovies;
} Artist;

typedef struct {
    int id;
    char title[100];
} Movie;

typedef struct rbNode {
    Artist artist;
    struct rbNode* parent;
    struct rbNode* left;
    struct rbNode* right;
    int color;
} RBNode;

typedef struct adjListNode {
    int movieId;
    struct adjListNode* next;
} AdjListNode;

typedef struct {
    char title[100];
    AdjListNode* head;
} AdjList;

typedef struct {
    int numMovies;
    AdjList* array;
} Graph;

RBNode* createRBNode(Artist artist);
void rbInsert(RBNode** root, RBNode* newNode); // Declaração da função rbInsert
AdjListNode* createAdjListNode(int movieId);
Graph* createGraph(int numMovies);
void addEdge(Graph* graph, int src, int dest);
void printGraph(Graph* graph);
void freeGraph(Graph* graph);
void buildArtistTree(char* filename, RBNode** root);
void buildMovieGraph(char* filename, Graph** graph);

#endif