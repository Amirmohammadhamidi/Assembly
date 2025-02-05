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
class Manager;

using ComponentID = std::size_t;
inline ComponentID getComponentTypeID()
{
    // each time we run this it will give ID for our component
    static ComponentID lastID = 0;
    return lastID++;
}

// each component type will only contians one typeID because typeID is static
template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
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
    Manager *manager;
    void update()
    {
        for (auto &c : components)
            c->update();
    }
    void draw()
    {
        for (auto &c : components)
            c->draw();
    }
    bool isActive() { return active; }
    void destroy() { active = false; }

    // func 1 : check component existense:
    template <typename T>
    bool hasComponent() const
    {
        // each component type in our game world has unique id which we can get it by getComponentID() function
        return componentBitSet[getComponentTypeID<T>()];
    }

    // func2 : add component to entity components

    //  T : component type which we add
    //  TArgs : variable number of additional argumants of any type
    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...mArgs)
    {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));
        // add component to array and bitset
        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c; // return refrence to the component
    }

    // func3 : getComponent
    template <typename T>
    T &getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }
};

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;

public:
    void update()
    {
        for (auto &e : entities)
            e->update();
    }
    void draw()
    {
        for (auto &e : entities)
            e->draw();
    }

    void refresh()
    {
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                                      [](const std::unique_ptr<Entity> &mEntity)
                                      {
                                          return !mEntity->isActive();
                                      }),
                       std::end(entities));
    }
    Entity &addEntity()
    {
        Entity *e = new Entity();
        e->manager = this;
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};
#endif