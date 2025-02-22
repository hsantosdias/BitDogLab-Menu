
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
//#include "pio_config.h"

// Biblioteca gerada pelo arquivo ws2818b.pio 
//#include "ws2818b.pio.h"



// Matrizes com nomes de colunas e linhas - GPIO - BitDogLab
//Portas GPIO sem função específica definida
const uint LINHAS[4] = {18, 19, 20, 4}; 
const uint COLUNAS[4] = {16, 17, 9, 8};


//função para inicializar o teclado
// Mapeamento das teclas em uma matriz 4x4
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Inicializando os pinos de GPIO para o teclado
void iniciar_teclado() {

    // Definição das linhas como saídas e inicialização em nível baixo
    for (int i = 0; i < 4; i++) {
        gpio_init(LINHAS[i]);
        gpio_set_dir(LINHAS[i], GPIO_OUT);
        gpio_put(LINHAS[i], 0);
    }

    // Definição das colunas como entradas com resistores de pull-up
    for (int i = 0; i < 4; i++) {
        gpio_init(COLUNAS[i]);
        gpio_set_dir(COLUNAS[i], GPIO_IN);
        gpio_pull_up(COLUNAS[i]);
    }
}

// Varredura do teclado e retorno da tecla pressionada
char leitura_teclado() {
    for (int row = 0; row < 4; row++) {
        // Coloca a linha atual em nível baixo
        gpio_put(LINHAS[row], 0);

        for (int col = 0; col < 4; col++) {
            // Verifica se a tecla foi pressionada
            if (!gpio_get(COLUNAS[col])) {
                printf("GPIO Linha: %d, GPIO Coluna: %d\n", LINHAS[row], COLUNAS[col]);
                printf("Tecla pressionada: %c\n", teclas[row][col]);

                // Espera um tempo para estabilização da tecla pressionada
                sleep_ms(150);

                // Aguarda a tecla ser liberada antes de continuar
                while (!gpio_get(COLUNAS[col]));

                gpio_put(LINHAS[row], 1); // Restaura a linha atual
                return teclas[row][col];
            }
        }

        // Restaura a linha atual para nível alto
        gpio_put(LINHAS[row], 1);
    }

    return 0; // Nenhuma tecla pressionada
}