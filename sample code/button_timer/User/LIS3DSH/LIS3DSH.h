#ifndef LIS3DSH_LIS3DSH_H_
#define LIS3DSH_LIS3DSH_H_

#include <stdint.h>

//LIS3DSH REGISTERS
#define WHOAMI 0x0F

#define CR4 0x20
#define SR 0x27

#define OUTX_L 0x28
#define OUTX_H 0x29
#define OUTY_L 0x2A
#define OUTY_H 0x2B
#define OUTZ_L 0x2C
#define OUTZ_H 0x2D

//struct for holding LIS3DSH acceleration data.
typedef struct Lis3dsh_t {
	float accX;
	float accY;
	float accZ;

} Lis3dsh;



//LIS3DSH function prototypes for initialization, 
// accelerometer reading, and pitch/roll calculation
void LIS3DSH_init(Lis3dsh *lis3dsh);
void LIS3DSH_readXYZ(Lis3dsh *lis3dsh);
void LIS3DSH_calc_pitch_roll(Lis3dsh *lis3dsh, float *pitch, float *roll);
uint8_t LIS3DSH_read_chip_id(void);

#endif /* LIS3DSH_LIS3DSH_H_ */
