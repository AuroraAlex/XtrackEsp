#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__


#include "comm.h"
#include "esp_timer.h"

typedef void (*bsp_timer_callback_t)(void);

/**
 * @brief 定时器初始化
 *
 */
void BSP_TimerInit(void);

/**
 * @brief 设置定时时间
 *
 * @param time 定时时间，单位ms
 */
void BSP_Timer_SetTimeMs(uint64_t time);

/**
 * @brief 开启定时器
 *
 */
void BSP_Timer_Start(void);

/**
 * @brief 关闭定时器
 *
 */
void BSP_Timer_Stop(void);

/**
 * @brief 注册定时器回调函数
 *
 * @param callback 回调函数指针
 */
void BSP_Timer_RegisterCallBack(bsp_timer_callback_t callback);

/**
 * @brief 开启一次性定时器
 * 
 * @param us 定时时间，单位us
 */
void BSP_Timer_StartOneShot(uint64_t us);


#endif
