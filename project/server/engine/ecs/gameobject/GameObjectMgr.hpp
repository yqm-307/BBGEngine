// #include "engine/ecs/gameobject/GameObject.hpp"
#include "engine/ecs/EcsDefine.hpp"
#include "util/managerbase/ManagerBase.hpp"
#include "util/assert/Assert.hpp"

namespace game::share::ecs
{

class GameObjectMgr:
    public game::util::managerbase::ManagerBase<GameObjectId, GameObjectSPtr>
{
public:
    static const std::unique_ptr<GameObjectMgr>& GetInstance();

    virtual Result      Search(KeyType key) override;
    virtual bool        IsExist(KeyType key) override;
    bool                OnInitGameObject(KeyType key, ValueType value);
    bool                OnDestoryGameObject(KeyType key, ValueType value);

    template<typename GameObjectChildType, typename ...InitArgs>
    GameObjectSPtr Create(InitArgs ...args);
protected:
    GameObjectMgr();
    ~GameObjectMgr();

private:
    std::map<GameObjectId, GameObjectSPtr>  m_gameobject_map;
};


template<typename GameObjectChildType, typename ...InitArgs>
GameObjectSPtr GameObjectMgr::Create<GameObjectChildType, InitArgs...>(InitArgs ...args)
{
    static_assert( std::is_base_of_v<GameObject, GameObjectChildType> ); // 必须派生自GameObject
    auto sptr = std::make_shared<GameObjectChildType>(args...);
    bool isok = OnInitGameObject(sptr->GetId(), sptr);
    DebugAssert(isok);
    return sptr;
}

}