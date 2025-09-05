#include "main.h"

int main(void)
{
    HAL_Init();
    SystemClockConfig();
    GPIOInit();
    TIMInit();

    I2cHandle.Instance = I2C;                                 /* I2C */
    I2cHandle.Init.ClockSpeed = I2C_SPEEDCLOCK;               /* I2C通讯速度 */
    I2cHandle.Init.DutyCycle = I2C_DUTYCYCLE;                 /* I2C占空比 */
    I2cHandle.Init.OwnAddress1 = I2C_ADDRESS;                 /* I2C地址 */
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; /* 禁止广播呼叫 */
    I2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     /* 允许时钟延长 */
    if (HAL_I2C_Init(&I2cHandle) != HAL_OK)
        Error_Handler();

    ssd1315_iic_init(SSD1315_INTERFACE_IIC, SSD1315_ADDR_SA0_1);
    ssd1315_set_display(&OledHandle, SSD1315_DISPLAY_ON);
    ssd1315_set_contrast(&OledHandle, 0x1F);

    SW6208_Init();

    CppMain();

    while (1)
    {
    }
}

void TIMInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM14_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    // INIT SPEED MEASURE GPIO
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // INIT PWM GPIO
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM14;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // INIT SPEED MEASURE TIM
    SpdMesTimHandle.Instance = TIM1;
    SpdMesTimHandle.Init.Period = 65535;                         /* 自动重装载值 */
    SpdMesTimHandle.Init.Prescaler = 31;                         /* 预分频为15 */
    SpdMesTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* 向上计数 */
    SpdMesTimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; /* 时钟不分频 */
    if (HAL_TIM_Base_Init(&SpdMesTimHandle) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_Base_Start_IT(&SpdMesTimHandle) != HAL_OK)
        Error_Handler();

    HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

    // CONF SPEED MEASURE TIM
    SpdMesTimConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
    SpdMesTimConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    SpdMesTimConfig.ICPrescaler = TIM_ICPSC_DIV1;
    SpdMesTimConfig.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&SpdMesTimHandle, &SpdMesTimConfig, TIM_CHANNEL_3) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_IC_Start_IT(&SpdMesTimHandle, TIM_CHANNEL_3) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_IC_ConfigChannel(&SpdMesTimHandle, &SpdMesTimConfig, TIM_CHANNEL_4) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_IC_Start_IT(&SpdMesTimHandle, TIM_CHANNEL_4) != HAL_OK)
        Error_Handler();

    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

    if (HAL_TIM_Base_Start(&SpdMesTimHandle) != HAL_OK)
        Error_Handler();

    // INIT PWM TIM
    PwmTimHandle.Instance = TIM14;                                       /* 选择TIM14 */
    PwmTimHandle.Init.Period = 1800;                                     /* 自动重装载值 */
    PwmTimHandle.Init.Prescaler = 0;                                     /* 预分频为0 */
    PwmTimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            /* 时钟不分频 */
    PwmTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;                  /* 向上计数 */
    PwmTimHandle.Init.RepetitionCounter = 0;                             /* 不重复计数 */
    PwmTimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* 自动重装载寄存器没有缓冲 */
    /* 基础时钟初始化 */
    if (HAL_TIM_PWM_Init(&PwmTimHandle) != HAL_OK)
        Error_Handler();

    // CONF PWM TIM
    PwmTimConfig.OCMode = TIM_OCMODE_PWM1;         /* 输出配置为PWM1 */
    PwmTimConfig.OCPolarity = TIM_OCPOLARITY_HIGH; /* OC通道输出高电平有效 */
    // PwmTimConfig.OCFastMode = TIM_OCFAST_DISABLE;     /* 输出快速使能关闭 */
    PwmTimConfig.OCIdleState = TIM_OCIDLESTATE_RESET; /* 空闲状态OC1输出低电平 */

    PwmTimConfig.Pulse = 0;
    /* 配置通道1 */
    if (HAL_TIM_PWM_ConfigChannel(&PwmTimHandle, &PwmTimConfig, TIM_CHANNEL_1) != HAL_OK)
        Error_Handler();

    if (HAL_TIM_PWM_Start(&PwmTimHandle, TIM_CHANNEL_1) != HAL_OK)
        Error_Handler();
}

uint32_t SpdMesCntA, SpdMesCntB = 0;
uint32_t SpdMesLastA, SpdMesLastB = 0;

void TIM1_BRK_UP_TRG_COM_IRQHandler()
{
    if (__HAL_TIM_GET_FLAG(&SpdMesTimHandle, TIM_FLAG_UPDATE))
    {
        __HAL_TIM_CLEAR_FLAG(&SpdMesTimHandle, TIM_FLAG_UPDATE);
        SpdMesCntA += 65536U;
        SpdMesCntB += 65536U;
    }
}

void TIM1_CC_IRQHandler()
{
    if (__HAL_TIM_GET_FLAG(&SpdMesTimHandle, TIM_FLAG_CC3))
    {
        __HAL_TIM_CLEAR_FLAG(&SpdMesTimHandle, TIM_FLAG_CC3);
        SpdA = 45000000 / (SpdMesCntA + HAL_TIM_ReadCapturedValue(&SpdMesTimHandle, TIM_CHANNEL_3) - SpdMesLastA);
        SpdMesLastA = HAL_TIM_ReadCapturedValue(&SpdMesTimHandle, TIM_CHANNEL_3);
        SpdMesCntA = 0;
    }
    if (__HAL_TIM_GET_FLAG(&SpdMesTimHandle, TIM_FLAG_CC4))
    {
        __HAL_TIM_CLEAR_FLAG(&SpdMesTimHandle, TIM_FLAG_CC4);
        SpdB = 45000000 / (SpdMesCntB + HAL_TIM_ReadCapturedValue(&SpdMesTimHandle, TIM_CHANNEL_4) - SpdMesLastB);
        SpdMesLastB = HAL_TIM_ReadCapturedValue(&SpdMesTimHandle, TIM_CHANNEL_4);
        SpdMesCntB = 0;
    }
}

void GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能GPIO时钟 - HAL库写法
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    // PF4配置为模拟输入
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    // PF2配置为模拟输入
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    // 用户配置口线
    // 输入区
    GPIO_InitStruct.Pin = GPIO_PIN_5; // INT
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7; // KEY UP
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2; // KEY DOWN
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0; // KEY LEFT
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3; // KEY RIGHT
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6; // KEY CLICK
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 输出区
    //  CP_EN引脚 - PB1，初始状态为高电平
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // QC_EN引脚 - PA6，初始状态为低电平
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void SystemClockConfig(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* 配置振荡器 */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* 启用HSI */
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI不分频 */
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; /* HSI校准频率24MHz */
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* 关闭HSE */
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* 关闭LSI */
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* 关闭LSE */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                      /* 启用PLL */
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;              /* 使用HSI作为PLL输入源 */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    /* 配置系统时钟 */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* 配置HSI作为系统时钟源 */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        /* AHB时钟不分频 */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         /* APB1时钟不分频 */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
        Error_Handler();

    /* 配置SysTick为1ms中断 */
    if (HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000) != HAL_OK)
        Error_Handler();

    /* 配置SysTick中断优先级 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void Error_Handler(void)
{
    while (1)
        __asm("nop");
}