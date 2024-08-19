#include "Sprite.hpp"
#include "graph/Graph.hpp"

Sprite::Sprite() {}

Sprite::Sprite(const std::string& id, SpriteManager* manager)
    : Sprite(id, manager, Values(Style::Units::GU, 0.0f, 0.0f, 0.0f)) {}

Sprite::Sprite(const std::string& id, SpriteManager* manager, const Values& position) {
    init(id, manager, position);
}

void Sprite::init(const std::string& id, SpriteManager* manager, const Values& position) {
    this->id = id;
    this->completeId = "ui.sprite." + id;
    this->manager = manager;

    if (!manager->getGraph().hasAttribute(completeId)) {
        this->position = position;
        manager->getGraph().setAttribute(completeId, position);
    } else {
        this->position = SpriteManager::getPositionValue(manager->getGraph().getAttribute(completeId));
    }
}

void Sprite::removed() {
    manager->getGraph().removeAttribute(completeId);

    if (attached())
        detach();

    std::vector<std::string> keys;

    for (const auto& key : manager->getGraph().attributeKeys()) {
        if (key.find(completeId + ".") == 0)
            keys.push_back(key);
    }

    for (const auto& key : keys) {
        manager->getGraph().removeAttribute(key);
    }
}

Element* Sprite::getAttachment() const {
    return attachment;
}

bool Sprite::attached() const {
    return (attachment != nullptr);
}

double Sprite::getX() const {
    if (position.getValues().size() > 0)
        return position.getValues()[0];
    return 0;
}

double Sprite::getY() const {
    if (position.getValues().size() > 1)
        return position.getValues()[1];
    return 0;
}

double Sprite::getZ() const {
    if (position.getValues().size() > 2)
        return position.getValues()[2];
    return 0;
}

Style::Units Sprite::getUnits() const {
    return position.getUnits();
}

void Sprite::attachToNode(const std::string& id) {
    if (attachment != nullptr)
        detach();

    attachment = manager->getGraph().getNode(id);

    if (attachment != nullptr)
        attachment->setAttribute(completeId);
}

void Sprite::attachToEdge(const std::string& id) {
    if (attachment != nullptr)
        detach();

    attachment = manager->getGraph().getEdge(id);

    if (attachment != nullptr)
        attachment->setAttribute(completeId);
}

void Sprite::detach() {
    if (attachment != nullptr) {
        attachment->removeAttribute(completeId);
        attachment = nullptr;
    }
}

void Sprite::setPosition(double percent) {
    setPosition(position.getUnits(), percent, 0.0, 0.0);
}

void Sprite::setPosition(double x, double y, double z) {
    setPosition(position.getUnits(), x, y, z);
}

void Sprite::setPosition(Style::Units units, double x, double y, double z) {
    bool changed = false;

    if (position.get(0) != x) {
        changed = true;
        position.setValue(0, x);
    }
    if (position.get(1) != y) {
        changed = true;
        position.setValue(1, y);
    }
    if (position.get(2) != z) {
        changed = true;
        position.setValue(2, z);
    }
    if (position.getUnits() != units) {
        changed = true;
        position.setUnits(units);
    }

    if (changed)
        manager->getGraph().setAttribute(completeId, position);
}

void Sprite::setPosition(const Values& values) {
    if (!values.isEmpty()) {
        int n = values.getValues().size();

        if (n > 2) {
            setPosition(values.getUnits(), values.get(0), values.get(1), values.get(2));
        } else if (n > 0) {
            setPosition(values.get(0));
        }
    }
}

std::string Sprite::getId() const {
    return id;
}

std::string Sprite::getLabel(const std::string& key) const {
    return manager->getGraph().getLabel(completeId + "." + key);
}

std::any Sprite::getAttribute(const std::string& key) const {
    return manager->getGraph().getAttribute(completeId + "." + key);
}

void Sprite::setAttribute(const std::string& attribute, const std::vector<std::any>& values) {
    manager->getGraph().setAttribute(completeId + "." + attribute, values);
}

void Sprite::removeAttribute(const std::string& attribute) {
    manager->getGraph().removeAttribute(completeId + "." + attribute);
}
