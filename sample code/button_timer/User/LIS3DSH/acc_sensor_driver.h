#ifndef ACC_SENSOR_H_
#define ACC_SENSOR_H_

#include <stdint.h>
#include "LIS3DSH.h"

typedef struct 
{
	int16_t XMovement;
	int16_t YMovement;
	uint8_t clicked;
}MovementStruct_t;

void acc_sensor_init(void);
void acc_sensor_reading(void);
uint32_t acc_sensor_build_frame(MovementStruct_t* mov, char* out_buf);
void __acc_sensor_led_indicator(Lis3dsh *acc_data);
void __acc_send_dataframe(Lis3dsh *acc_data);
Lis3dsh* acc_get_data_pointer(void);
void acc_sensor_process_movement(Lis3dsh *acc_data, MovementStruct_t *movement);

#endif /* ACC_SENSOR_H_ */