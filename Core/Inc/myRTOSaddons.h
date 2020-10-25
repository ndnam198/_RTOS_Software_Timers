/**
 * @file myRTOSaddons.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __MY_RTOS_ADDONS
#define __MY_RTOS_ADDONS

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "task.h"
#include "timers.h"

#include "main.h"
#include "myLib.h"

extern char ucGeneralString[100];

#define PRINT_IN_TASK(str)                                                       \
    do                                                                           \
    {                                                                            \
        sprintf(ucGeneralString, "[%s] - %s", pcTaskGetName(NULL), (char *)str); \
        vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString);                      \
    } while (0)

#define PRINT_VAR_IN_TASK(var)                                                       \
    do                                                                               \
    {                                                                                \
        sprintf(ucGeneralString, "[%s] - " #var " = %lu", pcTaskGetName(NULL), var); \
        vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString);                          \
        newline;                                                                     \
    } while (0)

#define PRINT_IN_SWTIMER(timer_id, str)                                                       \
    do                                                                                        \
    {                                                                                         \
        sprintf(ucGeneralString, "[%s] - %s", osTimerGetName((void *)timer_id), (char *)str); \
        vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString);                                   \
    } while (0)

#define PRINT_VAR_INSWTIMER(timer_id, var)                                                        \
    do                                                                                            \
    {                                                                                             \
        sprintf(ucGeneralString, "[%s] - " #var " = %lu", osTimerGetName((void *)timer_id), var); \
        vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString);                                       \
        newline;                                                                                  \
    } while (0)

#endif /* !__MY_RTOS_ADDONS */
