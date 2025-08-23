#include "key_handler.h"

key_info_t key_info_arr[5] = {
    {0, 0}, // UP
    {0, 0}, // DOWN
    {0, 0}, // LEFT
    {0, 0}, // RIGHT
    {0, 0}, // CLICK
};

int8_t key_status = -1; // 00n = press; 10n = long press; 20n = double click(* disabled)

void key_handler_update(void)
{
    for (int i = 0; i < 5; i++)
    {
        if (key_info_arr[i].key_pressing != 0)
        {
            key_info_arr[i].key_pressing += key_info_arr[i].key_pressing >= 127 ? 0 : 1;
            key_info_arr[i].key_last_press = -abs(key_info_arr[i].key_last_press);
        }
        else
        {
            if (key_info_arr[i].key_last_press < 0)
            {
                key_info_arr[i].key_last_press = 0;
            }
            key_info_arr[i].key_last_press += key_info_arr[i].key_last_press >= 127 ? 0 : 1;

            if (key_status != -1)
            {
                if (key_status % 10 == i && key_info_arr[i].key_last_press > 64)
                {
                    key_status = -1;
                }
            }
        }
    }
}

void button_it_handler(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t index)
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin))
    {
        if (abs(key_info_arr[index].key_last_press) > 1)
        {
            if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
            {
                if (key_info_arr[index].key_pressing == 0)
                {
                    key_info_arr[index].key_pressing = 1;
                }
            }
            else
            {
                if (key_info_arr[index].key_pressing != 0)
                {
                    if (key_info_arr[index].key_pressing > 10)
                    {
                        key_status = 10 + index; // Set long press status
                    }
                    else
                    {
                        key_status = index;
                    }
                    key_info_arr[index].key_pressing = 0;
                }
            }
        }

        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
        HAL_GPIO_EXTI_Callback(GPIO_Pin);
    }
}

void EXTI0_1_IRQHandler(void)
{
    button_it_handler(GPIOB, GPIO_PIN_0, 2);
}

void EXTI2_3_IRQHandler(void)
{
    button_it_handler(GPIOB, GPIO_PIN_2, 1);
    button_it_handler(GPIOB, GPIO_PIN_3, 3);
}

void EXTI4_15_IRQHandler(void)
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5)) // INT
    {
        // TODO
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_5);
        HAL_GPIO_EXTI_Callback(GPIO_PIN_5);
    }
    button_it_handler(GPIOB, GPIO_PIN_6, 4); // LEFT
    button_it_handler(GPIOA, GPIO_PIN_7, 0); // DOWN
}