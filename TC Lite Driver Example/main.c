//[*]=====================================================================================================================================[*]
#include <atmel_start.h>
#include "tc_lite_driver_example_config.h"
#include "tc_lite.h"
#include "hal_gpio.h"
#include <hal_delay.h>
//[*]=====================================================================================================================================[*]
/* Input waveform - compare value increment*/
#define CMP_VAL_INCREMENT (600U)

/* Calculated duty cycle of the input waveform in %*/
float duty;
/* Calculated frequency of the input waveform in Hz*/
float frequency;
//[*]=====================================================================================================================================[*]
void SYSTICK_TimerStart(void) {
    SysTick->VAL = 0U;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
//[*]=====================================================================================================================================[*]
/* TC channel 0 interrupt callback function */
void channel_0_cb(uint32_t status) {
    /* Toggle on board LED0 */
    gpio_toggle_pin_level(LED0);
}
//[*]=====================================================================================================================================[*]
uint16_t TC2_CH1_ComparePeriodGet(void) {
    return (uint16_t)hri_tc_read_RC_reg(TC2, 1);
}
//[*]=====================================================================================================================================[*]
uint32_t TC2_CH0_CaptureStatusGet(void) {
    return hri_tc_read_SR_reg(TC2, 0);
}
//[*]=====================================================================================================================================[*]
uint16_t TC2_CH0_CaptureAGet(void) {
    return (uint16_t)hri_tc_read_RA_reg(TC2, 0);
}
//[*]=====================================================================================================================================[*]
uint16_t TC2_CH0_CaptureBGet(void) {
    return (uint16_t)hri_tc_read_RB_reg(TC2, 0);
}
//[*]=====================================================================================================================================[*]
uint32_t TC2_CH0_CaptureFrequencyGet(void) {
    return (uint32_t)(150000000UL);
}
//[*]=====================================================================================================================================[*]
int main(void) {
    uint16_t period, on_time, off_time;
    uint16_t cmp_period, cmp_val = 0;

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    SYSTICK_TimerStart();
    cmp_period = TC2_CH1_ComparePeriodGet();

    /* Register callback function for TC Channel 0 interrupt */
    tc_register_callback(TC_LITE_DRIVER_EXAMPLE_INSTANCE, 0, channel_0_cb);

    /* Start TC channel 2 - configured in Waveform mode, generate PWM waveform and used as clock source to TC channel 0
     * and 1 */
    start_timer(TC_LITE_DRIVER_EXAMPLE_INSTANCE, 2);

    /* Start TC channel 0 - configured in Capture mode and generate periodic interrupt */
    start_timer(TC_LITE_DRIVER_EXAMPLE_INSTANCE, 0);

    /* Start TC channel 1 - configured in Waveform mode and generete PWM waveform on GPIO pin */
    start_timer(TC_LITE_DRIVER_EXAMPLE_INSTANCE, 1);

    while (1) {
        /* Change the duty cycle of the input waveform */
        cmp_val += CMP_VAL_INCREMENT;
        if (cmp_val >= cmp_period) {
            cmp_val = CMP_VAL_INCREMENT;
        }
        hri_tc_write_RB_reg(TC1, 2, cmp_val);

        /* Wait for compare event */
        while((TC2_CH0_CaptureStatusGet() & TC_SR_LDRBS_Msk) != TC_SR_LDRBS_Msk);

        /* Read Captured values */
        off_time = TC2_CH0_CaptureAGet();
        period = TC2_CH0_CaptureBGet();

        /* Compute Duty Cycle in percentage and Frequency in Hz */
        on_time = period - off_time;
        duty = ((on_time) * 100U) / period;
        frequency = (TC2_CH0_CaptureFrequencyGet() / period);

        /* Send the measured data to console for display */
        printf("Frequency of waveform: %.2f Hz \t Duty Cycle of waveform: %.2f %%", frequency, duty);
        printf("\r\n");

        /* Wait for 1 second */
        delay_ms(1000);
    }

    /* Execution should not come here during normal operation */
    return (0);
}
//[*]=====================================================================================================================================[*]