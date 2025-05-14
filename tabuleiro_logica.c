#include <stdio.h>
#include <string.h>

#define WIDTH 128
#define HEIGHT 64
#define LEVELS 8
#define OBSTACLE_SIZE 4
#define LEVEL_SPACING 6
#define STEP_HORIZONTAL 12

typedef struct Bola {
    int x_inicial;
    int x_final;
    int y_inicial;
    int y_final;
} Bola;

// Tabuleiro Galton Board
void desenhar_galton_board(int matriz[HEIGHT][WIDTH]) {
    memset(matriz, 0, sizeof(int) * WIDTH * HEIGHT);

    int margem_vertical = (HEIGHT - LEVELS * LEVEL_SPACING) / 2;
    int centro_tela = WIDTH / 2;

    for (int nivel = 1; nivel < LEVELS; nivel++) {
        int y_inicio = margem_vertical + nivel * LEVEL_SPACING;

        int total_largura = (nivel - 1) * STEP_HORIZONTAL;
        int x_inicio_base = centro_tela - total_largura / 2;

        for (int i = 0; i < nivel; i++) {
            int centro_x = x_inicio_base + i * STEP_HORIZONTAL;
            int x_inicio = centro_x - OBSTACLE_SIZE / 2;

            for (int dy = 0; dy < OBSTACLE_SIZE; dy++) {
                for (int dx = 0; dx < OBSTACLE_SIZE; dx++) {
                    int x = x_inicio + dx;
                    int y = y_inicio + dy;
                    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                        matriz[y][x] = 1;
                    }
                }
            }
        }
    }
}

void desenhar_borda(int matriz[HEIGHT][WIDTH]) {
    // Borda superior
    for (int i = 0; i < WIDTH; i++) matriz[0][i] = 1;
    // Borda inferior
    for (int i = 0; i < WIDTH; i++) matriz[HEIGHT - 1][i] = 1;
    // Borda Lateral esquerda
    for (int i = 0; i < HEIGHT; i++) matriz[i][0] = 1;
    // Borda Lateral direita
    for (int i = 0; i < HEIGHT; i++) matriz[i][WIDTH - 1] = 1;
    
}

void imprimir_matriz(int matriz[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Verifica o valor na matriz e imprime o caractere correspondente
            if (matriz[y][x] == 2) {
                printf("8");
            } else {
                printf("%c", matriz[y][x] ? '#' : '.');
            }
        }
        printf("\n");
    }
}



// Bola
void desenhar_bola(Bola *bola, int matriz[HEIGHT][WIDTH]) {
    for (int i = bola->x_inicial; i < bola->x_final; i++)
    {
        for (int j = bola->y_inicial; j < bola->y_final; j++)
        {
            matriz[i][j] = 2;
        }
    }
}

void desenha_bola_direcao(Bola *bola, int matriz[HEIGHT][WIDTH], int direcao) {
    // Apaga posição anterior
    for (int i = bola->x_inicial; i < bola->x_final; i++)
    {
        for (int j = bola->y_inicial; j < bola->y_final; j++)
        {
            matriz[i][j] = 0;
        }
    }
    if (direcao == 1)  // Esquerda
    {
        bola->y_inicial-=7;
        bola->y_final-=7;
    } else {
        bola->y_inicial+=7;
        bola->y_final+=7;
    }

    bola->x_inicial+=6;
    bola->x_final+=6;
    

    for (int i = bola->x_inicial; i < bola->x_final; i++)
    {
        for (int j = bola->y_inicial; j < bola->y_final; j++)
        {
            matriz[i][j] = 2;
        }
    }
}


int main() {
    Bola *bola;

    bola->x_inicial = 8;
    bola->x_final = 12;
    bola->y_inicial = 62;
    bola->y_final = 66;

    int matriz[HEIGHT][WIDTH];

    desenhar_galton_board(matriz);
    desenhar_borda(matriz);
    desenhar_bola(bola, matriz);
    imprimir_matriz(matriz);
    printf("\n\n\n");
    desenha_bola_direcao(bola, matriz, 1);
    imprimir_matriz(matriz);
    desenha_bola_direcao(bola, matriz, 0);
    imprimir_matriz(matriz);
    return 0;
}
