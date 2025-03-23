// Incluindo bibliotecas necessárias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "bibliotecas/som.h"
#include "bibliotecas/joystickled.h"
#include "bibliotecas/neopixel.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

// Definindo uma variável fixa para limitar a contagem de LEDs em 25
#define LED_COUNT 25

// Definição dos pinos e canais que irão ser utilizados
const uint I2C_SDA = 14; // Pino GPIO conectado ao barramento I2C
const uint I2C_SCL = 15; // Pino GPIO conectado ao barramento I2C
const uint BUTTON_A_PIN = 5; // Pino GPIO conectado ao Botão A
const uint BUTTON_B_PIN = 6; // Pino GPIO conectado ao Botão B
const uint BLUE_LED_PIN = 12; // Pino GPIO conectado ao LED azul
const uint RED_LED_PIN  = 13; // Pino GPIO conectado ao LED vermelho
const uint BUZZER_PIN = 21; // Pino GPIO conectado ao Buzzer
const uint VRX = 26;          // Pino de leitura do eixo X do joystick (conectado ao ADC)
const uint VRY = 27;          // Pino de leitura do eixo Y do joystick (conectado ao ADC)
const uint ADC_CHANNEL_0 = 0; // Canal ADC para o eixo X do joystick
const uint ADC_CHANNEL_1 = 1; // Canal ADC para o eixo Y do joystick
const uint JOYSTICK_BUTTON = 22; // Pino de leitura do botão do joystick
const uint LED_PIN = 7; // Pino GPIO do LED endereçável

// Variáveis de controle
int travar = 0;
int cor = 0;

