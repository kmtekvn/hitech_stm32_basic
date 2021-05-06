#include <stdint.h>
#define	BH1750_ADDR_WRITE	0x46	//01000110
#define	BH1750_ADDR_READ	0x47	//01000111

typedef enum
{
	POWER_OFF_CMD	=	0x00,	
	POWER_ON_CMD	=	0x01,	
	RESET_REGISTER	=	0x07,	
	CONT_H_MODE		=	0x10,	
	CONT_H_MODE2	=	0x11,	
	CONT_L_MODE		=	0x13,	
	ONCE_H_MODE		=	0x20,	
	ONCE_H_MODE2	=	0x21,	
	ONCE_L_MODE		=	0x23	
} BH1750_MODE;

uint8_t	BH1750_Send_Cmd(BH1750_MODE cmd);
uint16_t BH1750_Dat_To_Lux(uint8_t* dat);

