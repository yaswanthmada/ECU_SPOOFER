/*
 * iwatch_dog.c
 *
 *  Created on: Apr 16, 2025
 *      Author: MadaYaswanth
 */

/*
 * for 1 second
 *          timeout=(Reload_value+1)*pre_scaler/Low_speed_internal_clock
 *  Reload_value[0-->4095](12-bit)
 *  Pre_scaler_value[4, 8, 16, 32, 64, 128, or 256]
 *  Low_speed_internal_clock=40khz
 * */
#include"main.h"
#include"iwatch_dog.h"
void IWDGT_INIT(){//reste for 3 seconds
	IWDG->KR = 0x5555;// 1. Enable write acces
	IWDG->PR = 0x4;// 2. Set prescaler to divide by 64
	IWDG->RLR = 1874;// 3. Set reload value
	IWDG->KR = 0xAAAA;// 4. Reload the counter
	IWDG->KR = 0xCCCC;// 5. Start the watchdog
}
