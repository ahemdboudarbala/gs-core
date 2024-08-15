#include "SpriteFactory.hpp"

std::unique_ptr<Sprite> SpriteFactory::newSprite(const std::string& identifier, SpriteManager* manager, const Values* position) {
    if (position) {
        return std::make_unique<Sprite>(identifier, manager, *position);
    } else {
        return std::make_unique<Sprite>(identifier, manager);
    }
}
