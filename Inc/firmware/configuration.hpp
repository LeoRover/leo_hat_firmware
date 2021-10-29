#include "main.h"
#include "usart.h"

#include "firmware/motor_controller.hpp"

// UART used for rosserial communication
static UART_HandleTypeDef& ROSSERIAL_UART = huart1;

// The timer CCR value corresponding to 100% PWM duty cycle
const uint16_t PWM_RANGE = 1000;

// Number of encoder readings to remember when estimating the wheel velocity
const uint16_t ENCODER_BUFFER_SIZE = 10;

// Motor driver configurations
const MotorConfiguration MOT_A_CONFIG = {
    .nsleep = {H4_NSLEEP_GPIO_Port, H4_NSLEEP_Pin},
    .phase = {H4_PHASE_GPIO_Port, H4_PHASE_Pin},
    .mode = {H4_MODE_GPIO_Port, H4_MODE_Pin},
    .fault = {H4_FAULT_GPIO_Port, H4_FAULT_Pin},
    .enc_cnt = &TIM5->CNT,
    .pwm_ccr = &TIM1->CCR4,
};

const MotorConfiguration MOT_B_CONFIG = {
    .nsleep = {H3_NSLEEP_GPIO_Port, H3_NSLEEP_Pin},
    .phase = {H3_PHASE_GPIO_Port, H3_PHASE_Pin},
    .mode = {H3_MODE_GPIO_Port, H3_MODE_Pin},
    .fault = {H3_FAULT_GPIO_Port, H3_FAULT_Pin},
    .enc_cnt = &TIM4->CNT,
    .pwm_ccr = &TIM9->CCR2,
};

const MotorConfiguration MOT_C_CONFIG = {
    .nsleep = {H1_NSLEEP_GPIO_Port, H1_NSLEEP_Pin},
    .phase = {H1_PHASE_GPIO_Port, H1_PHASE_Pin},
    .mode = {H1_MODE_GPIO_Port, H1_MODE_Pin},
    .fault = {H1_FAULT_GPIO_Port, H1_FAULT_Pin},
    .enc_cnt = &TIM3->CNT,
    .pwm_ccr = &TIM1->CCR1,
};

const MotorConfiguration MOT_D_CONFIG = {
    .nsleep = {H2_NSLEEP_GPIO_Port, H2_NSLEEP_Pin},
    .phase = {H2_PHASE_GPIO_Port, H2_PHASE_Pin},
    .mode = {H2_MODE_GPIO_Port, H2_MODE_Pin},
    .fault = {H2_FAULT_GPIO_Port, H2_FAULT_Pin},
    .enc_cnt = &TIM2->CNT,
    .pwm_ccr = &TIM9->CCR1,
};