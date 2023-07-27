#include "bsp_timer.h"

/* 定时器实例 */
esp_timer_handle_t g_timer1;
/* 定时时间 ms */
uint64_t g_timer1_time_ms = 0U;
/* 初始化标志 */
bool g_timer_init_flag = false;

/* 定时器回调 */
bsp_timer_callback_t g_timer1_callback = NULL;

void BSP_Timer_CallBack(void *arg)
{
    (void)arg;
    if(NULL!=g_timer1_callback)
    {
        g_timer1_callback();
    }
    else
    {
        // do nothing
    }
}

void BSP_TimerInit(void)
{
    esp_timer_create_args_t timer_creat_args = {
        .callback = BSP_Timer_CallBack,
        .arg = NULL,
        .name = "BSP_TIMER1",
        .skip_unhandled_events = false
    };
    
    if (false == g_timer_init_flag)
    {
       esp_timer_create(&timer_creat_args, &g_timer1);
       g_timer_init_flag = true;
    }
    
}

void BSP_Timer_SetTimeMs(uint64_t time)
{
    g_timer1_time_ms = time;
}

// 开启定时器
void BSP_Timer_Start(void)
{
    //大于50us的定时避免CPU占用过高
    if (0U> g_timer1_time_ms)
    {
        g_timer1_time_ms = 1U;
    }
    esp_timer_start_periodic(g_timer1,g_timer1_time_ms);
}


// 开启定时器
void BSP_Timer_StartOneShot(uint64_t us)
{
    //大于50us的定时避免CPU占用过高
    if (50U> us)
    {
        esp_timer_start_once(g_timer1,50U);
    }
    else
    {
        esp_timer_start_once(g_timer1,us);
    }
    
}


void BSP_Timer_Stop(void)
{
    esp_timer_stop(g_timer1);
}


// 注册定时器中断回调
void BSP_Timer_RegisterCallBack(bsp_timer_callback_t callback)
{
    g_timer1_callback = callback;
}
