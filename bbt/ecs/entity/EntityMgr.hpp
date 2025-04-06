#pragma once
#include <bbt/ecs/Define.hpp>


namespace bbt::ecs
{

class EntityMgr final:
    public bbt::core::util::ManagerBase<EntityId, Entity>
{
    typedef std::tuple<std::string, GameObjectTemplateId> GameObjectInfo; 
    typedef std::pair<MemberPtr, bool> Result; 
public:
    EntityMgr(SceneSPtr scene);
    ~EntityMgr();
    virtual Result      Search(KeyType key);
    virtual bool        IsExist(KeyType key);
    size_t              ObjCount() const;
    std::string         GetName(GameObjectTemplateId tid) const;        
    int                 GetGameobjectByFilter(std::vector<EntityWKPtr>& gameobjects, std::shared_ptr<EntityFilter> filter);
    SceneSPtr           GetScene() const;
    void                Update() {}

    bool InitTemplateInfo(std::initializer_list<GameObjectInfo> list);
protected:

    virtual bool OnMemberCreate(MemberPtr member_base) override;

    virtual bool OnMemberDestory(KeyType key) override;

    virtual KeyType GenerateKey(MemberPtr member_base) override;

private:
    std::map<EntityId, EntityWKPtr>  m_gameobject_map;
    util::Hashmap<ComponentTemplateId, GameObjectInfo, 8> m_gameobject_info_map;
    SceneWKPtr m_scene;
};

}