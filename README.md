
# BitDogLab-Menu

## Descrição do Projeto

O **BitDogLab-Menu** é um sistema de navegação de menus desenvolvido para o **RP2040** utilizando um **joystick** e um  **display OLED SSD1306** . Este projeto permite navegar por um menu hierárquico com submenus e executar ações associadas a cada opção do menu. Ele foi desenvolvido em **C** utilizando o  **Pico SDK** , com suporte para navegação fluida e hierárquica em submenus.

---


## Funcionalidades Implementadas

* **Navegação Hierárquica** : O sistema permite navegar por submenus e retornar ao menu principal.
* **Execução de Ações** : Cada opção de menu pode ter uma função associada que é executada ao selecioná-la.
* **Histórico de Navegação** : O menu mantém um histórico de navegação, permitindo retornar ao nível anterior.
* **Timeout do Menu** : Após um tempo de inatividade (30 segundos), o sistema volta automaticamente para o menu principal.
* **Controle via Joystick** : Navegação e seleção de opções utilizando um  **joystick analógico** .
* **Display OLED SSD1306** : O menu é exibido em um **display OLED** utilizando a biblioteca  **SSD1306** .

---


## Estrutura do Menu

O menu é organizado de forma hierárquica, permitindo navegar por submenus e retornar ao menu principal. As opções de menu incluem:

* **Info Ambiental** :
* Temperatura
* Umidade
* Voltar (Retorna ao menu principal)
* **GeoLocalizacao** :
* Posição
* Voltar
* **Alert Mensagems** :
* Mensagens
* Voltar
* **Config Sistema** :
* Ajustes
* Informações
* Voltar

---

## **Estrutura do Projeto**

BitDogLab-Menu
├── build/                   # Diretório para compilação
├── BitDogLab-Menu.c         # Código-fonte principal
├── CMakeLists.txt           # Configuração do CMake
├── pico_sdk_import.cmake    # Configuração do SDK
├── README.md                # Documentação do projeto
└── ssd1306.c                # Biblioteca para o display OLED


## Funcionalidades Principais

* **Navegação no Menu** : Utiliza o eixo **Y do joystick** para navegar pelas opções do menu.
* **Seleção de Opções** : O **botão do joystick** é utilizado para selecionar uma opção.
* **Retorno ao Menu Principal** : O **Botão A** é utilizado para retornar ao menu principal.
* **Modo BOOTSEL** : O **Botão B** reinicia o microcontrolador no modo BOOTSEL.
* **Timeout do Menu** : Após **30 segundos** de inatividade, o sistema retorna automaticamente para o menu principal.

---


## Configuração do Ambiente, Instalação e Uso

### 1. **Instale o SDK do Raspberry Pi Pico:**

* Siga o guia oficial para configuração: [Guia do Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/)

### 2. **Clone o repositório do projeto:**


```bash
git clone https://github.com/hsantosdias/BitDogLab-Menu.git
cd BitDogLab-Menu

```



### 3. **Compile o código:**

* Certifique-se de que o SDK está instalado e funcionando corretamente no VSCode.



```bash
Usando a extensão do Raspberry Pi Pico Visual Studio Code extension
Siga os seguintes passos:
1. Clean CMake (Para garantir que o projeto será compilado do zero)
2. Compile Project (Compilação dos binários)
3. Run Project [USB] (Compila e copia o firmware para a placa automaticamente)
```



### 4. **Carregue o binário no Pico:**

* Conecte o Pico ao computador no modo bootloader.
* Copie o arquivo `.uf2` (Depois de compilado, copie o `./build/BitDogLab-Menu.uf2` para a Raspberry em modo bootsel).


## Controles e Navegação

* **Joystick Y (Cima/Baixo)** : Navega pelas opções do menu.
* **Joystick X (Direita/Esquerda)** : Navega em submenus.
* **Botão do Joystick (PB)** : Seleciona a opção atual.
* **Botão A** : Retorna ao menu principal.
* **Botão B** : Reinicia o RP2040 no modo BOOTSEL.


