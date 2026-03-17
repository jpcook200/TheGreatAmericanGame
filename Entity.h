#pragma once

#include "Components.h"
#include <tuple>
#include <string>

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifespan,
    CInput,
    CBoundingBox,
    CAnimation,
    CGravity,
    CState
> ComponentTuple;

class Entity
{
    friend class EntityManager;

    bool m_active = true;                               // is the entity alive
    size_t m_id = 0;                                    // id value for the entity
    std::string m_tag = "default";                      // tag for enitity type
    ComponentTuple m_components;                        // tuple of componenets instance

    Entity(const size_t id, const std::string& tag);    // private constructor

public:
    bool isActive() const;                              // get is active variable
    const std::string& getTag() const;                  // get tag variable
    const size_t getId() const;                         // get id variable
    void destroy();                                     // set active to false  

    template<typename T>
    bool hasComponent() const {
        return getComponent<T>().has;
    }

    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T& getComponent() {
        return std::get<T>(m_components);
    }

    template<typename T>
    const T& getComponent() const {
        return std::get<T>(m_components);
    }

    template<typename T>
    void removeComponent() {
        getComponent<T>() = T();
    }
};