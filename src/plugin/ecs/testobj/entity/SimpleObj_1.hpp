#pragma once
#include <engine/ecs/entity/Entity.hpp>


namespace plugin::ecs::entity::testobj
{



class SimpleObj_1:
    public engine::ecs::Entity
{
    GameObjectClassMetaInfo;
public:
    ~SimpleObj_1();

    virtual void OnUpdate() override;

    static int AllUpdateTimes();
    static void ResetAllUpdateTimes();
    int AloneUpdateTimes();

    static std::shared_ptr<SimpleObj_1> FastCreate();
protected:
    explicit SimpleObj_1();
    virtual void OnFatherDead() const override {}
private:
    static int create_num;

    static int destory_num;

    static int update_num;

    int m_owner_update_num{0};
};



} // namespace share::ecs::entity::testobj