## Requisitos do Projeto

### Hardware Utilizado:

* **RP2040 (Raspberry Pi Pico)**
* **Joystick analógico** (Conectado aos pinos ADC)
* **Display OLED SSD1306** (Conectado via I2C)
* **Botões A e B** para navegação e BOOTSEL

### Conexões:


| Componente         | GPIO do Pico   |
| :----------------- | -------------- |
| OLED SDA           | GPIO 14        |
| OLED SCL           | GPIO 15        |
| Joystick Eixo X    | GPIO 26 (ADC0) |
| Joystick Eixo Y    | GPIO 27 (ADC1) |
| Joystick Botão PB | GPIO 22        |
| Botão A           | GPIO 5         |
| Botão B           | GPIO 6         |


## Explicação do Fluxograma:

1. **Inicialização:**
   * O sistema inicializa o  **OLED** , o **joystick** e os  **botões** .
   * Configura o **modo BOOTSEL** para o  **Botão B** .
   * Exibe a **animação inicial** (opcional) no OLED.
2. **Loop Principal:**
   * **Verifica Timeout** : Se não houver interação por 30 segundos, retorna ao menu principal.
   * **Navegação no Menu** : O joystick é utilizado para navegar pelas opções do menu.
   * **Seleção de Opções** : O botão do joystick é utilizado para selecionar uma opção.
   * **Ação Associada** : Se houver uma função associada à opção, ela é executada.
   * **Navegação em Submenus** : Se a opção tiver um submenu, o sistema navega para o submenu correspondente.
3. **Histórico de Navegação:**
   * O sistema mantém um **histórico de navegação** para retornar ao menu anterior.
   * O histórico é **empilhado** ao entrar em um submenu e **desempilhado** ao voltar ao menu anterior.


## Funcionalidades das Ações do Menu

### **Info Ambiental**

* **Temperatura** : Exibe a temperatura atual (valor fictício).
* **Umidade** : Exibe a umidade atual (valor fictício).
* **Voltar** : Retorna ao menu principal.

### **GeoLocalizacao**

* **Posição** : Exibe latitude e longitude (valores fictícios).
* **Voltar** : Retorna ao menu principal.

### **Alert Mensagems**

* **Mensagens** : Exibe uma mensagem genérica.
* **Voltar** : Retorna ao menu principal.

### **Config Sistema**

* **Ajustes** : Exibe a mensagem "Config. Sistema".
* **Informações** : Exibe a versão do sistema.
* **Voltar** : Retorna ao menu principal.


## Dificuldades Encontradas

Durante o desenvolvimento, alguns desafios foram encontrados:

1. **Navegação Hierárquica em Submenus** : Foi necessário implementar um **histórico de navegação** para permitir o retorno aos níveis anteriores.
2. **Timeout do Menu** : Implementar um **timeout de 30 segundos** para retornar automaticamente ao menu principal exigiu o uso de **absolute_time_t** do SDK do Pico.
3. **Debounce do Joystick e Botões** : Foi necessário implementar **debounce** para evitar múltiplas detecções de cliques devido a ruídos elétricos.
4. **Exibição no OLED** : Ajustar a exibição no **OLED SSD1306** com **retângulo de seleção** e **setas de navegação** exigiu um trabalho cuidadoso de design da interface.


## Considerações Finais

O projeto **BitDogLab-Menu** oferece uma **experiência de navegação fluida e intuitiva** utilizando um **joystick** e um  **display OLED** . O sistema é **modular** e  **escalável** , permitindo adicionar novos submenus e funcionalidades com facilidade.

Este projeto é ideal para quem deseja aprender sobre:

* **Sistemas embarcados** com **RP2040**
* **Navegação hierárquica em menus**
* **Controle de OLED SSD1306** com **I2C**
* **Interação com Joystick Analógico**
* **Debounce de Botões e Joystick**


### Autor

**Hugo Santos Dias**

Este projeto foi desenvolvido com o intuito de criar um sistema de menu intuitivo e modular para o  **RP2040** , utilizando  **C e o Pico SDK** .
