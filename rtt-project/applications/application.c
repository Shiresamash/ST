/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006-2014, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup.
 */

#include <board.h>
#include <rtthread.h>

static struct rt_thread thread1;
static char thread1_stack[512];
static void thread1_entry(void* parameter)
{
    int count=0;
    while(1)
    {
        rt_kprintf("thread1 count: %d\r\n", count);
        count++;

        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}

static struct rt_thread thread2;
static char thread2_stack[512];
static void thread2_entry(void* parameter)
{
    int count=0;

    /* PF9: LED1 */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

        /* Configure UART Rx/tx PIN */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_Init(GPIOF, &GPIO_InitStructure);
    }

    while(1)
    {
        rt_kprintf("thread2 count: %d\r\n", count);
        count++;

        /* led off 2s. */
        GPIO_SetBits(GPIOF, GPIO_Pin_9);
        rt_thread_delay(RT_TICK_PER_SECOND*2);

        /* led on 0.1s. */
        GPIO_ResetBits(GPIOF, GPIO_Pin_9);
        rt_thread_delay(RT_TICK_PER_SECOND/10);
    }
}

int rt_application_init(void)
{
    rt_err_t result;

    result = rt_thread_init(&thread1,
                            "thread1",
                            thread1_entry,
                            RT_NULL,
                            &thread1_stack[0],
                            sizeof(thread1_stack),
                            1,
                            1);

    if (result == RT_EOK)
        rt_thread_startup(&thread1);

    result = rt_thread_init(&thread2,
                            "thread2",
                            thread2_entry,
                            RT_NULL,
                            &thread2_stack[0],
                            sizeof(thread2_stack),
                            2,
                            1);

    if (result == RT_EOK)
        rt_thread_startup(&thread2);

    return RT_EOK;
}

/*@}*/
