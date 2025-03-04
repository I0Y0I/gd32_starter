/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "logicpi.h"
#include "retarget.h"

/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if(timingdelaylocal){

        if(timingdelaylocal < 500U){
            pi_led_on(LED_R);
        }else{
            pi_led_off(LED_R);
        }

        timingdelaylocal--;
    }else{
        timingdelaylocal = 1000U;
    }
}

int main() {
    // configure systick
    systick_config();
    // initilize the LEDs, USART and key
    pi_led_init(LED_R);
    pi_led_init(LED_G);
    pi_led_init(LED_B);
    pi_key_init(KEY1, KEY_MODE_GPIO);

    semihosting_init(COM1);

    // print out the clock frequency of system, AHB, APB1 and APB2
    printf("\r\nCK_SYS is %ld", rcu_clock_freq_get(CK_SYS));
    printf("\r\nCK_AHB is %ld", rcu_clock_freq_get(CK_AHB));
    printf("\r\nCK_APB1 is %ld", rcu_clock_freq_get(CK_APB1));
    printf("\r\nCK_APB2 is %ld", rcu_clock_freq_get(CK_APB2));
    
    // main loop
    while(1) {
        if (RESET == pi_key_state_get(KEY1))
        {
            pi_led_on(LED_G);
            delay_1ms(500);
            pi_led_off(LED_G);
            pi_led_toggle(LED_B);
        }
    }
}