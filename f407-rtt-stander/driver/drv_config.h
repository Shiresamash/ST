/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2018-10-30     SummerGift        first version
 * 2020-10-14     Dozingfiretruck   Porting for stm32wbxx
 */

#ifndef __DRV_CONFIG_H__
#define __DRV_CONFIG_H__

#include <board.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "f4/dma_config.h"
#include "f4/uart_config.h"
#include "f4/spi_config.h"
#include "f4/qspi_config.h"
#include "f4/usbd_config.h"
#include "f4/adc_config.h"
#include "f4/dac_config.h"
#include "f4/tim_config.h"
#include "f4/sdio_config.h"
#include "f4/pwm_config.h"
#include "f4/pulse_encoder_config.h"


#ifdef __cplusplus
}
#endif

#endif
