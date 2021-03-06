// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/renderer_host/input/touchpad_tap_suppression_controller.h"

#include "content/browser/renderer_host/input/input_router.h"
#include "content/browser/renderer_host/input/tap_suppression_controller.h"
#include "content/browser/renderer_host/input/tap_suppression_controller_client.h"
#include "ui/base/gestures/gesture_configuration.h"

namespace content {

TouchpadTapSuppressionController::TouchpadTapSuppressionController(
    InputRouter* input_router)
    : input_router_(input_router),
      controller_(new TapSuppressionController(this)) {
}

TouchpadTapSuppressionController::~TouchpadTapSuppressionController() {}

void TouchpadTapSuppressionController::GestureFlingCancel() {
  controller_->GestureFlingCancel();
}

void TouchpadTapSuppressionController::GestureFlingCancelAck(bool processed) {
  controller_->GestureFlingCancelAck(processed);
}

bool TouchpadTapSuppressionController::ShouldDeferMouseDown(
    const MouseEventWithLatencyInfo& event) {
  bool should_defer = controller_->ShouldDeferTapDown();
  if (should_defer)
    stashed_mouse_down_ = event;
  return should_defer;
}

bool TouchpadTapSuppressionController::ShouldSuppressMouseUp() {
  return controller_->ShouldSuppressTapUp();
}

int TouchpadTapSuppressionController::MaxCancelToDownTimeInMs() {
  return ui::GestureConfiguration::fling_max_cancel_to_down_time_in_ms();
}

int TouchpadTapSuppressionController::MaxTapGapTimeInMs() {
  return ui::GestureConfiguration::fling_max_tap_gap_time_in_ms();
}

void TouchpadTapSuppressionController::DropStashedTapDown() {
}

void TouchpadTapSuppressionController::ForwardStashedTapDownForDeferral() {
  // Mouse downs are not handled by gesture event filter; so, they are
  // immediately forwarded to the renderer.
  input_router_->SendMouseEventImmediately(stashed_mouse_down_);
}

void TouchpadTapSuppressionController::ForwardStashedTapDownSkipDeferral() {
  // Mouse downs are not handled by gesture event filter; so, they are
  // immediately forwarded to the renderer.
  input_router_->SendMouseEventImmediately(stashed_mouse_down_);
}

}  // namespace content
