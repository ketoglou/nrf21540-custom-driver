/**
 ******************************************************************************
 *	@file		: fem_nrf21540.h
 *	@author		: ketoglou
 ******************************************************************************
 */

#ifndef FEM_NRF21540_H_
#define FEM_NRF21540_H_

#include <stdint.h>
#include <stdbool.h>

//Antenna enum
enum ant {
  ANTENNA_1 = 0,
  ANTENNA_2 = 1
}; 

//Mode enum
enum mode{
    TX_GAIN_POUTA = 0,
    TX_GAIN_POUTB = 1
};

/**
 * @fn void fem_nrf21540_init(void)
 * @brief Initialize the pins of the fem
 */
void fem_nrf21540_init(void);

/**
 * @fn void fem_nrf21540_power_on(bool enable)
 * @brief Power on/off nrf21540.
 *
 * @param enable
 */
void fem_nrf21540_power_on(bool enable);

/**
 * @fn void fem_nrf21540_tx_enable(bool enable)
 * @brief Enable/Disable TX.
 *
 * @param enable
 */
void fem_nrf21540_tx_enable(bool enable);

/**
 * @fn void fem_nrf21540_rx_enable(bool enable)
 * @brief Enable/Disable RX.
 *
 * @param enable
 */
void fem_nrf21540_rx_enable(bool enable);

/**
 * @fn void fem_nrf21540_antenna_sel(enum ant antenna)
 * @brief Select antenna.
 *
 * @param antenna
 */
void fem_nrf21540_antenna_sel(enum ant antenna);

/**
 * @fn fem_nrf21540_mode_sel(enum mode mode_)
 * @brief Select mode.
 *
 * @param mode_
 */
void fem_nrf21540_mode_sel(enum mode mode_);

/**
 * @fn bool fem_nrf21540_set_power(uint8_t power)
 * @brief Set power level (min = 0, max = 31)
 *
 * @param power
 */
bool fem_nrf21540_set_power(uint8_t power);

#endif