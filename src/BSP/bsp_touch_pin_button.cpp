#include "bsp_touch_pin_button.h"

static uint8_t touch_pin_state = RELEASED;
static lv_indev_drv_t indev_drv;

static int8_t my_btn_read_id(void)
{
    int8_t ret = -1;
    if (PRESSED == touch_pin_state)
    {
        ret = TOUCHU_PIN_BTN_0;
    }

    return ret;
}

void TouchPinInit(void)
{
    touch_value_t touch_value = 29000U;
    touchAttachInterrupt(TOUTCH_PIN, TouchCallBack, touch_value);

    lv_indev_drv_init(&indev_drv); /*Basic initialization*/

    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = button_read;

    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv);
}

void TouchCallBack(void)
{
    if (touchInterruptGetLastStatus(TOUTCH_PIN))
    {
        // Serial.println("touch");
        touch_pin_state = PRESSED;
    }
    else
    {
        touch_pin_state = RELEASED;
    }
}

void button_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    static uint32_t last_btn = 0;     /*Store the last pressed button*/
    static uint32_t last_key = 0;     /*Store the last pressed button*/
    static uint32_t last_x = 60;      /*Store the last pressed button*/
    static uint32_t last_y = 60;      /*Store the last pressed button*/
    int8_t btn_pr = my_btn_read_id(); /*Get the ID (0,1,2...) of the pressed button*/
    if (btn_pr >= 0)
    {                                         /*Is there a button press? (E.g. -1 indicated no button was pressed)*/
        last_btn = btn_pr;                    /*Save the ID of the pressed button*/
        data->state = LV_INDEV_STATE_PRESSED; /*Set the pressed state*/
        Serial.println("touch");
        last_key = LV_KEY_DOWN;
        last_y -= 10;
        if (0 == last_y)
        {
            last_y =0;
        }
    }
    else
    {

        data->state = LV_INDEV_STATE_RELEASED; /*Set the released state*/
        last_y =60;
    }
    data->point.x = last_x;
    data->point.y = last_y;
    data->key = last_key;
    data->btn_id = last_btn; /*Save the last button*/
}
