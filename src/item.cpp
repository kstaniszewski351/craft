#include "item.h"
#include "components/box_collider.h"
#include "components/transform.h"
#include "components/view.h"
#include "entity.h"
#include "systems/physics.h"

Item::Item(std::string name) :
  name_(name) {

}

BlockItem::BlockItem(std::string name, const Block& block) : Item(name), block_(block) {

}

void BlockItem::OnUse(World& world, Entity player) const {
  auto view = player.GetComponent<View>();
  auto transform = player.GetComponent<Transform>();
  auto aabb = player.GetComponent<BoxCollider>();

  Ray ray = view.GetRay(transform.pos);
  auto pos = world.Raycast(ray);
  if(!pos) {
    return;
  }
  glm::ivec3 place_pos = pos->pos + pos->face;
  if(Collides(transform, aabb, place_pos)) {
    return;
  }

  world.SetBlock(place_pos, block_.GetRegistryID());
}