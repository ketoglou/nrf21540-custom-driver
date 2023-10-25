/**
 ******************************************************************************
 *	@file		: fem_nrf21540.c
 *	@author		: ketoglou
 ******************************************************************************
 */

#include "fem_nrf21540.h"
#include <zephyr/kernel.h>
#include <hal/nrf_gpio.h>
#include <zephyr/drivers/spi.h>
#include "uart.h"

//NRF21540 PINS
#define TX_EN_PIN       25
#define RX_EN_PIN       19
#define PDN_PIN         23
#define ANT_SEL_PIN     20
#define MODE_PIN        17

//NRF21540 REGISTERS
#define READ_REG    0x80
#define WRITE_REG   0xC0
#define CONFREG0    0x00
#define CONFREG1    0x01
#define CONFREG2    0x02
#define CONFREG3    0x03
#define PARTNUMBER  0x14
#define HW_REVISION 0x15
#define HW_ID0      0x16
#define HW_ID1      0x17


//SPI
#define SPI_OP  SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER
const struct spi_dt_spec nrf21540_dev = SPI_DT_SPEC_GET(DT_NODELABEL(nrf_radio_fem_spi), SPI_OP, 0);

//SPI RX Buffers
uint8_t nrf21540_rx_buff[2] = {0};
struct spi_buf spi_rx_buff[1];
const struct spi_buf_set rx_buff = { spi_rx_buff, 1 };
//SPI TX Buffers
uint8_t nrf21540_tx_buff[2] = {0};
struct spi_buf spi_tx_buff[1];
const struct spi_buf_set tx_buff = { spi_tx_buff, 1 };


//*******************************************************************************************

void fem_nrf21540_init(void){

    //Chip in power off state
    nrf_gpio_cfg(PDN_PIN, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_clear(PDN_PIN);

    //Disable TX
	nrf_gpio_cfg(TX_EN_PIN, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_clear(TX_EN_PIN);

    //Disable RX
    nrf_gpio_cfg(RX_EN_PIN, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_clear(RX_EN_PIN);
    
    //Enable ANT1
    nrf_gpio_cfg(ANT_SEL_PIN, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_clear(ANT_SEL_PIN);

    //Mode to POUTA
    nrf_gpio_cfg(MODE_PIN, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_clear(MODE_PIN);

    spi_tx_buff[0].buf = nrf21540_tx_buff;
    spi_tx_buff[0].len = 2;

    spi_rx_buff[0].buf = nrf21540_rx_buff;
    spi_rx_buff[0].len = 2;

    if(!spi_is_ready_dt(&nrf21540_dev)){
        printk("SPI not ready\n");
    }

    printk("FEM initialized\n");
}

//*******************************************************************************************

void fem_nrf21540_power_on(bool enable){
    if(enable){
        nrf_gpio_pin_set(PDN_PIN);
        //Wait to power up
        k_sleep(K_MSEC(1));
    }else{
	//If the chip is powered down then any changes in tx gain is reseted
        nrf_gpio_pin_clear(PDN_PIN);
    }
}

//*******************************************************************************************

void fem_nrf21540_tx_enable(bool enable){
	if(enable){
        nrf_gpio_pin_set(TX_EN_PIN);
    }else{
        nrf_gpio_pin_clear(TX_EN_PIN);
    }
}

//*******************************************************************************************

void fem_nrf21540_rx_enable(bool enable){
	if(enable){
        nrf_gpio_pin_set(RX_EN_PIN);
    }else{
        nrf_gpio_pin_clear(RX_EN_PIN);
    }
}

//*******************************************************************************************

void fem_nrf21540_antenna_sel(enum ant antenna){
    if(antenna == ANTENNA_1){
        nrf_gpio_pin_clear(ANT_SEL_PIN);
    }else{
        nrf_gpio_pin_set(ANT_SEL_PIN);
    }
}

//*******************************************************************************************

void fem_nrf21540_mode_sel(enum mode mode_){
    if(mode_ == TX_GAIN_POUTA){
        nrf_gpio_pin_clear(MODE_PIN);
    }else{
        nrf_gpio_pin_set(MODE_PIN);
    }
}

//*******************************************************************************************

bool fem_nrf21540_set_power(uint8_t power){
    
    if(power <= 31){
        int ret;
        nrf21540_tx_buff[0] = WRITE_REG | CONFREG0;
        nrf21540_tx_buff[1] = power << 2;

        ret = spi_write_dt(&nrf21540_dev, &tx_buff);
        if(ret){
            printk("Error SPI Write\n");
        }

        //SPI write verification (Check rx_buff[1])
        // nrf21540_tx_buff[0] = READ_REG | CONFREG0;
        // nrf21540_tx_buff[1] = 0;

        // ret = spi_transceive_dt(&nrf21540_dev, &tx_buff, &rx_buff);
        // if(ret){
        //     uart_printk("Error SPI Read");
        // }
        return true;
    }
    return false;
}

//*******************************************************************************************