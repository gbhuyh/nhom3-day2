#include <stdint.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_rcc.h>

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef void void_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;

#define BTN_PRESS               0
#define GPIO_PIN_SET            1
#define GPIO_PIN_RESET          0

#define BUZZER_GPIO_PORT        GPIOC
#define BUZZER_GPIO_PIN         GPIO_Pin_9
#define BUZZERControl_SetClock  RCC_AHB1Periph_GPIOC

#define BUTTON_GPIO_PORT        GPIOB
#define BUTTON_GPIO_PIN         GPIO_Pin_3
#define BUTTONControl_SetClock  RCC_AHB1Periph_GPIOB

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void buzzer_Init(void);
static void button_Init(void);
static void buzzerControlSetStatus(GPIO_TypeDef *pGPIOx, u16_t wGPIO_Pin, u8_t byStatus);
static u8_t buttonReadStatus(GPIO_TypeDef *pGPIOx, u16_t wGPIO_Pin);
static void delay(void);

/******************************************************************************/
/*                            MAIN FUNCTION                                   */
/******************************************************************************/
int main(void) {
    u8_t byStatus = 1;
    buzzer_Init();
    button_Init();
    while (1) {
        if (buttonReadStatus(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN) == BTN_PRESS) {
            delay();
            byStatus++;
        }
        if (byStatus % 2 == 0) {
            delay();
            buzzerControlSetStatus(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET);
        } else {
            delay();
            buzzerControlSetStatus(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_RESET);
        }
    }
}

/**
 * @func   delay
 * @brief  Simple delay loop
 */
static void delay(void) {
    for (u32_t i = 0; i < 500000; i++);
}

/**
 * @func   buzzer_Init
 * @brief  Initialize GPIO for buzzer
 */
static void buzzer_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(BUZZERControl_SetClock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @func   button_Init
 * @brief  Initialize GPIO for button
 */
static void button_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(BUTTONControl_SetClock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @func   buzzerControlSetStatus
 * @brief  Control buzzer ON/OFF
 */
static void buzzerControlSetStatus(GPIO_TypeDef *pGPIOx, u16_t wGPIO_Pin, u8_t byStatus) {
    if (byStatus == GPIO_PIN_SET) {
        pGPIOx->BSRRL = wGPIO_Pin;
    } else {
        pGPIOx->BSRRH = wGPIO_Pin;
    }
}

/**
 * @func   buttonReadStatus
 * @brief  Read button press status
 */
static u8_t buttonReadStatus(GPIO_TypeDef *pGPIOx, u16_t wGPIO_Pin) {
    return (pGPIOx->IDR & wGPIO_Pin) ? (u8_t)1 : (u8_t)0;
}
