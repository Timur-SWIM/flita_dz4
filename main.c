#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_NODES 100
#define swap(x, y) do \
    {int t;           \
    t = x;            \
    x = y;            \
    y = t;\
    } while(0)

typedef struct graph {
    int nodes[MAX_NODES][MAX_NODES];
    int countNode;
}graph;

enum ErrorStatus_t {
    SUCCESSFUL,
    NODE_N_EXIST,
    EDGE_N_EXIST
};

int addEdge(graph *g, int A, int B) {

    if (A > g->countNode || B > g->countNode){

        return NODE_N_EXIST;
    }
    if (A > B)
        swap(A, B);
    if (g->nodes[A][B] == 1){

        return EDGE_N_EXIST;
    }
    g->nodes[A][B] = 1;
    return SUCCESSFUL;
}

void graphFromFile(FILE *inputf, graph *g){
    char line[128];
    int row = 0;

    while (fgets(line, sizeof(line), inputf)){
        int col = 0;
        char *token = strtok(line, " ");
        while (token){
            int value = atoi(token);
            g->nodes[row][col++] = value;
            if (col - 1 <= row)
                g->nodes[col - 1][row] = value;
            token = strtok(NULL, " ");
        }
        row++;
    }
    g->countNode = row;
}

void writeDOT(FILE *outf, graph *g){
    fprintf(outf, "digraph G {\n");
    for (int i = 0; i < g->countNode; i++) {
        fprintf(outf, "  %d;\n", i);
    }
    for (int i = 0; i < g->countNode; i++) {
        for (int j = i; j < g->countNode; j++) {
            if (g->nodes[i][j]) {
                fprintf(outf, "  %d -> %d [dir=none, label=%d];\n", i, j, g->nodes[i][j]);
            }
        }
    }
    fprintf(outf, "}\n");
}

void printMatrix(graph *g){
    for (int i = 0; i < g->countNode; i++) {
        for (int j = 0; j < g->countNode; j++) {
            printf("%d ", g->nodes[i][j]);
        }
        printf("\n");
    }
}

// Пример использования
int main(int argc, char* argv[]) {
    graph *g = malloc(sizeof(graph));
    FILE *inputf = fopen(argv[1], "rt");
    if (inputf == NULL){
        puts("ERROR: Unable to open file");
        exit(EXIT_FAILURE);
    }
    graphFromFile(inputf, g);
    fclose(inputf);

    printf("Matrix\n");
    printMatrix(g);



    int A ;
    int B ;
    bool flag = false;

    while (!flag){
        printf("Enter the vertices:");
        scanf("%d %d", &A, &B);
        switch (addEdge(g, A, B)) {
            case SUCCESSFUL:
                puts("Edge added");
                break;
            case EDGE_N_EXIST:
                puts("ERROR: edge already exist");
                flag = true;
                break;
            case NODE_N_EXIST:
                puts("ERROR: node not exist");
                flag = true;
                break;
        }
    }

    FILE *outputf = fopen("graph.dot", "w");
    if (outputf == NULL){
        puts("ERROR: Unable to open file");
    }
    writeDOT(outputf, g);
    fclose(outputf);
    free(g);
    system("dot -Tpng graph.dot -o graph.png");

    return 0;
}

