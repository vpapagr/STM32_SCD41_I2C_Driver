/*
 * scd41.h
 *
 *  Created on: Mar 9, 2025
 *      Author: vpapagrigoriou
 */


#ifndef INC_SCD41_H_
#define INC_SCD41_H_

#include <main.h>

#define SCD41_ADDRESS 0x62

// Basic commands
#define START_PERIODIC_MEASUREMENT_CMD 0x21b1
#define READ_MEASUREMENT_CMD 0xec05
#define STOP_PERIODIC_MEASUREMENT_CMD 0x3f86
#define SET_TEMPERATURE_OFFSET_CMD 0x241d
#define GET_TEMPERATURE_OFFSET_CMD 0x2318
#define SET_SENSOR_ALTITUDE_CMD 0x2427
#define GET_SENSOR_ALTITUDE_CMD 0x2322
#define SET_AMBIENT_PRESSURE_CMD 0xe000
#define GET_SERIAL_NUMBER_CMD 0x3682
#define PERFORM_SELF_TEST_CMD 0x3639
#define PERFORM_FACTORY_RESET_CMD 0x3632
#define POWER_DOWN_CMD 0x36e0
#define WAKE_UP_CMD 0x36f6
#define GET_DATA_READ_STATUS_CMD 0xe4b8

#define CRC8_POLYNOMIAL 0x31
#define CRC8_INIT 0xFF


// Function prototypes
int start_periodic_measurement();
int stop_periodic_measurement();
uint8_t sensirion_common_generate_crc(const uint8_t* data, uint16_t count);
int get_data_ready_status();
int read_measurement();
#endif /* INC_SCD41_H_ */
