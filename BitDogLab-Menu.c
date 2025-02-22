/*
Projeto BitDogLab - Menu
Objetivo: Desenvolver um menu para seleção de opções
Autor: Hugo Santos Dias
*/

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

// Configurações do OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Configuração dos Botões e Joystick
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22     // GPIO para botão do Joystick (Selecionar)
#define Botao_A 5          // GPIO para voltar ao Menu Principal
#define Botao_B 6          // GPIO para BOOTSEL

// Trecho para modo BOOTSEL com Botão B
#include "pico/bootrom.h"
void gpio_irq_handler(uint gpio, uint32_t events) {
    reset_usb_boot(0, 0);
}

// Estrutura do OLED
ssd1306_t ssd;

// Estrutura do Menu
typedef struct Menu {
    const char *titulo;
    struct Menu *submenus;
    int num_submenus;
} Menu;

void iniciar_oled();
void iniciar_joystick();
void animacao_inicial();
void mostrar_menu();
void navegar_menu();
void voltar_menu_principal();
// Prototipagem de Funções
void opcao_selecionada();

// Funções de Ação para Menu
void mostrar_temperatura(void);
void mostrar_umidade(void);
void mostrar_posicao(void);
void mostrar_mensagens(void);
void configurar_sistema(void);
void mostrar_informacoes(void);
// Submenus
Menu submenu_monitoramento[] = {
    {"Menu 01", NULL, 0},
    {"Menu 02", NULL, 0},
    {"Menu 03", NULL, 0},
    {"Voltar", NULL, 0}
};

Menu submenu_navegacao[] = {
    {"Menu 01", NULL, 0},
    {"Menu 02", NULL, 0},
    {"Voltar", NULL, 0}
};

Menu submenu_alertas[] = {
    {"Menu 01", NULL, 0},
    {"Menu 02", NULL, 0},
    {"Menu 03", NULL, 0},
    {"Voltar", NULL, 0},
    {"Menu 03", NULL, 0},
};

Menu submenu_configuracoes[] = {
    {"Menu 01", NULL, 0},
    {"Menu 02", NULL, 0},
    {"Menu 03", NULL, 0},
    {"Voltar", NULL, 0}
};

// Menu Principal
Menu menu_principal[] = {
  {"Info Ambiental", submenu_monitoramento, 4},
  {"GeoLocalizacao", submenu_navegacao, 4},
  {"Alertas Mensagems", submenu_alertas, 4},
  {"Config Sistema", submenu_configuracoes, 4}
};


// Variáveis Globais
int opcao_atual = 0;
Menu *menu_atual = menu_principal;
int num_opcoes = 4;

int main() {
    stdio_init_all();
    iniciar_joystick();
    iniciar_oled();
//    animacao_inicial();

    // Configuração do botão B para modo BOOTSEL
    gpio_init(Botao_B);
    gpio_set_dir(Botao_B, GPIO_IN);
    gpio_pull_up(Botao_B);
    gpio_set_irq_enabled_with_callback(Botao_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        mostrar_menu();
        navegar_menu();
        sleep_ms(150);
    }
}

// Inicializa o OLED
void iniciar_oled() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, 128, 64, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Inicializa o Joystick e Botões
void iniciar_joystick() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB);

    gpio_init(Botao_A);
    gpio_set_dir(Botao_A, GPIO_IN);
    gpio_pull_up(Botao_A);
}

// Animação Inicial
void animacao_inicial() {
    ssd1306_fill(&ssd, false);
    for (int i = 0; i < 128; i += 4) {
        ssd1306_rect(&ssd, i, i, 128 - i * 2, 64 - i * 2, true, false);
        ssd1306_send_data(&ssd);
        sleep_ms(50);
    }
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Mostra o Menu na Tela
void mostrar_menu() {
    ssd1306_fill(&ssd, false);
    for (int i = 0; i < num_opcoes; i++) {
        if (i == opcao_atual) {
            ssd1306_rect(&ssd, 0, i * 16, 128, 16, true, false);
            ssd1306_draw_string(&ssd, menu_atual[i].titulo, 5, i * 16 + 4);
        } else {
            ssd1306_draw_string(&ssd, menu_atual[i].titulo, 5, i * 16 + 4);
        }
    }
    ssd1306_send_data(&ssd);
}

// Navegação no Menu
void navegar_menu() {
    adc_select_input(1);
    uint16_t adc_value_y = adc_read();

    if (adc_value_y > 3000) {
        opcao_atual = (opcao_atual + 1) % num_opcoes;
    }
    if (adc_value_y < 1000) {
        opcao_atual = (opcao_atual - 1 + num_opcoes) % num_opcoes;
    }
    if (!gpio_get(JOYSTICK_PB)) {
        if (menu_atual[opcao_atual].submenus) {
            menu_atual = menu_atual[opcao_atual].submenus;
            num_opcoes = menu_atual[0].num_submenus;
            opcao_atual = 0;
        } else {
            opcao_selecionada();
        }
    }
    if (!gpio_get(Botao_A)) {
        voltar_menu_principal();
    }
}

// Retorna ao Menu Principal
void voltar_menu_principal() {
    menu_atual = menu_principal;
    num_opcoes = 4;
    opcao_atual = 0;
}

// Ação para Opção Selecionada
void opcao_selecionada() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Opcao Selecionada:", 10, 20);
    ssd1306_draw_string(&ssd, menu_atual[opcao_atual].titulo, 10, 40);
    ssd1306_send_data(&ssd);
    sleep_ms(1000);
}


// Funções de Ação do Menu
void mostrar_temperatura() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Temperatura:", 10, 20);
    ssd1306_draw_string(&ssd, "25.5 C", 10, 40);
    ssd1306_send_data(&ssd);
    sleep_ms(2000);
}

void mostrar_umidade() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Umidade:", 10, 20);
    ssd1306_draw_string(&ssd, "65%", 10, 40);
    ssd1306_send_data(&ssd);
    sleep_ms(2000);
}

void mostrar_posicao() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Latitude: -23.5", 10, 20);
    ssd1306_draw_string(&ssd, "Longitude: -46.6", 10, 40);
    ssd1306_send_data(&ssd);
    sleep_ms(2000);
}

void mostrar_mensagens() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Sem mensagens", 10, 20);
    ssd1306_send_data(&ssd);
    sleep_ms(2000);
}

void configurar_sistema() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Config. Sistema", 10, 20);
    ssd1306_draw_string(&ssd, "Ajustes feitos", 10, 40);
    ssd1306_send_data(&ssd);
    sleep_ms(2000);
}

void mostrar_informacoes() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Info. Sistema", 10, 20);
    ssd1306_draw_string(&ssd, "Versao 1.0", 10, 40);
    ssd1306_send_data(&ssd);
    sleep_ms(2000);

}