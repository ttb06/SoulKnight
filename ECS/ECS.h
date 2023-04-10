/*This ECS implementation allows to easily add, remove and manage different components
for different entities, making it easy to add new functionality to the game without
having to change the existing code*/

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <array>
#include <bitset>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component *, maxComponents>;

/*This is the base class for all components,
it defines the basic interface of a component
and contains a pointer to the entity that it belongs to.*/
class Component
{
public:
    Entity *entity;

    virtual void init(){};
    virtual void update(){};
    virtual void draw(){};

    virtual ~Component(){};
};

/*This class represents an entity in the game, it contains
an array of unique pointers to the different components
that make up the entity, as well as a bitset
(std::bitset<maxComponents>) to keep track of which components an entity has.*/
class Entity
{
private:
    Manager &manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    // Store components
    ComponentArray componentArray;

    // If component is added, set value of the componentID inside bitset to true
    ComponentBitSet componentBitSet;

    GroupBitset groupBitset;

public:
    Entity(Manager &mManager) : manager(mManager) {}

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
    bool destroy() { active = false; }

    bool hasGroup(Group mGroup)
    {
        return groupBitset[mGroup];
    }

    void addGroup(Group mGroup);
    void delGroup(Group mGroup)
    {
        groupBitset[mGroup] = false;
    }

    // check if an entity has a specific component.
    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    /*used to add a new component of type T to an entity, it creates a new instance of
    T and passing in the arguments, then adds it to the entity's components list.*/
    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...mArgs)
    {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    /*retrieve a specific component from an entity.*/
    template <typename T>
    T &getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }
};

/*This class manages all entities in the game, it contains a vector of unique pointers to entities.
The getComponentTypeID() and getComponentTypeID<T>() are helper functions that generate unique IDs
for different component types, these are used to identify components in the bitset and array.*/
class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity *>, maxGroups> groupedEntities;

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

    /*remove entities that have been marked for destruction.*/
    void refresh()
    {
        for (auto i(0u); i < maxGroups; i++)
        {
            auto &v(groupedEntities[i]);
            v.erase(
                std::remove_if(std::begin(v), std::end(v),
                               [i](Entity *mEntity)
                               {
                                   return !mEntity->isActive() || !mEntity->hasGroup(i);
                               }),
                std::end(v));
        }

        entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity)
                                      { return !mEntity->isActive(); }),
                       std::end(entities));
    }

    void AddToGroup(Entity *mEntity, Group mGroup)
    {
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    std::vector<Entity *> &getGroup(Group mGroup)
    {
        return groupedEntities[mGroup];
    }
    // add a new entity to the game.
    Entity &addEntity()
    {
        Entity *e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};