#include "key_handler.h"

int8_t key_status = -1; // 00n = press; 10n = long press; 20n = double click(* disabled)

uint32_t key_timer = 0;
int8_t key_info_arr[5] = {0, 0, 0, 0, 0}; // U D L R C

void key_handler_update(void)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (key_info_arr[i] <= -1 && key_info_arr[i] >= -126)
        {
            key_info_arr[i]--;
        }
    }
}

void button_it_handler(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t index)
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin))
    {
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
        {
            if (key_info_arr[index] >= 0)
            {
                key_info_arr[index] = -1;
                key_timer = HAL_GetTick() / 10;
            }
        }
        else
        {
            for (uint8_t i = 0; i < 5; i++)
            {
                if (key_info_arr[i] <= -1)
                {
                    key_info_arr[i] = min(127, (HAL_GetTick() / 10 - key_timer));

                    if (key_info_arr[i] > 32)
                    {
                        key_status = 10 + i;
                    }
                    else if (key_info_arr[i] >= 2)
                    {
                        key_status = i;
                    }
                }
            }
        }

        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
        HAL_GPIO_EXTI_Callback(GPIO_Pin);
    }
}

void EXTI0_1_IRQHandler(void)
{
    button_it_handler(GPIOB, GPIO_PIN_0, KEY_LEFT_INDEX);
}

void EXTI2_3_IRQHandler(void)
{
    button_it_handler(GPIOB, GPIO_PIN_2, KEY_DOWN_INDEX);
    button_it_handler(GPIOB, GPIO_PIN_3, KEY_RIGHT_INDEX);
}

void EXTI4_15_IRQHandler(void)
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5)) // INT
    {
        // TODO
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_5);
        HAL_GPIO_EXTI_Callback(GPIO_PIN_5);
    }
    button_it_handler(GPIOB, GPIO_PIN_6, KEY_CENTER_INDEX);
    button_it_handler(GPIOA, GPIO_PIN_7, KEY_UP_INDEX);
}