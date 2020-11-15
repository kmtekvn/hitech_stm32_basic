
#include "spi.h"
#include "LIS3DSH.h"
#include <math.h>

#define M_PI (3.14)

/*Khai bao con tro ham de link toi ham xu ly driver tuong ung tung MCU*/
void (*SPI_Write_Reg)(uint8_t reg, uint8_t data);
uint16_t (*SPI_Read_Reg)(uint8_t reg);
void (*SPI_CS_Set)(void);
void (*SPI_CS_Reset)(void);

void LIST3DSH_register_ll_function(void);

//LIS3DSH initialization function. 
// Writes a value of 0x67 into control register to initialize accelerometer.
void LIS3DSH_init(Lis3dsh *lis3dsh) 
{
	uint8_t data = 0x67; /* 110 0111 - HR / Normal / Low-power mode (200 Hz)*/
	
	/* Dang ky ham ham driver*/
	LIST3DSH_register_ll_function();
	
	_spi_enable();
	
	/* Cau hinh ban dau cho cam bien*/
	SPI_Write_Reg(CR4, data);

	/*Khoi tao cau truc du lieu ban dau*/
	lis3dsh->accX = 0;
	lis3dsh->accY = 0;
	lis3dsh->accZ = 0;
	lis3dsh->sensitivity = 0;
}
/* Dang ky ham low level driver */
void LIST3DSH_register_ll_function(void)
{
	SPI_Read_Reg = _spi_read_reg;
	SPI_Write_Reg = _spi_write_reg;
	SPI_CS_Reset = _spi_cs_reset;
	SPI_CS_Set = _spi_cs_set;
}

//function for reading accelerometer reading on X, Y and Z axes.
uint8_t LIS3DSH_read_chip_id(void)
{
	//Start reading from X_L register. 
	//Subsequent registers hold X_H, Y_L, Y_H, and Z_L, Z_H values. 
	// Store these values in rx_buffer
	return SPI_Read_Reg(WHOAMI);
}

void LIS3DSH_readSetting(Lis3dsh *lis3dsh)
{
	uint8_t crtl = 0;
	uint8_t sensitivity = LIS302DL_SENSITIVITY_2_3G;

	crtl  = SPI_Read_Reg(LIS302DL_CTRL_REG1_ADDR);
	switch(crtl & LIS302DL_FULLSCALE_9_2) 
	  {
	    /* FS bit = 0 ==> Sensitivity typical value = 18milligals/digit*/ 
	  case LIS302DL_FULLSCALE_2_3:
	    sensitivity = LIS302DL_SENSITIVITY_2_3G;
	    break;
	    
	    /* FS bit = 1 ==> Sensitivity typical value = 72milligals/digit*/ 
	  case LIS302DL_FULLSCALE_9_2:
	    sensitivity = LIS302DL_SENSITIVITY_9_2G;
	    break;
	    
	  default:
	    break;
	  }	

	lis3dsh->sensitivity = sensitivity;
}

//function for reading accelerometer reading on X, Y and Z axes.
void LIS3DSH_readXYZ(Lis3dsh *lis3dsh)
{
	//variables used to hold raw accelerometer readings
	uint16_t x, y, z;
	uint16_t sensitivity = 1; //lis3dsh->sensitivity;
	uint8_t spi_rx_buf[6] = {0};
	
	
	//Start reading from X_L register. 
	//Subsequent registers hold X_H, Y_L, Y_H, and Z_L, Z_H values. 
	//Store these values in rx_buffer
	spi_rx_buf[0] = SPI_Read_Reg(OUTX_L);
	spi_rx_buf[1] = SPI_Read_Reg(OUTX_H);
	
	spi_rx_buf[2] = SPI_Read_Reg(OUTY_L);
	spi_rx_buf[3] = SPI_Read_Reg(OUTY_H);
	
	spi_rx_buf[4] = SPI_Read_Reg(OUTZ_L);
	spi_rx_buf[5] = SPI_Read_Reg(OUTZ_H);

	//complete accelerometer reading(signed 16 bits) is made up of 2 unsigned 8 bit readings(LOW and HIGH registers).
	//concatenate these 2 readings into a signed, 16 bit variable to construct actual value
	x = (spi_rx_buf[1] << 8) | spi_rx_buf[0] ;
	y = (spi_rx_buf[3] << 8) | spi_rx_buf[2] ;
	z = (spi_rx_buf[5] << 8) | spi_rx_buf[4] ;

	//set accelerometer readings of struct to the ones derived above, scaled by a factor of 2000 as configured.
	lis3dsh->accX = x*sensitivity;
	lis3dsh->accY = y*sensitivity;
	lis3dsh->accZ = z*sensitivity;
}

/**
  * @brief  Reboot memory content of LIS302DL.
  * @param  None
  * @retval None
  */
void LIS302DL_RebootCmd(void)
{
  uint8_t tmpreg;
  /* Read CTRL_REG2 register */
  tmpreg = SPI_Read_Reg(LIS302DL_CTRL_REG2_ADDR);
  
  /* Enable or Disable the reboot memory */
  tmpreg |= LIS302DL_BOOT_REBOOTMEMORY;
  
  /* Write value to MEMS CTRL_REG2 register */
  SPI_Write_Reg(LIS302DL_CTRL_REG2_ADDR, tmpreg);
}

//calculates pitch and roll in degrees from accelerometer readings
void LIS3DSH_calc_pitch_roll(Lis3dsh *lis3dsh, float *pitch, float *roll)
{
	*pitch = 180 * atan2(lis3dsh->accY, lis3dsh->accZ) / M_PI;
	*roll = 180 * atan2(lis3dsh->accX, lis3dsh->accZ) / M_PI;
}


