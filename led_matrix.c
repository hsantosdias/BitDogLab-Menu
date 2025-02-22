#include "led_matrix.h" // Inclui o arquivo de cabeçalho local com as definições de funções e tipos de dados

// Variáveis globais para controle da matriz de LEDs WS2812
static PIO np_pio;  // Instância da interface PIO
static uint sm;     // Máquina de estado usada na PIO
static npLED_t leds[LED_COUNT]; // Buffer de LEDs armazenando os valores de cor

// Mapeia um índice para a posição correta na matriz de LEDs (linha x coluna)
static int map_index(int row, int col) {
    return row * COLS + col;
}

// Converte valores RGB para o formato GRB exigido pelo WS2812
static uint32_t rgb_to_grb(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

// Define a cor de um pixel específico na matriz de LEDs
void led_matrix_set_pixel(uint index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < LED_COUNT) {
        leds[index] = (npLED_t){g, r, b};  // Define as cores no formato GRB
    }
}

// Inicializa a matriz de LEDs WS2812
void led_matrix_init(void) {
    uint offset = pio_add_program(pio0, &ws2812b_program); // Carrega o programa PIO
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true); // Obtém uma máquina de estado livre
    ws2812b_program_init(np_pio, sm, offset, MATRIX_LED_PIN); // Configura a PIO para comunicação com os LEDs
    led_matrix_clear(); // Limpa a matriz inicializando todos os LEDs como apagados
}

// Limpa a matriz de LEDs (desliga todos os LEDs)
void led_matrix_clear(void) {
    for (uint i = 0; i < LED_COUNT; i++) {
        led_matrix_set_pixel(i, 0, 0, 0); // Define todos os LEDs para a cor preta (desligado)
    }
}

// Escreve os dados da matriz de LEDs no barramento WS2812
void led_matrix_write(void) {
    uint32_t save = save_and_disable_interrupts(); // Desativa interrupções para evitar conflitos de tempo
    for (uint i = 0; i < LED_COUNT; i++) {
        pio_sm_put_blocking(np_pio, sm, rgb_to_grb(leds[i].R, leds[i].G, leds[i].B)); // Envia dados ao LED
    }
    busy_wait_us(300); // Aguarda tempo necessário para atualização correta dos LEDs
    restore_interrupts(save); // Restaura as interrupções
}

// Exibe um número de 0 a 9 na matriz de LEDs 5x5
void led_matrix_display_number(int number) {
    led_matrix_clear(); // Limpa a matriz antes de exibir um novo número

    // Definição de padrões para exibição dos números de 0 a 9 na matriz 5x5
    static const uint numbers[10][5][5] = {
        {{0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}}, // 0
        {{0, 1, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 0, 0}}, // 1
        {{0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}}, // 2
        {{0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}}, // 3
        {{0, 1, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}}, // 4
        {{0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}}, // 5
        {{0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}}, // 6
        {{0, 1, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}}, // 7
        {{0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}}, // 8
        {{0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}}  // 9
    };

    // Percorre a matriz 5x5 e ativa os LEDs correspondentes ao número
    for (uint i = 0; i < 5; i++) {
        for (uint j = 0; j < 5; j++) {
            if (numbers[number][i][j]) { 
                led_matrix_set_pixel(map_index(i, j), 255, 255, 255); // Define os LEDs como branco
            }
        }
    }
    led_matrix_write(); // Atualiza a matriz para exibir o número
}
