#include "SpriteManager.hpp"
#include "InvalidSpriteIDException.hpp"
#include <iostream>
#include <stdexcept>

SpriteManager::SpriteManager(Graph* graph) 
    : graph_(graph), factory_(std::make_unique<SpriteFactory>()), attributeLock_(false) {
    lookForExistingSprites();
    graph_->addAttributeSink(this);
}

SpriteManager::~SpriteManager() {
    detach();
}

int SpriteManager::getSpriteCount() const {
    return sprites_.size();
}

bool SpriteManager::hasSprite(const std::string& identifier) const {
    return sprites_.find(identifier) != sprites_.end();
}

Sprite* SpriteManager::getSprite(const std::string& identifier) const {
    auto it = sprites_.find(identifier);
    return it != sprites_.end() ? it->second.get() : nullptr;
}

std::vector<Sprite*> SpriteManager::sprites() const {
    std::vector<Sprite*> spriteList;
    for (const auto& pair : sprites_) {
        spriteList.push_back(pair.second.get());
    }
    return spriteList;
}

void SpriteManager::detach() {
    if (graph_) {
        graph_->removeAttributeSink(this);
    }
    sprites_.clear();
    graph_ = nullptr;
}

void SpriteManager::setSpriteFactory(std::unique_ptr<SpriteFactory> factory) {
    factory_ = std::move(factory);
}

void SpriteManager::resetSpriteFactory() {
    factory_ = std::make_unique<SpriteFactory>();
}

Sprite* SpriteManager::addSprite(const std::string& identifier) {
    return addSprite(identifier, nullptr);
}

Sprite* SpriteManager::addSprite(const std::string& identifier, const Values* position) {
    if (identifier.find('.') != std::string::npos) {
        throw InvalidSpriteIDException("Sprite identifiers cannot contain dots.");
    }

    auto it = sprites_.find(identifier);
    if (it == sprites_.end()) {
        attributeLock_ = true;
        auto sprite = factory_->newSprite(identifier, this, position);
        sprites_.emplace(identifier, std::move(sprite));
        attributeLock_ = false;
    } else {
        if (position) {
            it->second->setPosition(*position);
        }
    }

    return sprites_[identifier].get();
}

template <typename T>
T* SpriteManager::addSprite(const std::string& identifier, const Values* position) {
    T* sprite = new T();
    sprite->init(identifier, this, position);
    sprites_.emplace(identifier, std::unique_ptr<Sprite>(sprite));
    return sprite;
}

void SpriteManager::removeSprite(const std::string& identifier) {
    auto it = sprites_.find(identifier);
    if (it != sprites_.end()) {
        attributeLock_ = true;
        it->second->removed();
        sprites_.erase(it);
        attributeLock_ = false;
    }
}

void SpriteManager::lookForExistingSprites() {
    // Implementation for looking up existing sprites from the graph
}

Values* SpriteManager::getPositionValue(void* value) {
    // Implementation for getting position values based on different types
    return nullptr;
}

