#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"
 
//Definição de entradas e saidas conectadas às GPIOs
#define RED_LED 13
#define BUTTON_A 5
#define BUTTON_B 6
#define MATRIX 7
//Definição de tempo de Debounce em ms
#define DEBOUNCE_TIME_MS 500
//Define a quantidade de leds na matriz
#define NUM_PIXELS 25 

//Variáveis globlais
int number_index = 0; //Controla qual número é exibido na matriz de LEDS
uint32_t valor_led; 
static volatile uint32_t last_time = 0;
PIO pio = pio0;
uint sm;

/**
 * Frames com desenhos dos números
 */
const int numbers_frames[10][NUM_PIXELS] = {
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0
    },//0
    {0, 0, 1, 0, 0,
     0, 0, 1, 1, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 1, 0
    },//1
    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0
    },//2
    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0
    },//3
    {0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 0, 0, 1, 0
    },//4
    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0
    },//5
    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0
    },//6
    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 0, 1, 0,
     0, 0, 0, 0, 0
    },//7
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0
    },//8
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0
    } //9
};

//Inicializa os botões A e B
void init_buttons()
{
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

// Inicializa o led
void init_led()
{
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
}

uint32_t matrix_rgb(double r, double g, double b)
{
    unsigned char R = (unsigned char)(r * 255);
    unsigned char G = (unsigned char)(g * 255);
    unsigned char B = (unsigned char)(b * 255);

    return (G << 24) | (R << 16) | (B << 8);
} 

// Mostra um dos números na matriz de leds, de acordo o valor da varivel "number_index"
void draw_number()
{
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (numbers_frames[number_index][24-i] == 1.0) {
            valor_led = matrix_rgb(0, 0, 1.0); 
        } else {
            valor_led = matrix_rgb(0, 0, 0); 
        }
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

/**
 * Função de callback para lidar com eventos dos botões 
 */
static void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if ((current_time - last_time) > DEBOUNCE_TIME_MS) //Debounce 
    {
        last_time = current_time;
        
        /**Verifica qual botão foi pressionado e decrementa (caso tenha sido A) ou incrementa (caso B)
        * em 1 o valor exibido na matriz
        */
        if (gpio == BUTTON_A) { 
            if (number_index > 0) number_index--; 
            else printf("Número mínimo alcançado.\n");
        } else if (gpio == BUTTON_B) {
            if (number_index < 9) number_index++; 
            else printf("Número máximo alcançado.\n");
        }

        printf("Indice: %d\n\n", number_index);
        draw_number();
    }
}

int main()
{
    bool ok;

    ok = set_sys_clock_khz(128000, false);
    
    stdio_init_all();
    init_led();
    init_buttons();

    if (ok) printf("Clock set to: %ld\n\n", clock_get_hz(clk_sys));    
 
    uint offset = pio_add_program(pio, &pio_matrix_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, MATRIX);

    /**
     * Aciona a função gpio_irq_handler quando um dos botões é pressionado
     */
    gpio_set_irq_enabled(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_callback(&gpio_irq_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);

    while (true) {
        /**
         * Faz o led vermelho piscar 5 vezes a cada segundo
         */
        gpio_put(RED_LED, true);
        sleep_ms(100);
        gpio_put(RED_LED, false);
        sleep_ms(100);
    }
}
