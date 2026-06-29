#include "player_movement.h"
#include "components/physics_object.h"
#include "components/box_collider.h"
#include "components/transform.h"
#include "components/view.h"
#include "components/player_controller.h"
#include "components/inventory.h"
#include <SDL3/SDL_scancode.h>
#include <glm/ext/scalar_constants.hpp>
#include "input_manager.h"
#include "direction.h"
#include "world.h"
#include <SDL3/SDL_mouse.h>



// bool DoesCollide(glm::vec3 p1, glm::vec3 p2, glm::ivec3 pos) {
//   glm::ivec3 ip1 = glm::floor(p1);
//   glm::ivec3 ip2 = glm::floor(p2);

//   if(pos.x >= ip1.x && pos.x <= ip2.x 
//     && pos.y >= ip1.y && pos.y <= ip2.y
//     && pos.z >= ip1.z && pos.z <= ip2.z) {
//     return true;
//   }
//   return false;
// }

void MovePlayer(ECS& ecs, Window& window, Scene& scene, float delta_time) {
  auto& reg = ecs.GetReg();
  auto entt_view = reg.view<Transform, BoxCollider, PhysicsObject, View, PlayerController, Inventory>();
  auto& input_manager = Game::Get().GetInputManager();

  for(auto [entity, transform, box, physics, view, player_controller, inventory] : entt_view.each()) {
    
    //camera rotation
    glm::vec2 mouse_motion = window.GetRelativeMousePos(input_manager.GetMouseDelta());
    mouse_motion /= 0.5f;
    mouse_motion += 0.5f;
    mouse_motion = glm::mix(glm::vec2(-1.0f), glm::vec2(1.0f), mouse_motion);

    float& pitch = player_controller.pitch;
    float& yaw = player_controller.yaw;

    yaw -= mouse_motion.x * 36;
    yaw = std::fmod(yaw, 360);

    pitch -= mouse_motion.y * 36;
    pitch = glm::clamp(pitch, -89.9f, 89.9f);

    view.rotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    view.rotation = glm::rotate(view.rotation, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

    //movement
    int num_keys;
    const bool* keyboard_state = SDL_GetKeyboardState(&num_keys);

    if(keyboard_state[SDL_SCANCODE_LSHIFT]) {
      box.size = glm::vec3(0.8, 1.4, 0.8);
      view.yOffset = 1.2;
    }
    else {
      box.size = glm::vec3(0.8, 1.8, 0.8);
      view.yOffset = 1.6;
    }

    if(keyboard_state[SDL_SCANCODE_SPACE] && physics.onGround) {
      physics.velocity.y += 5;
    }

    glm::vec3 xz_vector {};

    if(keyboard_state[SDL_SCANCODE_W]) {
      xz_vector += direction_vectors[Front];
    }
    if(keyboard_state[SDL_SCANCODE_S]) {
      xz_vector += direction_vectors[Back];
    }
    if(keyboard_state[SDL_SCANCODE_A]) {
      xz_vector += direction_vectors[Left];
    }
    if(keyboard_state[SDL_SCANCODE_D]) {
      xz_vector += direction_vectors[Right];
    }

    if(xz_vector != glm::vec3(0.0f)) {
      glm::quat yaw_rotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
      xz_vector = yaw_rotation * xz_vector;
      xz_vector = glm::normalize(xz_vector);
      physics.velocity += xz_vector * 30.0f * delta_time;
    }

    //update invetory

    for(int i = SDL_SCANCODE_1; i <= SDL_SCANCODE_9; i++) {
      if(keyboard_state[i]) {
        inventory.active_slot = i - SDL_SCANCODE_1;
      }
    }

    //place blocks
    if(input_manager.GetMouseDowns()[SDL_BUTTON_RIGHT]) {
      auto& active_item = inventory.slots[inventory.active_slot];
      if(active_item.has_value()) {
        active_item->item->OnUse(scene.GetWorld(), Entity(&ecs, entity));
      }
    }
    if(input_manager.GetMouseDowns()[SDL_BUTTON_LEFT]) {
      auto raycast = scene.GetWorld().Raycast(view.GetRay(transform.pos));
      if(raycast) {
        scene.GetWorld().SetBlock(raycast->pos, 0);
      }
    }
  }
}