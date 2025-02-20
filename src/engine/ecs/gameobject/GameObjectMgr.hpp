#pragma once
#include <engine/ecs/EcsDefine.hpp>


namespace engine::ecs
{

class GameObjectMgr final:
    public bbt::templateutil::ManagerBase<GameObjectId, GameObject>
{
    typedef std::tuple<std::string, GameObjectTemplateId> GameObjectInfo; 
    typedef std::pair<MemberPtr, bool> Result; 
public:
    GameObjectMgr(SceneSPtr scene);
    ~GameObjectMgr();
    virtual Result      Search(KeyType key);
    virtual bool        IsExist(KeyType key);
    size_t              ObjCount() const;
    std::string         GetName(GameObjectTemplateId tid) const;        
    int                 GetGameobjectByFilter(std::vector<GameObjectWKPtr>& gameobjects, std::shared_ptr<EntityFilter> filter);
    SceneSPtr           GetScene() const;
    void                Update() {}

    bool InitTemplateInfo(std::initializer_list<GameObjectInfo> list);
protected:

    virtual bool OnMemberCreate(MemberPtr member_base) override;

    virtual bool OnMemberDestory(KeyType key) override;

    virtual KeyType GenerateKey(MemberPtr member_base) override;

private:
    std::map<GameObjectId, GameObjectWKPtr>  m_gameobject_map;
    util::hashmap::Hashmap<ComponentTemplateId, GameObjectInfo, 8> m_gameobject_info_map;
    SceneWKPtr m_scene;
};

}