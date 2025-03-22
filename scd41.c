#include <scd41.h>


int start_periodic_measurement()
{
	uint16_t command = START_PERIODIC_MEASUREMENT_CMD;
	uint8_t data[2] = {0};
    int result;

    /* Prepare the command */
	data[0] = command >> 8 & 0xff;
    data[1] = command & 0xff;

	result = HAL_I2C_Master_Transmit(&hi2c1,(SCD41_ADDRESS << 1), data, 2, 1000);
	if (result!=HAL_OK)
		return -1; //error
	else
		return 0; //Success
}

int stop_periodic_measurement()
{
	uint16_t command = STOP_PERIODIC_MEASUREMENT_CMD;
	uint8_t data[2] = {0};
    int result;

    /* Prepare the command */
	data[0] = command >> 8 & 0xff;
    data[1] = command & 0xff;

	result = HAL_I2C_Master_Transmit(&hi2c1,(SCD41_ADDRESS << 1), data, 2, 1000);
	if (result!=HAL_OK)
		return -1; //error
	else
	{
		HAL_Delay(500);
		return 0; //Success
	}
}


/* Function to calculate CRC */
uint8_t sensirion_common_generate_crc(const uint8_t* data, uint16_t count)
{
    uint8_t crc = CRC8_INIT;
    for (uint16_t i = 0; i < count; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

/* Function to check if measurement data is ready */
int get_data_ready_status()
{
    uint8_t cmd[2] = { (GET_DATA_READ_STATUS_CMD >> 8) & 0xFF, GET_DATA_READ_STATUS_CMD & 0xFF }; // Convert to big-endian
    uint8_t response[3] = {0}; // 2 bytes status + 1 byte CRC
    int result;

    // Send command
    result = HAL_I2C_Master_Transmit(&hi2c1, (SCD41_ADDRESS << 1), cmd, 2, 1000);
    if (result != HAL_OK) {
        printf("I2C Transmit Error\n");
        return -1;
    }

    HAL_Delay(1); // Wait for sensor to process

    // Read response
    result = HAL_I2C_Master_Receive(&hi2c1, (SCD41_ADDRESS << 1), response, 3, 1000);
    if (result != HAL_OK) {
        printf("I2C Receive Error\n");
        return -1;
    }

    // Verify CRC
    if (sensirion_common_generate_crc(response, 2) != response[2]) {
        printf("CRC Error\n");
        return -1;
    }

    // Extract the 16-bit status
    uint16_t status = (response[0] << 8) | response[1];

    // Check if data is ready (status[10:0] != 0)
    if ((status & 0x07FF) == 0) {
        return -1;
    } else {
        return 0;
    }
}


int read_measurement()
{
	uint16_t command = READ_MEASUREMENT_CMD;
	uint8_t data[2] = {0};
	uint16_t word[3]={0};
	double co2=0.0;
	double temp=0.0;
	double rh=0.0;
	int result=0;

	if(get_data_ready_status()==-1)
		return -1;

	data[0] = command >> 8 & 0xff;
	data[1] = command & 0xff;


	result = HAL_I2C_Master_Transmit(&hi2c1, SCD41_ADDRESS << 1, data, 2, 1000);
	if (result!= HAL_OK)
		return -1;

	uint8_t response[9]; // 3 data words + 3 CRC bytes
	result = HAL_I2C_Master_Receive(&hi2c1, SCD41_ADDRESS << 1, response, 9, 1000);
	if (result!= HAL_OK)
			return -1;

	word[0] = (response[0] << 8) | response[1];  // CO2
	word[1] = (response[3] << 8) | response[4];  // Temperature
	word[2] = (response[6] << 8) | response[7];  // Humidity


	co2=word[0];
	temp=-45 + 175*word[1]/ pow(2,16)-1;
	rh = 100*word[2]/ pow(2,16)-1;

	printf("co2=%f \n",co2);
	printf("temp=%f \n",temp);
	printf("rh = %f\n",rh);

	return 0;

}
