#include "comm.h"
#include "bsp_timer.h"
#include "bsp_touch_pin_button.h"

#define MUTEX
/*Change to your screen resolution*/
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight];
//互斥锁
static SemaphoreHandle_t lvgl_mutex;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif


void timer_test_log(void)
{
    static uint8_t count = 0U;
    Serial.println("timer_test_log");
    if (10U == count)
    {
        BSP_Timer_Stop();
    }
    else
    {
        count++;
    }
    
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/// 定时检测任务
void lv_timer_handler_task(void *pt)
{
    (void)pt;
    while (1)
    {  
        #ifdef MUTEX
        xSemaphoreTake(lvgl_mutex,portMAX_DELAY);
        lv_tick_inc(1);
        lv_task_handler();
        xSemaphoreGive(lvgl_mutex);
        vTaskDelay(pdMS_TO_TICKS(1));
        #endif
    }
}


void setup()
{
    Serial.begin(9600); /* prepare for possible serial debug */

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println(LVGL_Arduino);
    Serial.println("I am LVGL_Arduino");

    //初始化lvgl互斥锁
    lvgl_mutex = xSemaphoreCreateMutex();


    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin();        /* TFT init */
    tft.setRotation(3); /* Landscape orientation, flipped */

    /*Set the touchscreen calibration data,
     the actual data for your display can be acquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    // uint16_t calData[5] = {275, 3620, 264, 3532, 1};
    // tft.setTouch(calData);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    TouchPinInit();

#if 1
    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#else
    /* Try an example from the lv_examples Arduino library
       make sure to include it as written above.
    lv_example_btn_1();
   */

    // uncomment one of these demos
    lv_demo_widgets(); // OK
                       // lv_demo_benchmark();          // OK
                       // lv_demo_keypad_encoder();     // works, but I haven't an encoder
                       // lv_demo_music();              // NOK
                       // lv_demo_printer();
                       // lv_demo_stress();             // seems to be OK
#endif

    Serial.println("Setup done");
    //关闭看门狗
    disableCore0WDT();
    //创建定时器
    BSP_TimerInit();
    // BSP_Timer_SetTimeMs(500U);
    BSP_Timer_StartOneShot(5000U);
    BSP_Timer_RegisterCallBack(timer_test_log);
    BSP_Timer_Start();

    //创建LVGL任务
    xTaskCreate(lv_timer_handler_task, "lv_timer_handler_task", 4096, NULL, 1, NULL);

    
}


void loop()
{
    // lv_timer_handler(); /* let the GUI do its work */
    // lv_tick_inc(1);
    // touch_value = touchRead(TOUTCH_PIN);

    // delay(1);
}
