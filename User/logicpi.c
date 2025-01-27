#include "gd32f30x.h"
#include "logicpi.h"

/* private variables */
static uint32_t GPIO_PORT[LEDn] = {LED_R_GPIO_PORT, LED_G_GPIO_PORT, LED_B_GPIO_PORT};
static uint32_t GPIO_PIN[LEDn] = {LED_R_PIN, LED_G_PIN, LED_B_PIN};

static rcu_periph_enum COM_CLK[COMn] = {COM1_CLK};
static uint32_t COM_TX_PIN[COMn] = {COM1_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {COM1_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn] = {COM1_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {COM1_GPIO_CLK};

static rcu_periph_enum GPIO_CLK[LEDn] = {LED_R_GPIO_CLK, LED_G_GPIO_CLK, LED_B_GPIO_CLK};

static uint32_t KEY_PORT[KEYn] = {KEY1_GPIO_PORT};
static uint32_t KEY_PIN[KEYn] = {KEY1_PIN};
static rcu_periph_enum KEY_CLK[KEYn] = {KEY1_GPIO_CLK};
static exti_line_enum KEY_EXTI_LINE[KEYn] = {KEY1_EXTI_LINE};
static uint8_t KEY_PORT_SOURCE[KEYn] = {KEY1_EXTI_PORT_SOURCE};
static uint8_t KEY_PIN_SOURCE[KEYn] = {KEY1_EXTI_PIN_SOURCE};
static uint8_t KEY_IRQn[KEYn] = {KEY1_EXTI_IRQn};

/*!
    \brief      configure led GPIO
    \param[in]  lednum: specify the led to be configured
      \arg        LED_R
      \arg        LED_G
      \arg        LED_B
    \param[out] none
    \retval     none
*/
void  pi_led_init (led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */ 
    gpio_init(GPIO_PORT[lednum], GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LED_R
      \arg        LED_G
      \arg        LED_B
    \param[out] none
    \retval     none
*/
void pi_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LED_R
      \arg        LED_G
      \arg        LED_B
    \param[out] none
    \retval     none
*/
void pi_led_off(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED_R
      \arg        LED_G
      \arg        LED_B
    \param[out] none
    \retval     none
*/
void pi_led_toggle(led_typedef_enum lednum)
{
    gpio_bit_write(GPIO_PORT[lednum], GPIO_PIN[lednum], 
                    (bit_status)(1-gpio_input_bit_get(GPIO_PORT[lednum], GPIO_PIN[lednum])));
}

/*!
    \brief      configure key
    \param[in]  key_num: specify the key to be configured
      \arg        KEY: key on board
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void pi_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode)
{
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[key_num]);
    rcu_periph_clock_enable(RCU_AF);

    /* configure button pin as input */
    gpio_init(KEY_PORT[key_num], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY_PIN[key_num]);

    if (key_mode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[key_num], 2U, 0U);

        /* connect key EXTI line to key GPIO pin */
        gpio_exti_source_select(KEY_PORT_SOURCE[key_num], KEY_PIN_SOURCE[key_num]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[key_num], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[key_num]);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  key: specify the key to be checked
      \arg        KEY: key on board
    \param[out] none
    \retval     the key's GPIO pin value
*/
uint8_t pi_key_state_get(key_typedef_enum key)
{
    return gpio_input_bit_get(KEY_PORT[key], KEY_PIN[key]);
}

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        COM1: COM1 on the board
    \param[out] none
    \retval     none
*/
void pi_com_init(uint32_t com)
{
    uint32_t com_id = 0U;
    if(COM1 == com){
        com_id = 0U;
    }
    
    /* enable GPIO clock */
    rcu_periph_clock_enable(COM_GPIO_CLK[com_id]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[com_id]);

    /* connect port to USARTx_Tx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, COM_TX_PIN[com_id]);

    /* connect port to USARTx_Rx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, COM_RX_PIN[com_id]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}
