#include "ili9341_LPC4337_patch.h"  // Incluye el archivo de cabecera correspondiente

#define TICKRATE_HZ (1000)

void ili9341_gpio_init(void){
    gpioConfig(CS_PIN, GPIO_OUTPUT);        //esto lo hice basándome en el ejemplo de examples>c>sapi>gpio>switches_leds
    gpioConfig(DC_PIN, GPIO_OUTPUT);
    gpioConfig(RST_PIN, GPIO_OUTPUT);
   
   SystemCoreClockUpdate();
   SysTick_Config(SystemCoreClock / TICKRATE_HZ); // examplers>c>lpc_open>lpc_open_basic_example
}

/* 1MS Timer callback */
void SysTick_Handler(void)
{
    /* Update Display driver timers. */

    ili9341_1ms_timer_cb();
}

void ili9341_spi_init(void){
    spiConfig(SPI0);                       //esto lo hice basándome en el ejemplo de examples>c>sapi>spi>sd_card>fatfss_list
    tickConfig(10);
    tickCallbackSet(diskTickHook, NULL);
    //FSSDC_InitSPI();                // puede que esto sea solo para leer SD cards examples\c\sapi\spi\sd_card\fatfs_log_time_stamp\src\sd_spi.c

    Chip_SSP_DMA_Enable(LPC_SSP1); // libs\lpc_open\lpc_chip_43xx\inc\ssp_18xx_43xx.h
}

void diskTickHook(void *ptr)
{
    disk_timerproc(); // Disk timer process
}

void gpio_cs_pin(ili9341_gpio_pin_value_t value) {
    if (value == ILI9341_PIN_RESET) {
        gpioWrite(CS_PIN, 0);
    } else {
        gpioWrite(CS_PIN, 0);
    }
}

void gpio_dc_pin(ili9341_gpio_pin_value_t value) {
    if (value == ILI9341_PIN_RESET) {
        gpioWrite(DC_PIN, 0);
    } else {
        gpioWrite(DC_PIN, 0);
    }
}

void gpio_rst_pin(ili9341_gpio_pin_value_t value) {
    if (value == ILI9341_PIN_RESET) {
        gpioWrite(RST_PIN, 0);
    } else {
        gpioWrite(RST_PIN, 0);
    }
}

int spi_tx_dma_b(const uint8_t* data, uint32_t len) {

    bool_t result = spiWrite(SPI0, data, len);      //esto lo hice basándome en Libs>Sapi>Sapi_v0.5.2>Soc>Peripherals>src>sapi_spi.c

    if (result)
        return 0; // Éxito
    else return 1; //Fail
}

bool spi_tx_dma_ready(void) {
    //en página 1174 del datasheet

    // Código para verificar si el SPI está listo para transmitir
    return (Chip_SSP_GetStatus(LPC_SSP1, SSP_STAT_TNF) == SET);         // line160 -->  \libs\lpc_open\lpc_chip_43xx\src\ssp_18xx_43xx.c
    // return (LPC_SSP1->SR & SPI_SR_TNF) != 0; // Ejemplo: Verifica la bandera de no lleno
}

