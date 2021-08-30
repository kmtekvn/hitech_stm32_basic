/*
 * debounce.h
 *
 *  Created on: Aug 30, 2021
 *      Author: Lenovo
 */

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include <stdint.h>

uint8_t debouce_get_btn_state (void);
void    debounce_handler (void);
void    debouce_run_test (void);

#endif /* DEBOUNCE_H_ */
