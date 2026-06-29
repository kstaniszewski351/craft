#include "physics.h"
#include "axis.h"
#include "components/transform.h"
#include "components/box_collider.h"
#include "components/physics_object.h"
#include <glm/common.hpp>

bool Collides(const Transform& transform, const BoxCollider& aabb, glm::ivec3 block_pos) {
  glm::vec3 p1 = transform.pos - glm::vec3(aabb.size.x / 2.0f, 0.0f, aabb.size.z / 2.0f);
  glm::vec3 p2 = transform.pos + glm::vec3(aabb.size.x / 2.0f, aabb.size.y, aabb.size.z / 2.0f);
  glm::ivec3 ip1 = glm::floor(p1);
  glm::ivec3 ip2 = glm::floor(p2);


  for(int x = p1.x; x <= p2.x; x++) {
    for(int y = p1.y; y <= p2.y; y++) {
      for(int z = p1.z; z <= p2.z; z++) {
        if(glm::ivec3(x, y, z) == block_pos) {
          return true;
        }
      }
    }
  }
  return false;
}

struct AAFace {
  glm::vec2 p1;
  glm::vec2 p2;
  float d;
  Axis axis;
};


void Collide(const BoxCollider& aabb, Transform& transform, PhysicsObject& physics, World& world, float delta_time) {
  glm::vec3 velocity = physics.velocity * delta_time;
  // glm::vec3 p1 = transform.pos;
  // glm::vec3 p2 = transform.pos + aabb.size;
  glm::vec3 p1 = transform.pos - glm::vec3(aabb.size.x / 2.0f, 0.0f, aabb.size.z / 2.0f);
  glm::vec3 p2 = transform.pos + glm::vec3(aabb.size.x / 2.0f, aabb.size.y, aabb.size.z / 2.0f);
  float eps = 1e-5f;
  //float eps = 0.0f;
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
          physics.velocity.y = 0.0f;
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
            physics.velocity.y = 0.0f;

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
            delta.x = ip1.x + 1 - transform.pos.x + aabb.size.x / 2.0f + eps;
            physics.velocity.x = 0.0f;

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
            delta.x = ip2.x - transform.pos.x - aabb.size.x / 2.0f - eps;
            physics.velocity.x = 0.0f;

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
            delta.z = ip1.z + 1 - transform.pos.z + aabb.size.z / 2.0f + eps;
            physics.velocity.z = 0.0f;

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
            delta.z = ip2.z - transform.pos.z - aabb.size.z / 2.0f - eps;
            physics.velocity.z = 0.0f;

            return;
          }
        }
      }
    }();

  }

  transform.pos += delta;
}

void Physics(entt::registry& reg, World& world, float delta_time) {
  auto view = reg.view<Transform, BoxCollider, PhysicsObject>();

  for(auto [entity, transform, aabb, physics] : view.each()) {
    //constexpr float dt = 1.0 / 60;
    float h_drag = std::exp(-5 * delta_time) ;
    float v_drag = std::exp(-0.1 * delta_time);

    physics.velocity.y += -10 * delta_time;
    physics.velocity.y *= v_drag;
    physics.velocity.x *= h_drag;
    physics.velocity.z *= h_drag;
    Collide(aabb, transform, physics, world, delta_time);
  }
};