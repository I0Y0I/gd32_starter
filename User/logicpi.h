#ifndef LOGICPI_H
#define LOGICPI_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"

typedef enum
{
    LED_R = 0,
    LED_G = 1,
    LED_B = 2
} led_typedef_enum;

typedef enum
{
    KEY1 = 0
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

#define LEDn            3U
// Red LED at PB7
#define LED_R_PIN       GPIO_PIN_7
#define LED_R_GPIO_PORT GPIOB
#define LED_R_GPIO_CLK  RCU_GPIOB
// Green LED at PB7
#define LED_G_PIN       GPIO_PIN_6
#define LED_G_GPIO_PORT GPIOB
#define LED_G_GPIO_CLK  RCU_GPIOB
// Blue LED at PB7
#define LED_B_PIN       GPIO_PIN_8
#define LED_B_GPIO_PORT GPIOB
#define LED_B_GPIO_CLK  RCU_GPIOB

#define COMn            1U
// USART TX at PA9, RX at PA10
#define COM1            USART0
#define COM1_CLK        RCU_USART0
#define COM1_TX_PIN     GPIO_PIN_9
#define COM1_RX_PIN     GPIO_PIN_10
#define COM1_GPIO_PORT  GPIOA
#define COM1_GPIO_CLK   RCU_GPIOA  

#define KEYn            1U
// Key at PA8 with pull up res
#define KEY1_PIN                 GPIO_PIN_8
#define KEY1_GPIO_PORT           GPIOA
#define KEY1_GPIO_CLK            RCU_GPIOA
#define KEY1_EXTI_LINE           EXTI_8
#define KEY1_EXTI_PORT_SOURCE    GPIO_PORT_SOURCE_GPIOA
#define KEY1_EXTI_PIN_SOURCE     GPIO_PIN_SOURCE_8
#define KEY1_EXTI_IRQn           EXTI5_9_IRQn

/* function declarations */
/* configure led GPIO */
void pi_led_init(led_typedef_enum lednum);
/* turn on selected led */
void pi_led_on(led_typedef_enum lednum);
/* turn off selected led */
void pi_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void pi_led_toggle(led_typedef_enum lednum);
/* configure key */
void pi_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
uint8_t pi_key_state_get(key_typedef_enum key);
/* configure COM port */
void pi_com_init(uint32_t com);

#ifdef __cplusplus
}
#endif

#endif /* LOGICPI_H */