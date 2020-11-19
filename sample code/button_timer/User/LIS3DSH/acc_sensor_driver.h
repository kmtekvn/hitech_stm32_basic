#ifndef ACC_SENSOR_H_
#define ACC_SENSOR_H_

#include <stdint.h>
#include "LIS3DSH.h"

void acc_sensor_init(void);
void acc_sensor_reading(void);
uint32_t acc_sensor_build_frame(char* out_buf);
void __acc_sensor_led_indicator(Lis3dsh *acc_data);
Lis3dsh* acc_get_data_pointer(void);

#endif /* ACC_SENSOR_H_ */