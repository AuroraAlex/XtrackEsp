#ifndef __BSP_TOUCH_PIN_BUTTON_H__
#define __BSP_TOUCH_PIN_BUTTON_H__

#include "comm.h"


#define TOUTCH_PIN 8


typedef enum{
    RELEASED = 0x00,
    PRESSED = 0x01
}touch_pin_state_enum;

typedef enum{
    TOUCHU_PIN_BTN_0 = 0x00
}touch_pin_btn_enum;

#if __cplusplus
extern "C"{
#endif

void TouchPinInit(void);
void TouchCallBack(void);
void button_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

#if __cplusplus
}
#endif

#endif