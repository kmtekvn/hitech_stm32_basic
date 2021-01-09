
#include <stdint.h>
#include <string.h>

#include "afproto.h"
#include "afproto_user.h"

#include "usart.h"

int afproto_user_create_frame(const char* orig_msg, uint32_t orig_size, char* buff, uint32_t*  write_len )
{
	return (afproto_frame_data(orig_msg, orig_size, buff, write_len));
}

#ifdef TEST_AFPROTO_USER

void afproto_user_run_test(void)
{
		const char* orig_msg = "123,456,789";
    char buff[512];
    uint32_t write_len;
	
		afproto_user_create_frame(orig_msg, strlen(orig_msg), buff, &write_len);
		
		if (write_len > 0)
		{
			__sendBufferToUart(buff, write_len);
		}
}

#endif /* TEST_AFPROTO_USER */
