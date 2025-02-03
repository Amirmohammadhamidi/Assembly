#ifndef ECS_hpp
#define ECS_hpp
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

// header file for Entity Component System:
// entities are game's objects
// components are data containers which contains data about entities

class Component;
class Entity;

using ComponentID = std::size_t;
inline ComponentID getComponentID()
{
    // each time we run this it will give ID for our component
    static ComponentID lastID = 0;
    return lastID++;
}

// each component type will only contians one typeID because typeID is static
template <typename T>
inline ComponentID getComponentID() noexcept
{
    static ComponentID typeID = getComponentID();
    return typeID();
}

constexpr std::size_t maxComponents = 32;
// componentBitSet is bitset with fixed size 32 which will represents the presence or absence of components
// for entities
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

class Component
{
public:
    // each component has refrence to it's owner
    Entity *entity;
    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual ~Component() {}
};

class Entity
{
private:
    bool active = true;
    // each entity save list of components which it contains
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;

public:
    void update()
    {
        for (auto &c : components)
            c->update();
        for (auto &c : components)
            c->draw();
    }
    void draw()
    {
    }
    bool isActive() { return active; }
    void destroy() { active = false; }

    // we give this function a component type and check if this entity contains this component type or not
    template <typename T>
    bool hasComponent() const
    {
        // each component type in our game world has unique id which we can get it by getComponentID() function
        return componentBitSet[getComponentID]
    }
};

#endif