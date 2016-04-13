/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <stdio.h>


#include <board.h>
#include <rtthread.h>
#include "NUC472_442.h"
#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32_eth.h"
#endif
#include <stdio.h>
#include "gpio.h"
#include "ade7878_Int.h" 
#include"Usart.h"
#include "ADE7878_Measure.h"
#include"Config.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

void rt_init_thread_entry(void* parameter)
{
 eMBErrorCode    eStatus;
 eStatus = eMBInit( MB_RTU, 2, 0,9600, MB_PAR_NONE );
 /* Enable the Modbus Protocol Stack. */
 eStatus = eMBEnable();
//FS
//GUI
}

ALIGN(RT_ALIGN_SIZE)
static char thread_PowerMeasurment_stack[1024];
struct rt_thread thread_PowerMeasurment;
extern void PowerMeasurment(void);
static void rt_thread_entry_PowerMeasurment(void* parameter)
{
    while (1)
    {	 
	   //50ms采集一次电能数据并显示
	    PB13=0;
        rt_thread_delay(25); 
		PowerMeasurment();
		PB13=1;
       	rt_thread_delay(25);  
    }
}
ALIGN(RT_ALIGN_SIZE)
static char thread_Rs485comm_stack[1024];
struct rt_thread thread_Rs485comm;
static void rt_thread_entry_Rs485comm(void* parameter)
{
  
    while(1)
    {

		//10MS采集一次485数据
	    PB12=0;
      	(void)eMBPoll();
		PB12=1;
        rt_thread_delay(10);		

    }
}

int rt_application_init()
{
    rt_thread_t init_thread;

#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 80, 20);
#endif

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    //------- init PowerMeasurment thread
    rt_thread_init(&thread_PowerMeasurment,
                   "PowerMeasurment",
                   rt_thread_entry_PowerMeasurment,
                   RT_NULL,
                   &thread_PowerMeasurment_stack[0],
                   sizeof(thread_PowerMeasurment_stack),11,5);
    rt_thread_startup(&thread_PowerMeasurment);

    //------- init Rs485comm thread
    rt_thread_init(&thread_Rs485comm,
                   "Rs485comm",
                   rt_thread_entry_Rs485comm,
                   RT_NULL,
                   &thread_Rs485comm_stack[0],
                   sizeof(thread_Rs485comm_stack),10,5);
    rt_thread_startup(&thread_Rs485comm);

    return 0;
}

/*@}*/
