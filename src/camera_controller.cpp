#include "camera_controller.h"
#include "fpv_camera.h"
#include <SDL3/SDL_keyboard.h>
#include <glm/vec2.hpp>
#include "direction.h"

CameraController::CameraController(FPVCamera& camera) : 
  camera_(camera),
  move_speed_(10.0f),
  rotation_speed_(3.0f),
  pitch_(0.0f),
  yaw_(0.0f) {

}

void CameraController::Update(float delta_time, glm::vec2 mouse_motion) {

  mouse_motion /= 0.5f;
  mouse_motion += 0.5f;
  mouse_motion = glm::mix(glm::vec2(-1.0f), glm::vec2(1.0f), mouse_motion);

  // update stored pitch/yaw (persist between frames)
  pitch_ += -mouse_motion.y * 36;
  pitch_ = glm::clamp(pitch_, -89.0f, 89.0f);
  yaw_ += -mouse_motion.x * 36;
  yaw_ = std::fmod(yaw_, 360.0f);

  camera_.rotation = glm::angleAxis(glm::radians(yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
  camera_.rotation = glm::rotate(camera_.rotation, glm::radians(pitch_), glm::vec3(1.0f, 0.0f, 0.0f));


  int num_keys;
  const bool* keyboard_state = SDL_GetKeyboardState(&num_keys);

  glm::vec3 xz_move_direction {};

  if(keyboard_state[SDL_SCANCODE_W]) {
      xz_move_direction += direction_vectors[Front];
  }
  if(keyboard_state[SDL_SCANCODE_S]) {
    xz_move_direction += direction_vectors[Back];
  }
  if(keyboard_state[SDL_SCANCODE_A]) {
    xz_move_direction += direction_vectors[Left];
  }
  if(keyboard_state[SDL_SCANCODE_D]) {
    xz_move_direction += direction_vectors[Right];
  }

  if(xz_move_direction != glm::vec3(0.0f)) {
    glm::quat yaw_rotation = glm::angleAxis(glm::radians(yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
    xz_move_direction = yaw_rotation * xz_move_direction;
    xz_move_direction = glm::normalize(xz_move_direction);
    camera_.position += xz_move_direction * move_speed_ * delta_time;
  }

  glm::vec3 y_move_direction {};
  if(keyboard_state[SDL_SCANCODE_SPACE]) {
    y_move_direction += direction_vectors[Up];
  }
  if(keyboard_state[SDL_SCANCODE_LSHIFT]) {
    y_move_direction += direction_vectors[Down];
  }
  if(y_move_direction != glm::vec3(0.0f)) {
    y_move_direction = glm::normalize(y_move_direction);
    camera_.position += y_move_direction * move_speed_ * delta_time;
  }
}