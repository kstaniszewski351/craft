#include "physics.h"
#include "../components/transform.h"
#include "../components/aabb.h"
#include "../components/physics_object.h"
#include "../axis.h"
#include <glm/common.hpp>
//#include <entt/entity/fwd.hpp>


void Collide(const AABB& aabb, Transform& transform, PhysicsObject& physics, World& world, float delta_time) {
  glm::vec3 velocity = physics.velocity * delta_time;
  glm::vec3 p1 = transform.pos;
  glm::vec3 p2 = transform.pos + aabb.size;
  float eps = 1e-3f;
  glm::vec3 delta = velocity;



  glm::ivec3 ip1 = glm::floor(p1 + glm::vec3(0, delta.y, 0) + glm::vec3(eps, 0, eps));
  glm::ivec3 ip2 = glm::floor(p2 + glm::vec3(0, delta.y, 0) - glm::vec3(eps, 0, eps));
  // glm::ivec3 ip1 = glm::floor(p1 + velocity);
  // glm::ivec3 ip2 = glm::floor(p2 + velocity);

  physics.onGround = false;

  if(physics.velocity.y < 0) {
    [&] {for(int x = ip1.x; x <= ip2.x; x++) {
      for(int z = ip1.z; z <= ip2.z; z++) {
        char block = world.GetBlock(glm::ivec3(x, ip1.y, z));

        if(block != 0) {
          delta.y = ip1.y + 1 - transform.pos.y;
          physics.velocity.y = 0.0;
          physics.onGround = true;

          return;
        }
      }
    }}();
  }
  if(physics.velocity.y > 0) {
    [&] {
      for(int x = ip1.x; x <= ip2.x; x++) {
        for(int z = ip1.z; z <= ip2.z; z++) {

          char block = world.GetBlock(glm::ivec3(x, ip2.y, z));
          if(block != 0) {
            delta.y = ip2.y - transform.pos.y - aabb.size.y;
            physics.velocity.y = 0.0;

            return;
          }
        }
      }
    }();
  }
  
  ip1 = glm::floor(p1 + glm::vec3(delta.x, 0, 0) + glm::vec3(0, eps, eps));
  ip2 = glm::floor(p2 + glm::vec3(delta.x, 0, 0) - glm::vec3(0, eps, eps));
  // ip1 = glm::floor(p1 + velocity);
  // ip2 = glm::floor(p2 + velocity);

  if(physics.velocity.x < 0) {
    [&] {
      for(int y = ip1.y; y <= ip2.y; y++) {
        for(int z = ip1.z; z <= ip2.z; z++) {

          char block = world.GetBlock(glm::ivec3(ip1.x, y, z));

          if(block != 0) {
            delta.x = ip1.x + 1 - transform.pos.x;
            physics.velocity.x = 0.0;

            return;
          }
        }
      }
    }();
  }
  if(physics.velocity.x > 0) {
    [&] {
      for(int y = ip1.y; y <= ip2.y; y++) {
        for(int z = ip1.z; z <= ip2.z; z++) {

          char block = world.GetBlock(glm::ivec3(ip2.x, y, z));

          if(block != 0) {
            delta.x = ip2.x - transform.pos.x - aabb.size.x;
            physics.velocity.x = 0.0;

            return;
          }
        }
      }
    }();

  }


  ip1 = glm::floor(p1 + glm::vec3(0, 0, delta.z) + glm::vec3(eps, eps, 0));
  ip2 = glm::floor(p2 + glm::vec3(0, 0, delta.z) - glm::vec3(eps, eps, 0));
  // ip1 = glm::floor(p1 + velocity);
  // ip2 = glm::floor(p2 + velocity);

  if(physics.velocity.z < 0) {
    [&] {
      for(int y = ip1.y; y <= ip2.y; y++) {
        for(int x = ip1.x; x <= ip2.x; x++) {

          char block = world.GetBlock(glm::ivec3(x, y, ip1.z));

          if(block != 0) {
            delta.z = ip1.z + 1 - transform.pos.z;
            physics.velocity.z = 0.0;

            return;
          }
        }
      }
    }();
  }
  if(physics.velocity.z > 0) {
    [&] {
      for(int y = ip1.y; y <= ip2.y; y++) {
        for(int x = ip1.x; x <= ip2.x; x++) {

          char block = world.GetBlock(glm::ivec3(x, y, ip2.z));

          if(block != 0) {
            delta.z = ip2.z - transform.pos.z - aabb.size.z;
            physics.velocity.z = 0.0;

            return;
          }
        }
      }
    }();

  }
  
  // glm::vec3 delta_abs = glm::abs(delta);

  // float max = std::max(std::max(delta_abs.x, delta_abs.y), delta_abs.z);

  // if(max == delta_abs.x) {
  //   transform.pos.x += delta.x;
  // }
  // else if(max == delta_abs.y) {
  //   transform.pos.y += delta.y;
  // }
  // else if(max == delta_abs.z) {
  //   transform.pos.z += delta.z;
  // }

  transform.pos += delta;
}

void Physics(entt::registry& reg, World& world, float delta_time) {
  auto view = reg.view<Transform, AABB, PhysicsObject>();

  for(auto [entity, transform, aabb, physics] : view.each()) {
    constexpr float dt = 1.0 / 60;
    constexpr float h_drag = 1.0 - 0.088;
    constexpr float v_drag = 0.993;

    physics.velocity.y += -10 * dt;
    physics.velocity.y *= v_drag;
    physics.velocity.x *= h_drag;
    physics.velocity.z *= h_drag;
    Collide(aabb, transform, physics, world, dt);
  }
};