// Função principal
int main() {

    // Inicializando as entradas e saídas padrão (UART)
    stdio_init_all();

    // Inicializando o conversor analógico-digital (ADC)
    adc_init();

    // Inicialização e configuração do i2c como pull-up
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Inicialização e onfiguração dos pinos dos botões como entrada com pull-up
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    // Inicialização e configuração dos LEDs RGB como saída
    gpio_init(BLUE_LED_PIN);
    gpio_init(RED_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_put(BLUE_LED_PIN, 0);
    gpio_put(RED_LED_PIN, 0);

    // Inicialização e configuração do buzzer como saída
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    pwm_init_buzzer(BUZZER_PIN);

    // Declaração de variáveis para leitura da posição do joystick (x,y)
    uint16_t x, y;

    // Inicialização dos pinos de entrada analógica
    adc_gpio_init(VRX); // Configura o pino VRX (eixo X) para entrada ADC
    adc_gpio_init(VRY); // Configura o pino VRY (eixo y) para entrada ADC

    // Inicialização e configuração do pino do botão do joystick como entrada com pull-up
    gpio_init(JOYSTICK_BUTTON);
    gpio_set_dir(JOYSTICK_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON);

    // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);
    npClear();

    // Escreve os dados nos LEDs.
    npWrite();

    // Prepara área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    // Inicializa o buffer do display
    uint8_t ssd[ssd1306_buffer_length];

    // Inicializando o loop infinito
    while (true) {
        // Resetando a placa
        memset(ssd, 0, ssd1306_buffer_length);

        // Monitorando e lendo a posição do joystick
        read_x_y(&x, &y, ADC_CHANNEL_0, ADC_CHANNEL_1);

        // Monitorando a função que muda o desenho da matriz de LEDs
        mudar_desenho(&x, &y, cor);

        // Verifica se o botão do Joystick foi pressionado
        if(gpio_get(JOYSTICK_BUTTON) == 0){
            // Limpa o buffer de pixels
            npClear();

            // Escreve os dados "vazios" nos LEDs.
            npWrite();

            // Desliga LED azul
            gpio_put(BLUE_LED_PIN, 0);

            // Desliga LED vermelho
            gpio_put(RED_LED_PIN, 0);

            // Modifica os valores das variáveis de controle para 0
            travar = 0;
            cor = 0;

            // Declara um ponteiro para um array de char que armazena a palavra "Saindo"
            char *text[] = {"  Saindo  "};

            // Declara e inicializa a variável de altura
            int y = 0;

            // Laço de repetição que se repete ao longo do tamanho do ponteiro de char anteriormente mencionado
            for(uint i = 0; i < count_of(text); i++){
                // Chama a função de desenhar string enquanto o laço se repete
                ssd1306_draw_string(ssd, 5, y, text[i]);

                // Soma 8 a variável de altura a cada repetição
                y += 8;
            }
            // Atualiza uma parte do display com uma área de renderização
            render_on_display(ssd, &frame_area);

            // Chama a função que toca o buzzer por 1 segundo
            beep(BUZZER_PIN, 1000);
            
            // Delay de 300ms
            sleep_ms(300);
            
            // Reseta os dados do display
            memset(ssd, 0, ssd1306_buffer_length);
            // Atualiza uma parte do display com uma área de renderização "vazia"
            render_on_display(ssd, &frame_area);

            // Delay de 500 ms
            sleep_ms(500);

            // Interrompe o loop
            break;

        // Verifica se nenhum botão foi pressionado
        }else if(gpio_get(BUTTON_A_PIN) == 1 && gpio_get(BUTTON_B_PIN) == 1){
            // Verifica se a váriavel de controle é igual a 0 e executa as linhas de comandos subsequentes enquanto for
            if(travar == 0){
                // Desliga o LED vermelho
                gpio_put(RED_LED_PIN, 0);

                // Desliga o LED azul
                gpio_put(BLUE_LED_PIN, 0);
                
                // Declara um ponteiro para um array de char que armazena um menu
                char *text[] = {
                    "A para Azul",
                    "B para Vermelho",
                    "A e B para Roxo",
                    "Press Joystick",
                    "Para Sair"};
                
                // Declara e inicializa a variável de altura
                int y = 0;
                
                // Laço de repetição que se repete ao longo do tamanho do ponteiro de char anteriormente mencionado
                for(uint i = 0; i < count_of(text); i++){
                    // Chama a função de desenhar string enquanto o laço se repete
                    ssd1306_draw_string(ssd, 5, y, text[i]);

                    // Soma 8 a variável de altura a cada repetição
                    y += 8;
                }

                // Atualiza uma parte do display com uma área de renderização
                render_on_display(ssd, &frame_area);
            }
        // Verifica se ambos os botões foram pressionados
        }else if(gpio_get(BUTTON_A_PIN) == 0 && gpio_get(BUTTON_B_PIN) == 0){
            // Verifica se a váriavel de controle é diferente de 3
            if(travar != 3){
                // Modifica o valor da variável de controle para 3
                travar = 3;

                // Modifica o valor da variável de controle relacionada a cor para 3 (3 = Roxo)
                cor = 3;

                // Liga o LED vermelho
                gpio_put(RED_LED_PIN, 1);

                // Liga o LED azul
                gpio_put(BLUE_LED_PIN, 1);

                // Laço de repetição que se repete até chegar na quantidade máxima de LEDs da matriz
                for(uint i = 0; i < LED_COUNT; i++){
                    // Atribui a cor roxa a um LED da matriz, avançando para o próximo a cada repetição
                    npSetLED(i, 255, 0, 255);
                }

                // Escreve os dados nos LEDs.
                npWrite();
                
                // Declara um ponteiro para um array de char que armazena a palavra "Roxo"
                char *text[] = {"  Roxo   "};

                // Declara e inicializa a variável de altura
                int y = 0;

                // Laço de repetição que se repete ao longo do tamanho do ponteiro de char anteriormente mencionado
                for(uint i = 0; i < count_of(text); i++){
                    // Chama a função de desenhar string enquanto o laço se repete
                    ssd1306_draw_string(ssd, 5, y, text[i]);

                    // Soma 8 a variável de altura a cada repetição
                    y += 8;
                }

                // Atualiza uma parte do display com uma área de renderização
                render_on_display(ssd, &frame_area);

                // Chama a função que toca o buzzer por 1 segundo
                beep(BUZZER_PIN, 1000);
            }
        // Verifica se apenas o Botão A foi pressionado
        }else if(gpio_get(BUTTON_A_PIN) == 0){
            // Verifica se a váriavel de controle é diferente de 1
            if(travar != 1){
                // Modifica o valor da variável de controle para 1
                travar = 1;

                // Modifica o valor da variável de controle relacionada a cor para 1 (1 = Azul)
                cor = 1;

                // Desliga o LED vermelho
                gpio_put(RED_LED_PIN, 0);

                // Liga o LED azul
                gpio_put(BLUE_LED_PIN, 1);

                // Laço de repetição que se repete até chegar na quantidade máxima de LEDs da matriz
                for(uint i = 0; i < LED_COUNT; i++){
                    // Atribui a cor azul a um LED da matriz, avançando para o próximo a cada repetição
                    npSetLED(i, 0, 0, 255);
                }

                // Escreve os dados nos LEDs.
                npWrite();

                // Declara um ponteiro para um array de char que armazena a palavra "Azul"
                char *text[] = {"  Azul   "};

                // Declara e inicializa variável de altura
                int y = 0;

                // Laço de repetição que se repete ao longo do tamanho do ponteiro de char anteriormente mencionado                
                for(uint i = 0; i < count_of(text); i++){
                    // Chama a função de desenhar string enquanto o laço se repete
                    ssd1306_draw_string(ssd, 5, y, text[i]);

                    // Soma 8 a variável de altura a cada repetição
                    y += 8;
                }

                // Atualiza uma parte do display com uma área de renderização
                render_on_display(ssd, &frame_area);

                // Chama a função que toca o buzzer por 1 segundo
                beep(BUZZER_PIN, 1000);
            }
        // Verifica se apenas o Botão B foi pressionado
        }else if(gpio_get(BUTTON_B_PIN) == 0){
            // Verifica se a váriavel de controle é diferente de 2
            if(travar != 2){
                // Modifica o valor da variável de controle para 2
                travar = 2;

                // Modifica o valor da variável de controle relacionada a cor para 2 (2 = Vermelho)
                cor = 2;

                // Desliga o LED azul
                gpio_put(BLUE_LED_PIN, 0);

                // Liga o LED vermelho
                gpio_put(RED_LED_PIN, 1);

                // Laço de repetição que se repete até chegar na quantidade máxima de LEDs da matriz
                for(uint i = 0; i < LED_COUNT; i++){

                    // Atribui a cor vermelha a um LED da matriz, avançando para o próximo a cada repetição
                    npSetLED(i, 255, 0, 0);
                }

                // Escreve os dados nos LEDs.
                npWrite();

                // Declara um ponteiro para um array de char que armazena a palavra "Vermelho"
                char *text[] = {"  Vermelho   "};

                // Declara e inicializa a variável de altura
                int y = 0;

                // Laço de repetição que se repete ao longo do tamanho do ponteiro de char anteriormente mencionado
                for(uint i = 0; i < count_of(text); i++){
                    // Chama a função de desenhar string enquanto o laço se repete
                    ssd1306_draw_string(ssd, 5, y, text[i]);

                    // Soma 8 a variável de altura a cada repetição
                    y += 8;
                }
                // Atualiza uma parte do display com uma área de renderização
                render_on_display(ssd, &frame_area);

                // Chama a função que toca o buzzer por 1 segundo
                beep(BUZZER_PIN, 1000);
            }
        }
    }
    // Delay de 100 ms
    sleep_ms(100);
    
    // retorna 0 e finaliza função principal (só chega aqui quando o joystick é pressionado)
    return 0;
}