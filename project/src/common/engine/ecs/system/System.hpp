#pragma once
#include <engine/ecs/gameobject/GameObject.hpp>

namespace engine::ecs
{

template<class TSystem>
static inline std::unique_ptr<TSystem>& GetSystem()
{
    return TSystem::GetSysInst();
}

template<typename TChildClass>
class System
{
public:
    typedef engine::ecs::GameObject Gameobject;
    typedef engine::ecs::GameObjectSPtr GameObjectSPtr;
    System(){}
    virtual ~System() = 0;

    static std::unique_ptr<TChildClass>& GetSysInst()
    {
        static std::unique_ptr<TChildClass> _inst{nullptr};
        if (_inst == nullptr)
            _inst = std::unique_ptr<TChildClass>(new TChildClass());
        return _inst;
    }
};

template<typename TChildClass>
System<TChildClass>::~System()
{
}

}