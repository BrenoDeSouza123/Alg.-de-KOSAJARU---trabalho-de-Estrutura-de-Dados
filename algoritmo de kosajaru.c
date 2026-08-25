#include <stdio.h>
#include <stdlib.h>

typedef struct Aresta {
    int destino;
    struct Aresta *prox;
} Aresta;


typedef struct {
    int n;          
    Aresta **adj;   
} Grafo;

typedef struct {
    int visitas;
    int arestas_examinadas;
} Contador;

void liberar_grafo(Grafo *g);

Grafo *criar_grafo(int n) {
    Grafo *g = malloc(sizeof(Grafo));

    if (g == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }

    g->n = n;
    g->adj = calloc((size_t)n, sizeof(Aresta *));

    if (g->adj == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        free(g);
        exit(EXIT_FAILURE);
    }
    return g;
}

void adicionar_aresta(Grafo *g, int origem, int destino) {
    Aresta *nova = malloc(sizeof(Aresta));

    if (nova == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }

    nova->destino = destino;
    nova->prox = g->adj[origem];
    g->adj[origem] = nova;
}

Grafo *transposto(const Grafo *g) {
    Grafo *t = criar_grafo(g->n);

    for (int u = 0; u < g->n; u++) {
        for (Aresta *e = g->adj[u]; e != NULL; e = e->prox) {
            adicionar_aresta(t, e->destino, u);
        }
    }
    return t;
}

void dfs_ordem(const Grafo *g, int u, int *visitado, int *ordem, int *pos, Contador *contador) {
    visitado[u] = 1;
    contador->visitas++;

    for (Aresta *e = g->adj[u]; e != NULL; e = e->prox) {
     
        contador->arestas_examinadas++;

        if (!visitado[e->destino]) {
            dfs_ordem(g, e->destino, visitado, ordem, pos, contador);
        }
    }
    ordem[(*pos)++] = u;
}

void dfs_componente(const Grafo *g, int u, int *visitado, int id_componente, int *componente, Contador *contador) {
    visitado[u] = 1;
    componente[u] = id_componente;

    contador->visitas++;

    for (Aresta *e = g->adj[u]; e != NULL; e = e->prox) {

        contador->arestas_examinadas++;

        if (!visitado[e->destino]) {
            dfs_componente(g, e->destino, visitado, id_componente, componente, contador);
        }
    }
}


int *kosaraju(const Grafo *g, int *num_componentes, Contador *contador) {
    
    int n = g->n;
    int *visitado = calloc((size_t)n, sizeof(int));
    int *ordem = malloc((size_t)n * sizeof(int));
    int *componente = malloc((size_t)n * sizeof(int));

    if (visitado == NULL || ordem == NULL || componente == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");

        free(visitado);
        free(ordem);
        free(componente);

        exit(EXIT_FAILURE);
    }

    contador->visitas = 0;
    contador->arestas_examinadas = 0;

    int pos = 0;
    for (int u = 0; u < n; u++) {
        if (!visitado[u]) {
            dfs_ordem(g, u, visitado, ordem, &pos, contador);
        }
    }

    for (int u = 0; u < n; u++) {
        visitado[u] = 0;
    }

    *num_componentes = 0;

    for (int i = n - 1; i >= 0; i--) {

        int u = ordem[i];

        if (!visitado[u]) {

            dfs_componente(
                gt,
                u,
                visitado,
                *num_componentes,
                componente,
                contador
            );

            (*num_componentes)++;
        }
    }

    liberar_grafo(gt);
    free(visitado);
    free(ordem);

    return componente;
}

void imprimir_grafo(const Grafo *g) {

    for (int u = 0; u < g->n; u++) {
        printf("  %d: ", u + 1);

        Aresta *e = g->adj[u];

        if (e == NULL) {
            printf("(nenhuma)");
        }

        while (e != NULL) {

            printf("%d", e->destino + 1);
            if (e->prox != NULL) {
                printf(" -> ");
            }

            e = e->prox;
        }
        printf("\n");
    }
}


void imprimir_arestas(const int arestas[][2], int m) {
    for (int i = 0; i < m; i++) {

        printf(
            "  %d -> %d\n",
            arestas[i][0] + 1,
            arestas[i][1] + 1
        );
    }
}

void imprimir_componentes(const int *componente, int n,int num_componentes) {
    printf(
        "Componentes fortemente conexas: %d\n",
        num_componentes
    );

    for (int c = 0; c < num_componentes; c++) {

        printf("  CFC %d: { ", c + 1);
        int primeiro = 1;

        for (int v = 0; v < n; v++) {
            if (componente[v] == c) {
                if (!primeiro) {
                    printf(", ");
                }

                printf("%d", v + 1);
                primeiro = 0;
            }
        }
        printf(" }\n");
    }
}

void executar_exemplo(const char *nome, int n, const int arestas[][2], int m) {
    Grafo *g = criar_grafo(n);

    for (int i = 0; i < m; i++) {
        adicionar_aresta(g, arestas[i][0], arestas[i][1]);
    }
    printf("==================================================\n");
    printf("%s\n", nome);
    printf("==================================================\n");

    printf("Entrada:\n");

    printf("  Vertices: %d\n", n);
    printf("  Arestas: %d\n", m);

    imprimir_arestas(arestas, m);

    printf("\nLista de adjacencia:\n");

    imprimir_grafo(g);

    Contador contador;

    int num_componentes;

    int *componente = kosaraju(g, &num_componentes, &contador);

    printf("\nSaida do algoritmo de Kosaraju:\n");

    imprimir_componentes(componente, n, num_componentes);

    printf("\nContadores:\n");
    printf(""  Visitas de vertices nas duas DFS: %d\n", contador.visitas);

    printf("  Arestas examinadas nas duas DFS: %d\n", contador.arestas_examinadas);

    free(componente);
    liberar_grafo(g);

    printf("\n");
}

void liberar_grafo(Grafo *g) {
    if (g == NULL) {
        return;
    }

    for (int u = 0; u < g->n; u++) {

        Aresta *e = g->adj[u];
        while (e != NULL) {

            Aresta *prox = e->prox;
            free(e);
            e = prox;
        }
    }

    free(g->adj);
    free(g);
}

int main(void) {

    const int arestas1[][2] = {

        {0, 1},
        {1, 2},
        {2, 0},

        {2, 3},

        {3, 4},
        {4, 3},

        {4, 5}
    };

    const int arestas2[][2] = {

        {0, 1},
        {1, 0},

        {1, 2},

        {2, 3},
        {3, 2},

        {3, 4}
    };

    executar_exemplo("EXEMPLO 1", 6, arestas1, (int)(sizeof(arestas1) / sizeof(arestas1[0])));

    executar_exemplo("EXEMPLO 2", 5, arestas2, (int)(sizeof(arestas2) / sizeof(arestas2[0])));
    return 0;
}