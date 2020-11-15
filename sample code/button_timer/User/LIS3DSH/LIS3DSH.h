#ifndef LIS3DSH_LIS3DSH_H_
#define LIS3DSH_LIS3DSH_H_

#include <stdint.h>

//LIS3DSH REGISTERS
#define WHOAMI 0x0F

#define CR4 0x20
#define FIFO_CTRL 0x2E
#define SR 0x27

#define OUTX_L 0x28
#define OUTX_H 0x29
#define OUTY_L 0x2A
#define OUTY_H 0x2B
#define OUTZ_L 0x2C
#define OUTZ_H 0x2D

/** @defgroup Full_Scale_selection 
  * @{
  */
#define LIS302DL_FULLSCALE_2_3                            ((uint8_t)0x00)
#define LIS302DL_FULLSCALE_9_2                            ((uint8_t)0x20)

#define LIS302DL_SENSITIVITY_2_3G                         18  /* 18 mg/digit*/
#define LIS302DL_SENSITIVITY_9_2G                         72  /* 72 mg/digit*/

/*******************************************************************************
*  CTRL_REG2 Regsiter: Control Register 2
*  Read Write register
*  Default value: 0x00
*  7 SIM: SPI Serial Interface Mode Selection. 
*         0 - 4 wire interface
*         1 - 3 wire interface
*  6 BOOT: Reboot memory content
*          0 - normal mode 
*          1 - reboot memory content
*  5 Reserved
*  4 FDS: Filtered data selection.
*         0 - internal filter bypassed
*         1 - data from internal filter sent to output register
*  3 HP FF_WU2: High pass filter enabled for FreeFall/WakeUp#2.
*               0 - filter bypassed
*               1 - filter enabled
*  2 HP FF_WU1: High pass filter enabled for FreeFall/WakeUp#1.
*               0 - filter bypassed
*               1 - filter enabled
*  1:0 HP coeff2-HP coeff1 High pass filter cut-off frequency (ft) configuration.
*                 ft= ODR[hz]/6*HP coeff
*            HP coeff2 | HP coeff1 |   HP coeff
*            -------------------------------------------
*                 0     |     0     |   8
*                 0     |     1     |   16
*                 1     |     0     |   32
*                 1     |     1     |   64
*            HP coeff |  ft[hz]   |  ft[hz]   |
*                     |ODR 100Hz | ODR 400Hz  |
*            --------------------------------------------
*              00     |    2      |     8     |
*              01     |    1      |     4     |
*              10     |    0.5    |     2     |
*              11     |    0.25   |     1     |
*******************************************************************************/
#define LIS302DL_CTRL_REG2_ADDR              0x21

 /** @defgroup Boot_Mode_selection 
  * @{
  */
#define LIS302DL_BOOT_NORMALMODE                          ((uint8_t)0x00)
#define LIS302DL_BOOT_REBOOTMEMORY                        ((uint8_t)0x40)

/*******************************************************************************
*  CTRL_REG1 Register: Control Register 1
*  Read Write register
*  Default value: 0x07
*  7 DR: Data Rate selection.
*        0 - 100 Hz output data rate
*        1 - 400 Hz output data rate
*  6 PD: Power Down control.
*        0 - power down mode
*        1 - active mode
*  5 FS: Full Scale selection.
*        0 - Typical measurement range 2.3
*        1 - Typical measurement range 9.2
*  4:3 STP-STM Self Test Enable:
*              STP |  STM |   mode
*            ----------------------------
*               0  |  0   |   Normal mode
*               0  |  1   |   Self Test M
*               1  |  0   |   Self Test P
*  2 Zen: Z axis enable.
*         0 - Z axis disabled
*         1- Z axis enabled
*  1 Yen: Y axis enable.
*         0 - Y axis disabled
*         1- Y axis enabled
*  0 Xen: X axis enable.
*         0 - X axis disabled
*         1- X axis enabled
*******************************************************************************/
#define LIS302DL_CTRL_REG1_ADDR                 0x20

//struct for holding LIS3DSH acceleration data.
typedef struct Lis3dsh_t {
	uint16_t accX;
	uint16_t accY;
	uint16_t accZ;
	uint16_t sensitivity;
} Lis3dsh;



//LIS3DSH function prototypes for initialization, 
// accelerometer reading, and pitch/roll calculation
void LIS3DSH_init(Lis3dsh *lis3dsh);
void LIS3DSH_readSetting(Lis3dsh *lis3dsh);
void LIS3DSH_readXYZ(Lis3dsh *lis3dsh);
void LIS3DSH_calc_pitch_roll(Lis3dsh *lis3dsh, float *pitch, float *roll);
uint8_t LIS3DSH_read_chip_id(void);

#endif /* LIS3DSH_LIS3DSH_H_ */
