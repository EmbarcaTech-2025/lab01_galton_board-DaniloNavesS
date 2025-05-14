#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>
#include <time.h>

// Biblioteca para uso do SSD1306, display OLED.
#include "include/ssd1306.h"
ssd1306_t oled;

// Definições para uso de comunicação I2C com os pinos do display OLED.
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

#define WIDTH 128
#define HEIGHT 64
#define LEVELS 8
#define OBSTACLE_SIZE 4
#define LEVEL_SPACING 8
#define STEP_HORIZONTAL 12 // distância entre os centros dos obstáculos

#define NUM_BOLA 15

#define NUM_CASAS 8

typedef struct Bola {
    int x_inicial;
    int x_final;
    int y_inicial;
    int y_final;
    int status;
} Bola;

typedef enum {
    ESQUERDA = -1,
    DIREITA = 1
} Direcao;

Bola bolas[NUM_BOLA];

char buffer[16];


int contador_posicao[NUM_CASAS] = {0};


// Para desenhar em matriz
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

// Desenhar bola
void desenhar_bola(Bola *bola) {
    for (int y = bola->y_inicial; y < bola->y_final; y++) {
        for (int x = bola->x_inicial; x < bola->x_final; x++) {
            // Verifica se o pixel está na borda (primeira ou última linha/coluna)
            bool borda_horizontal = (y == bola->y_inicial || y == bola->y_final - 1);
            bool borda_vertical = (x == bola->x_inicial || x == bola->x_final - 1);

            if ((borda_horizontal || borda_vertical) && x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                ssd1306_draw_pixel(&oled, x, y);
            }
        }
    }

    ssd1306_show(&oled);
}

// Apagar bola
void apagar_bola(Bola *bola) {
    for (int y = bola->y_inicial; y < bola->y_final; y++) {
        for (int x = bola->x_inicial; x < bola->x_final; x++) {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                ssd1306_clear_pixel(&oled, x, y);
            }
        }
    }

    ssd1306_show(&oled);
}

void mover_bola(Bola *bola, Direcao direcao) {
    apagar_bola(bola);

    int deslocamento = 6 * direcao;

    int gravidade = 8;

    // Verifica se nova posição estará dentro dos limites da tela
    if(bola->x_inicial + deslocamento >= 0 && bola->x_final + deslocamento <= WIDTH) {
        bola->x_inicial += deslocamento;
        bola->x_final += deslocamento;
    }

    if(bola->y_inicial + gravidade >= 0 && bola->y_final + gravidade <= HEIGHT) {
        bola->y_inicial += gravidade;
        bola->y_final += gravidade;
    }

    desenhar_bola(bola);
}


// Funcoes de Controle
void atualizarDisplay (int matriz[HEIGHT][WIDTH]) {

    desenhar_galton_board(matriz);
    desenhar_borda(matriz);

    ssd1306_clear(&oled);

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (matriz[y][x] == 1)
            {
                ssd1306_draw_pixel(&oled, x, y);
            }
        }
    }

    ssd1306_show(&oled);
};

void inicializar_bolas() {
    for (int i = 0; i < NUM_BOLA; i++) {
        bolas[i].x_inicial = 62;
        bolas[i].x_final   = bolas[i].x_inicial + 4;
        bolas[i].y_inicial = 2;
        bolas[i].y_final   = bolas[i].y_inicial + 4;
        bolas[i].status    = false;
    }
}

int calcular_casa_final(Bola *bola, int num_casas) {
    int centro_bola = (bola->x_inicial + bola->x_final) / 2;
    int largura_casa = WIDTH / num_casas;
    int casa = centro_bola / largura_casa;

    // Garante que casa está dentro do intervalo [0, num_casas - 1]
    if (casa >= num_casas) casa = num_casas - 1;
    if (casa < 0) casa = 0;

    return casa;
}

void mostrar_numero_bola(int numero) {
    for (int y = 1; y < 15; y++) {
        for (int x = 1; x < 50; x++) {
            ssd1306_clear_pixel(&oled, x, y);
        }
    }

    snprintf(buffer, sizeof(buffer), "Bola:%d", numero + 1); // +1 para exibir a partir de 1
    ssd1306_draw_string(&oled, 5, 5, 1,buffer);
    ssd1306_show(&oled);
}

void desenhar_histograma() {
    int histograma_width = 3;  // Largura de cada barra do histograma
    int max_height = HEIGHT - 10;  // Altura máxima para o histograma (considerando algum espaço para margem)

    // Desenhando as barras do histograma
    for (int i = 0; i < NUM_CASAS; i++) {
        int altura_barra = (contador_posicao[i] * max_height) / NUM_BOLA;  // Normalizando a altura

        // Desenhando a barra no histograma
        for (int y = HEIGHT - altura_barra; y < HEIGHT; y++) {
            for (int x = i * histograma_width; x < (i + 1) * histograma_width; x++) {
                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                    ssd1306_draw_pixel(&oled, x, y);  // Desenha o pixel da barra
                }
            }
        }
    }

    ssd1306_show(&oled);
}


int main() {
    stdio_init_all();

    srand(to_us_since_boot(get_absolute_time()));

    sleep_ms(500);

    int matriz[HEIGHT][WIDTH];

    // Inicialização da porta I2C em 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    if (!ssd1306_init(&oled, 128, 64, 0x3C, I2C_PORT))
    {
        printf("Parou aqui\n");
        return 1;
    }

    // Limpeza de inicializacao
    ssd1306_clear(&oled);
    ssd1306_show(&oled);

    atualizarDisplay(matriz);

    sleep_ms(2000);

    inicializar_bolas();

    for (int b = 0; b < NUM_BOLA; b++) {

        mostrar_numero_bola(b);

        Bola *bola = &bolas[b];

        desenhar_bola(bola);

        sleep_ms(350);

        for (int i = 0; i < LEVELS - 1; i++) {
            int rng = rand();
            printf("rng: %d\n", rng);

            if (rng % 2 == 0) 
                mover_bola(bola, DIREITA);
            else 
                mover_bola(bola, ESQUERDA);

            sleep_ms(100);
        }

        int casa_final = calcular_casa_final(bola, NUM_CASAS);
        printf("casa final da bola %d: %d\n", b, casa_final);
        contador_posicao[casa_final]++;
        desenhar_histograma();

        sleep_ms(100);
        apagar_bola(bola);
    }

    fflush(stdout);
    for (int i = 0; i < NUM_CASAS; i++)
    {
        printf("casa %d: %d\n", i ,contador_posicao[i]);
    }
    
    fflush(stdout);

    return 0;
}
