#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
cyhal_pwm_t pwm_obj;
int x=100;
static void button_isr1(void *handler_arg, cyhal_gpio_event_t event1){
 cyhal_gpio_toggle(CYBSP_USER_LED);
}
cyhal_gpio_callback_data_t cb_data1 = {
.callback = button_isr1,
.callback_arg = NULL
};
static void button_isr2(void *handler_arg, cyhal_gpio_event_t event2){
 for(int i=0;i<10;i++){
  cyhal_gpio_toggle(P9_1);
  cyhal_system_delay_ms(250);
 }
}
cyhal_gpio_callback_data_t cb_data2 = {
.callback = button_isr2,
.callback_arg = NULL
};
static void button_isr3(void *handler_arg, cyhal_gpio_event_t event3){
 cyhal_system_delay_ms(250);
 x-=20;
 cyhal_pwm_set_duty_cycle(&pwm_obj,x,5000);
  if(x==0){
   x=120;
  }
}
cyhal_gpio_callback_data_t cb_data3 = {
.callback = button_isr3,
.callback_arg = NULL
};
int main(void)
{
    cy_rslt_t result;
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    __enable_irq();
    cyhal_gpio_init (CYBSP_USER_LED , CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG , 1);
    cyhal_gpio_init (P9_1 , CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    //cyhal_gpio_init (P9_2 , CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT,CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
    cyhal_gpio_init(P9_4, CYHAL_GPIO_DIR_INPUT,CYHAL_GPIO_DRIVE_PULLUP, 1);
    cyhal_gpio_init(P9_7, CYHAL_GPIO_DIR_INPUT,CYHAL_GPIO_DRIVE_PULLUP, 1);
    cyhal_gpio_register_callback(CYBSP_USER_BTN, &cb_data1);
    cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL,1, true);
    cyhal_gpio_register_callback(P9_4, &cb_data2);
    cyhal_gpio_enable_event(P9_4, CYHAL_GPIO_IRQ_FALL,2, true);
    cyhal_gpio_register_callback(P9_7, &cb_data3);
    cyhal_gpio_enable_event(P9_7, CYHAL_GPIO_IRQ_FALL,3, true);
    cyhal_pwm_init(&pwm_obj, P9_2, NULL);
    cyhal_pwm_set_duty_cycle(&pwm_obj,100,5000);
    cyhal_pwm_start(&pwm_obj);
    for (;;)
    {
    }
}

