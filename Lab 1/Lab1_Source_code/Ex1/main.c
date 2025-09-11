#include "main.h"

/* === Active-low helpers: RESET = ON, SET = OFF === */
#define LED_ON(port, pin)   HAL_GPIO_WritePin((port), (pin), GPIO_PIN_RESET)
#define LED_OFF(port, pin)  HAL_GPIO_WritePin((port), (pin), GPIO_PIN_SET)

/* === Pins (GPIOA) === */
#define NS_RED_PORT     GPIOA
#define NS_RED_PIN      GPIO_PIN_5
#define NS_YEL_PORT     GPIOA
#define NS_YEL_PIN      GPIO_PIN_6
#define NS_GRN_PORT     GPIOA
#define NS_GRN_PIN      GPIO_PIN_7

#define EW_RED_PORT     GPIOA
#define EW_RED_PIN      GPIO_PIN_10
#define EW_YEL_PORT     GPIOA
#define EW_YEL_PIN      GPIO_PIN_9
#define EW_GRN_PORT     GPIOA
#define EW_GRN_PIN      GPIO_PIN_8

/* === Timing (ms) === */
#define T_GREEN_NS      3000   // NS green time
#define T_YELLOW_NS     2000   // NS yellow time
#define T_GREEN_EW      3000   // EW green time
#define T_YELLOW_EW     2000   // EW yellow time
#define ALL_RED_MS         0   // clearance (0 to disable)

/* Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Error_Handler(void);

/* === Write all six LEDs deterministically ===
   Use 1 to turn that LED ON, 0 to turn it OFF (active-low handled here) */
static void set_all(uint8_t ns_r, uint8_t ns_y, uint8_t ns_g,
                    uint8_t ew_r, uint8_t ew_y, uint8_t ew_g)
{
    (ns_r ? LED_ON(NS_RED_PORT, NS_RED_PIN) : LED_OFF(NS_RED_PORT, NS_RED_PIN));
    (ns_y ? LED_ON(NS_YEL_PORT, NS_YEL_PIN) : LED_OFF(NS_YEL_PORT, NS_YEL_PIN));
    (ns_g ? LED_ON(NS_GRN_PORT, NS_GRN_PIN) : LED_OFF(NS_GRN_PORT, NS_GRN_PIN));

    (ew_r ? LED_ON(EW_RED_PORT, EW_RED_PIN) : LED_OFF(EW_RED_PORT, EW_RED_PIN));
    (ew_y ? LED_ON(EW_YEL_PORT, EW_YEL_PIN) : LED_OFF(EW_YEL_PORT, EW_YEL_PIN));
    (ew_g ? LED_ON(EW_GRN_PORT, EW_GRN_PIN) : LED_OFF(EW_GRN_PORT, EW_GRN_PIN));
}

static void all_off(void)
{
    set_all(0,0,0, 0,0,0);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    all_off();

    while (1)
    {
        /* ===== PHASE A: NS GREEN + EW RED (simultaneous) ===== */
        set_all(/*NS*/1,0,0, /*EW*/0,0,1);
        HAL_Delay(T_GREEN_NS);

        /* NS YELLOW + EW RED (simultaneous)  <<< FIXED */
        set_all(/*NS*/0,1,0, /*EW*/0,0,1);
        HAL_Delay(T_YELLOW_NS);

        /* Optional all-red clearance */
        if (ALL_RED_MS) { set_all(1,0,0, 1,0,0); HAL_Delay(ALL_RED_MS); }

        /* ===== PHASE B: EW GREEN + NS RED (simultaneous) ===== */
        set_all(/*NS*/0,0,1, /*EW*/1,0,0);
        HAL_Delay(T_GREEN_EW);

        /* EW YELLOW + NS RED (simultaneous)  <<< FIXED */
        set_all(/*NS*/0,0,1, /*EW*/0,1,0);
        HAL_Delay(T_YELLOW_EW);

        /* Optional all-red clearance */
        if (ALL_RED_MS) { set_all(1,0,0, 1,0,0); HAL_Delay(ALL_RED_MS); }
    }
}

/* ===== System Clock (simple HSI) ===== */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) { Error_Handler(); }
}

/* ===== GPIO Init ===== */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;     // ensure no internal pulls fight your LED wiring
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = NS_RED_PIN | NS_YEL_PIN | NS_GRN_PIN |
                          EW_RED_PIN | EW_YEL_PIN | EW_GRN_PIN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    all_off(); // start with all LEDs OFF (inactive, since active-low)
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) { (void)file; (void)line; }
#endif
