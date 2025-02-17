#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <som.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"


#define BUZZER_PIN 21

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint BUTTON_A_PIN = 5;
const uint BUTTON_B_PIN = 6;
const uint BLUE_LED_PIN = 12;
const uint RED_LED_PIN  = 13;

int main() {

    stdio_init_all();

    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    gpio_init(BLUE_LED_PIN);
    gpio_init(RED_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_put(BLUE_LED_PIN, 0);
    gpio_put(RED_LED_PIN, 0);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    pwm_init_buzzer(BUZZER_PIN);

    while (true) {
        memset(ssd, 0, ssd1306_buffer_length);
        
        if(gpio_get(BUTTON_A_PIN) == 0 && gpio_get(BUTTON_B_PIN) == 0){
            gpio_put(RED_LED_PIN, 1);
            gpio_put(BLUE_LED_PIN, 1);

            char *text[] = {"  Roxo   "};
            int y = 0;

            for(uint i = 0; i < count_of(text); i++){
                ssd1306_draw_string(ssd, 5, y, text[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);

            beep(BUZZER_PIN, 1000);

        }else if(gpio_get(BUTTON_A_PIN) == 0){
            gpio_put(RED_LED_PIN, 0);
            gpio_put(BLUE_LED_PIN, 1);

            char *text[] = {"  Azul   "};
            int y = 0;

            for(uint i = 0; i < count_of(text); i++){
                ssd1306_draw_string(ssd, 5, y, text[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);


            beep(BUZZER_PIN, 1000);
        }else if(gpio_get(BUTTON_B_PIN) == 0){
            gpio_put(BLUE_LED_PIN, 0);
            gpio_put(RED_LED_PIN, 1);

            char *text[] = {"  Vermelho   "};
            int y = 0;

            for(uint i = 0; i < count_of(text); i++){
                ssd1306_draw_string(ssd, 5, y, text[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);
            
            beep(BUZZER_PIN, 1000);
        }else{
            gpio_put(RED_LED_PIN, 0);
            gpio_put(BLUE_LED_PIN, 0);char *text[] = {
                "A para Azul",
                "B para Vermelho",
                "A e B para Roxo"};
            int y = 0;

            for(uint i = 0; i < count_of(text); i++){
                ssd1306_draw_string(ssd, 5, y, text[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);
        }
        sleep_ms(100);
    }
    return 0;
}
