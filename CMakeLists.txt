cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Definir o modelo da placa
set(PICO_BOARD pico CACHE STRING "Board type")

# Importar o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

# Nome do projeto e linguagens utilizadas
project(BitDogLab-Menu C CXX ASM)

# Inicializar o SDK do Raspberry Pi Pico
pico_sdk_init()

# Adicionar o executável
add_executable(BitDogLab-Menu 
    BitDogLab-Menu.c 
    ssd1306.c 
    led_matrix.c
)

# Configurações do executável
pico_set_program_name(BitDogLab-Menu "BitDogLab-Menu")
pico_set_program_version(BitDogLab-Menu "0.1")

# Habilitar saída USB e desabilitar UART
pico_enable_stdio_usb(BitDogLab-Menu 1)
pico_enable_stdio_uart(BitDogLab-Menu 0)

# Gerar cabeçalho para PIO
pico_generate_pio_header(BitDogLab-Menu ${CMAKE_CURRENT_LIST_DIR}/ws2812b.pio)

# Adicionar bibliotecas e linkar ao executável
target_link_libraries(BitDogLab-Menu 
    pico_stdlib 
    hardware_uart 
    hardware_i2c 
    hardware_adc 
    hardware_pwm 
    hardware_pio
)

# Incluir diretórios de cabeçalhos
target_include_directories(BitDogLab-Menu PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
)

# Gerar arquivos de saída extras (UF2, BIN, etc.)
pico_add_extra_outputs(BitDogLab-Menu)
