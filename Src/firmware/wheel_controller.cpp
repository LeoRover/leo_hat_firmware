#include <algorithm>

#include "firmware/configuration.hpp"
#include "firmware/motor_controller.hpp"
#include "firmware/parameters.hpp"
#include "firmware/utils.hpp"
#include "firmware/wheel_controller.hpp"

static constexpr float PI = 3.141592653F;

WheelController::WheelController(const WheelConfiguration& wheel_conf)
    : motor(wheel_conf.motor_conf), encoder_buffer_(ENCODER_BUFFER_SIZE) {
  if (wheel_conf.reverse_polarity) {
    motor.setMotorPolarity(Polarity::Reversed);
    motor.setEncoderPolarity(Polarity::Reversed);
  }
}

void WheelController::init() {
  v_reg_.setCoeffs(params.motor_pid_p, params.motor_pid_i, params.motor_pid_d);
  v_reg_.setRange(
      std::min(static_cast<float>(PWM_RANGE), params.motor_power_limit));
  motor.init();
  motor.resetEncoderCnt();
}

void WheelController::update(const uint32_t dt_ms) {
  int32_t ticks_prev = ticks_now_;
  ticks_now_ = motor.getEncoderCnt();

  int32_t new_ticks = ticks_now_ - ticks_prev;

  std::pair<int32_t, uint32_t> encoder_old =
      encoder_buffer_.push_back(std::pair<int32_t, uint32_t>(new_ticks, dt_ms));

  ticks_sum_ += new_ticks;
  dt_sum_ += dt_ms;

  ticks_sum_ -= encoder_old.first;
  dt_sum_ -= encoder_old.second;

  v_now_ = static_cast<float>(ticks_sum_) / (dt_sum_ * 0.001F);

  if (enabled_) {
    if (v_now_ == 0.0F && v_target_ == 0.0F) {
      v_reg_.reset();
      power_ = 0;
    } else {
      float v_err = v_now_ - v_target_;
      power_ = v_reg_.update(v_err, dt_ms);
    }
    motor.setPower(power_);
  }
}

void WheelController::setTargetVelocity(const float speed) {
  v_target_ = (speed / (2.0F * PI)) * params.motor_encoder_resolution;
}

float WheelController::getVelocity() {
  return (v_now_ / params.motor_encoder_resolution) * (2.0F * PI);
}

float WheelController::getPWMDutyCycle() {
  return (static_cast<float>(power_) / static_cast<float>(PWM_RANGE)) * 100.0F;
}

float WheelController::getTorque() {
  return motor.getWindingCurrent() * params.motor_torque_constant;
}

float WheelController::getDistance() {
  return (ticks_now_ / params.motor_encoder_resolution) * (2.0F * PI);
}

void WheelController::resetDistance() {
  motor.resetEncoderCnt();
  ticks_now_ = 0;
}

void WheelController::enable() {
  if (!enabled_) {
    v_reg_.reset();
    enabled_ = true;
  }
}

void WheelController::disable() {
  enabled_ = false;
  power_ = 0;
  motor.setPower(0);
}
