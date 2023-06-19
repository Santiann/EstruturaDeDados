#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_ACTORS 100
#define MAX_MOVIES 100

typedef struct {
    int id;
    char primaryName[MAX_LINE_LENGTH];
    char knownForTitles[MAX_LINE_LENGTH];
} Actor;

typedef struct {
    int id;
    char primaryTitle[MAX_LINE_LENGTH];
} Movie;

typedef struct {
    int numActors;
    Actor actors[MAX_ACTORS];
} ActorTree;

typedef struct Node {
    Actor actor;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int color; // 0 for black, 1 for red
} Node;

typedef struct {
    int numMovies;
    Movie movies[MAX_MOVIES];
    int adjacencyMatrix[MAX_MOVIES][MAX_MOVIES];
} MovieGraph;

Node* createNode(Actor actor) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->actor = actor;
    node->left = NULL;
    node->right = NULL;
    node->color = 1; // New node is always red
    node->parent = NULL;
    return node;
}

Node* bstInsert(Node* root, Node* newNode) {
    if (root == NULL)
        return newNode;

    if (newNode->actor.id < root->actor.id)
        root->left = bstInsert(root->left, newNode);
    else if (newNode->actor.id > root->actor.id)
        root->right = bstInsert(root->right, newNode);

    return root;
}

void leftRotate(Node* root, Node* node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;

    if (node->right != NULL)
        node->right->color = 1;

    rightChild->color = node->color;
    rightChild->left = node;
    node->color = 0;

    if (node == root)
        root = rightChild;
    else if (node == node->left)
        node->left = rightChild;
    else
        node->right = rightChild;
}

void rightRotate(Node* root, Node* node) {
    Node* leftChild = node->left;
    node->left = leftChild->right;

    if (node->left != NULL)
        node->left->color = 1;

    leftChild->color = node->color;
    leftChild->right = node;
    node->color = 0;

    if (node == root)
        root = leftChild;
    else if (node == node->right)
        node->right = leftChild;
    else
        node->left = leftChild;
}

void flipColors(Node* node) {
    node->color = 1;
    node->left->color = 0;
    node->right->color = 0;
}

void fixRedRed(Node* root, Node* node) {
    if (node == root) {
        node->color = 0;
        return;
    }

    Node* parent = NULL;
    Node* grandparent = NULL;
    Node* greatGrandparent = NULL;

    while (node != root && node->color == 1 && node->parent->color == 1) {
    
        parent = node->parent;
        grandparent = parent->parent;

        if (parent == grandparent->left) {
            greatGrandparent = grandparent->parent;

            Node* uncle = grandparent->right;
            if (uncle != NULL && uncle->color == 1) {
                grandparent->color = 1;
                parent->color = 0;
                uncle->color = 0;
                node = grandparent;
            }
            else {
                if (node == parent->right) {
                    leftRotate(root, parent);
                    node = parent;
                    parent = node->parent;
                }
                rightRotate(root, grandparent);
                int tempColor = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tempColor;
                node = parent;
            }
        }
        else {
            greatGrandparent = grandparent->parent;

            Node* uncle = grandparent->left;
            if (uncle != NULL && uncle->color == 1) {
                grandparent->color = 1;
                parent->color = 0;
                uncle->color = 0;
                node = grandparent;
            }
            else {
                if (node == parent->left) {
                    rightRotate(root, parent);
                    node = parent;
                    parent = node->parent;
                }
                leftRotate(root, grandparent);
                int tempColor = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tempColor;
                node = parent;
            }
        }
    }

    root->color = 0;
}

void bstInsertWithFix(Node* root, Node* newNode) {
    root = bstInsert(root, newNode);
    fixRedRed(root, newNode);
}

void readActorsFile(ActorTree* actorTree, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];

    // Skip header line
    fgets(line, sizeof(line), file);

    int actorCount = 0;

    while (fgets(line, sizeof(line), file) != NULL && actorCount < MAX_ACTORS) {
        char* token = strtok(line, "\t");

        Actor actor;
        actor.id = atoi(token + 2); // Skip the "nm" prefix
        strcpy(actor.primaryName, strtok(NULL, "\t"));
        strcpy(actor.knownForTitles, strtok(NULL, "\t"));

        actorTree->actors[actorCount++] = actor;
    }

    actorTree->numActors = actorCount;

    fclose(file);
}

void readMoviesFile(MovieGraph* movieGraph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];

    // Skip header line
    fgets(line, sizeof(line), file);

    int movieCount = 0;

    while (fgets(line, sizeof(line), file) != NULL && movieCount < MAX_MOVIES) {
        char* token = strtok(line, "\t");

        Movie movie;
        movie.id = atoi(token + 2);
        strtok(NULL, "\t");
        strcpy(movie.primaryTitle, strtok(NULL, "\t"));

        movieGraph->movies[movieCount++] = movie;
    }

    movieGraph->numMovies = movieCount;

    fclose(file);
}

int findMovieIndex(MovieGraph* movieGraph, int movieId) {
    for (int i = 0; i < movieGraph->numMovies; i++) {
        if (movieGraph->movies[i].id == movieId)
            return i;
    }
    return -1;
}

void addEdge(MovieGraph* movieGraph, int movieIndex1, int movieIndex2) {
    movieGraph->adjacencyMatrix[movieIndex1][movieIndex2] = 1;
    movieGraph->adjacencyMatrix[movieIndex2][movieIndex1] = 1;
}

void buildMovieGraph(MovieGraph* movieGraph, ActorTree* actorTree) {
    for (int i = 0; i < actorTree->numActors; i++) {
        Actor actor = actorTree->actors[i];
        char* token = strtok(actor.knownForTitles, ",");
        while (token != NULL) {
            int movieId = atoi(token + 2); // Skip the "tt" prefix
            int movieIndex = findMovieIndex(movieGraph, movieId);
            if (movieIndex != -1) {
                for (int j = 0; j < movieGraph->numMovies; j++) {
                    if (j != movieIndex && movieGraph->adjacencyMatrix[movieIndex][j] == 1) {
                        addEdge(movieGraph, j, movieIndex);
                    }
                }
            }
            token = strtok(NULL, ",");
        }
    }
}

void printGraphDot(MovieGraph* movieGraph) {
    printf("graph {\n");

    for (int i = 0; i < movieGraph->numMovies; i++) {
        for (int j = i + 1; j < movieGraph->numMovies; j++) {
            if (movieGraph->adjacencyMatrix[i][j] == 1) {
                printf("\t\"%s\" -- \"%s\";\n", movieGraph->movies[i].primaryTitle, movieGraph->movies[j].primaryTitle);
            }
        }
    }

    printf("}\n");
}

int main() {
    ActorTree actorTree;
    readActorsFile(&actorTree, "C:/Versionamento/Estrutura/EstruturaDeDados/name.tsv");

    Node* root = NULL;
    for (int i = 0; i < actorTree.numActors; i++) {
        Node* newNode = createNode(actorTree.actors[i]);
        bstInsertWithFix(root, newNode);
    }

    MovieGraph movieGraph;
    readMoviesFile(&movieGraph, "C:/Versionamento/Estrutura/EstruturaDeDados/titiles.tsv");
    buildMovieGraph(&movieGraph, &actorTree);

    printGraphDot(&movieGraph);

    return 0;
}