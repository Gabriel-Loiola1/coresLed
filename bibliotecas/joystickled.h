#include <stdio.h>
#include "bibliotecas/neopixel.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

void read_x_y(uint16_t *eixo_x, uint16_t *eixo_y, uint16_t channel1, uint16_t channel2){
    // Seleciona a entrada ADC (conectada ao eixo X do joystick)
    adc_select_input(channel1);
    // Pequeno delay para estabilidade
    sleep_ms(200);
    // Lê o valor do ADC para o eixo X
    *eixo_x = adc_read();
    
    // Seleciona a entrada ADC (conectada ao eixo Y do joystick)
    adc_select_input(channel2);
    // Pequeno delay para estabilidade
    sleep_ms(200);
    // Lê o valor do ADC para o eixo Y
    *eixo_y = adc_read();
}

// Função para preencher a matriz de LEDs antes de mudar o desenho
void preencher(int cor){
    // Verifica se a matriz está na cor azul
    if(cor == 1){
        // Laço de repetição que se repete até chegar na quantidade máxima de LEDs da matriz
        for(uint i = 0; i < 25; i++){
            // Atribui a cor azul a um LED da matriz, avançando para o próximo a cada repetição
            npSetLED(i, 0, 0, 255);
        }

        // Escreve os dados nos LEDs.
        npWrite();
    
    // Verifica se a matriz está na cor vermelha
    }else if(cor == 2){
        // Laço de repetição que se repete até chegar na quantidade máxima de LEDs da matriz
        for(uint i = 0; i < 25; i++){
            // Atribui a cor vermelga a um LED da matriz, avançando para o próximo a cada repetição
            npSetLED(i, 255, 0, 0);
        }

        // Escreve os dados nos LEDs.
        npWrite();
    
    // Verifica se a matriz está na cor roxa
    }else if(cor == 3){
        // Laço de repetição que se repete até chegar na quantidade máxima de LEDs da matriz
        for(uint i = 0; i < 25; i++){
            // Atribui a cor roxa a um LED da matriz, avançando para o próximo a cada repetição
            npSetLED(i, 255, 0, 255);
        }

        // Escreve os dados nos LEDs.
        npWrite();
    }
}

// Função para mudar o desenho da matriz de LEDs de acordo com a posição do Joystick
void mudar_desenho(uint16_t *eixo_x, uint16_t *eixo_y, int cor){
    // Verifica se o Joystick foi para a esquerda
    if((*eixo_x > 2580)){
        // Chama a função de preencher
        preencher(cor);

        // Delay de 50ms
        sleep_ms(50);

        // Desenha um coração desligando os LEDs que não vão ser utilizados
        npSetLED(0, 0, 0, 0);
        npSetLED(1, 0 , 0, 0);
        npSetLED(3, 0 , 0, 0);
        npSetLED(4, 0 , 0, 0);
        npSetLED(5, 0 , 0, 0);
        npSetLED(9, 0 , 0, 0);
        npSetLED(20, 0 , 0, 0);
        npSetLED(22, 0 , 0, 0);
        npSetLED(24, 0 , 0, 0);
        npWrite();
    
    // Verifica se o Joystick foi para a direita
    }else if(*eixo_x < 1580){
        // Chama a função de preencher
        preencher(cor);

        // Delay de 50ms
        sleep_ms(50);

        // Desenha uma cara feliz desligando os LEDs que não vão ser utilizados
        npSetLED(0, 0 , 0, 0);
        npSetLED(4, 0 , 0, 0);
        npSetLED(6, 0, 0, 0);
        npSetLED(7, 0 , 0, 0);
        npSetLED(8, 0 , 0, 0);
        npSetLED(10, 0 , 0, 0);
        npSetLED(11, 0 , 0, 0);
        npSetLED(12, 0 , 0, 0);
        npSetLED(13, 0 , 0, 0);
        npSetLED(14, 0 , 0, 0);
        npSetLED(17, 0 , 0, 0);
        npSetLED(22, 0 , 0, 0);
        npWrite();

    // Verifica se o Joystick foi para cima
    }else if(*eixo_y > 2500){
        // Chama a função de preencher
        preencher(cor);
        
        // Delay de 50ms
        sleep_ms(50);

        // Desenha uma estrela desligando os LEDs que não vão ser utilizados
        npSetLED(1, 0 , 0, 0);
        npSetLED(2, 0 , 0, 0);
        npSetLED(3, 0 , 0, 0);
        npSetLED(5, 0 , 0, 0);
        npSetLED(9, 0 , 0, 0);
        npSetLED(15, 0 , 0, 0);
        npSetLED(16, 0 , 0, 0);
        npSetLED(18, 0 , 0, 0);
        npSetLED(19, 0 , 0, 0);
        npSetLED(20, 0 , 0, 0);
        npSetLED(21, 0 , 0, 0);
        npSetLED(23, 0 , 0, 0);
        npSetLED(24, 0 , 0, 0);
        npWrite();
    
    // Verifica se o Joystick foi para baixo
    }else if(*eixo_y < 1500){
        // Chama a função de preencher
        preencher(cor);

        // Delay de 50ms
        sleep_ms(50);
        
        // Desenha um símbolo os LEDs que não vão ser utilizados
        npSetLED(1, 0 , 0, 0);
        npSetLED(3, 0 , 0, 0);
        npSetLED(5, 0 , 0, 0);
        npSetLED(9, 0 , 0, 0);
        npSetLED(15, 0 , 0, 0);
        npSetLED(19, 0 , 0, 0);
        npSetLED(21, 0 , 0, 0);
        npSetLED(23, 0 , 0, 0);
        npWrite();
    }
}