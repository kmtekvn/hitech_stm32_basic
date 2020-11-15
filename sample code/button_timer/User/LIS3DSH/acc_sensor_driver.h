#ifndef ACC_SENSOR_H_
#define ACC_SENSOR_H_

#include <stdint.h>

void acc_sensor_init(void);
void acc_sensor_reading(void);
uint32_t acc_sensor_build_frame(char* out_buf);

#endif /* ACC_SENSOR_H_ */