#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <engine/ecs/scene/Scene.hpp>
#include <engine/ecs/system/System.hpp>
#include <engine/ecs/component/Component.hpp>
#include <engine/ecs/filter/EntityFilter.hpp>
#include <plugin/ecs/gameobject/GameObject.hpp>

class PosComp:
    public engine::ecs::Component
{
public:
    PosComp() {};
    virtual ~PosComp() = default;
    BBT_REFLEX_DYN_TYPEINFO_METHOD(PosComp);

    void OnUpdate() override {}

    int x{0};
    int y{0};
};

class CommandComp:
    public engine::ecs::Component
{
public:
    CommandComp() {};
    virtual ~CommandComp() = default;
    BBT_REFLEX_DYN_TYPEINFO_METHOD(CommandComp);

    void OnUpdate() override {}

    int cmd{0};
};

class VelocityComp:
    public engine::ecs::Component
{
public:
    VelocityComp() {};
    virtual ~VelocityComp() = default;
    BBT_REFLEX_DYN_TYPEINFO_METHOD(VelocityComp);

    void OnUpdate() override {}

    int x{1};
    int y{1};
};


class MoveSystem:
    public engine::ecs::System
{
public:

    void OnInitFilter(std::shared_ptr<engine::ecs::EntityFilter> filter) override
    {
        filter->AddTag<PosComp>().AddTag<VelocityComp>();
    }

    void OnUpdate() override
    {
        for (auto& go_wkptr : m_gameobjects)
        {
            if (go_wkptr.expired())
                continue;
            auto go_sptr = go_wkptr.lock();
            auto pos = go_sptr->GetComponent<PosComp>();
            auto vel = go_sptr->GetComponent<VelocityComp>();
            pos->x += vel->x;
            pos->y += vel->y;
        }
    }
};

#define DefineTestComponent(name) \
class name: \
    public engine::ecs::Component \
{ \
public: \
    name() {}; \
    virtual ~name() = default; \
    BBT_REFLEX_DYN_TYPEINFO_METHOD(name); \
    void OnUpdate() override {} \
};


DefineTestComponent(TestComp1);
DefineTestComponent(TestComp2);
DefineTestComponent(TestComp3);
DefineTestComponent(TestComp4);
DefineTestComponent(TestComp5);

std::map<engine::ecs::EntityId, int> g_gameobject_count_map;

class FilterTestSystem:
    public engine::ecs::System
{
public:

    void OnInitFilter(std::shared_ptr<engine::ecs::EntityFilter> filter) override
    {
        filter->AddTag<TestComp2>().AddTag<TestComp3>().AddTag<TestComp4>();
    }
    void OnUpdate() override
    {
        for (auto& go_wkptr : m_gameobjects)
        {
            if (go_wkptr.expired())
                continue;
            auto go_sptr = go_wkptr.lock();
            auto test2 = go_sptr->GetComponent<TestComp2>();
            auto test3 = go_sptr->GetComponent<TestComp3>();
            auto test4 = go_sptr->GetComponent<TestComp4>();

            BOOST_TEST_MESSAGE( "component_count=" << go_sptr->GetComponentCount() );
            BOOST_CHECK_EQUAL(g_gameobject_count_map[go_sptr->GetId()], go_sptr->GetComponentCount());
            BOOST_CHECK_NE(test2, nullptr);
            BOOST_CHECK_NE(test3, nullptr);
            BOOST_CHECK_NE(test4, nullptr);
        }
    }
};

BOOST_AUTO_TEST_SUITE(SceneTest)

// 单独场景 Update
BOOST_AUTO_TEST_CASE(t_scene_alone_update)
{
    auto scene = std::make_shared<engine::ecs::Scene>();
    scene->Init();
    scene->RegistComponent<PosComp>("PosComp");
    scene->RegistComponent<VelocityComp>("VelocityComp");

    auto move_system = std::make_shared<MoveSystem>();
    scene->RegistSystem(move_system, engine::ecs::EntityFilter());
    

    auto go = scene->GetEntityMgr()->Create<plugin::ecs::gameobject::GameObject>();
    BOOST_ASSERT(go->AddComponent<PosComp>());
    BOOST_ASSERT(go->AddComponent<VelocityComp>());

    scene->Update();
    
    auto pos = go->GetComponent<PosComp>();
    BOOST_CHECK_EQUAL(pos->x, 1);
    BOOST_CHECK_EQUAL(pos->y, 1);

}

BOOST_AUTO_TEST_CASE(t_entity_filter_test)
{
    auto scene = std::make_shared<engine::ecs::Scene>();
    scene->Init();
    scene->RegistComponent<TestComp1>("TestComp1");
    scene->RegistComponent<TestComp2>("TestComp2");
    scene->RegistComponent<TestComp3>("TestComp3");
    scene->RegistComponent<TestComp4>("TestComp4");
    scene->RegistComponent<TestComp5>("TestComp5");

    auto filter_system = std::make_shared<FilterTestSystem>();
    scene->RegistSystem(filter_system, engine::ecs::EntityFilter());
    
    auto go_1 = scene->GetEntityMgr()->Create<plugin::ecs::gameobject::GameObject>();
    {
        BOOST_ASSERT(go_1->AddComponent<TestComp1>());
        g_gameobject_count_map[go_1->GetId()] = go_1->GetComponentCount();
    }

    auto go_12 = scene->GetEntityMgr()->Create<plugin::ecs::gameobject::GameObject>();
    {
        BOOST_ASSERT(go_12->AddComponent<TestComp1>());
        BOOST_ASSERT(go_12->AddComponent<TestComp2>());
        g_gameobject_count_map[go_12->GetId()] = go_12->GetComponentCount();
    }

    auto go_123 = scene->GetEntityMgr()->Create<plugin::ecs::gameobject::GameObject>();
    {
        BOOST_ASSERT(go_123->AddComponent<TestComp1>());
        BOOST_ASSERT(go_123->AddComponent<TestComp2>());
        BOOST_ASSERT(go_123->AddComponent<TestComp3>());
        g_gameobject_count_map[go_123->GetId()] = go_123->GetComponentCount();
    }

    auto go_1234 = scene->GetEntityMgr()->Create<plugin::ecs::gameobject::GameObject>();
    {
        BOOST_ASSERT(go_1234->AddComponent<TestComp1>());
        BOOST_ASSERT(go_1234->AddComponent<TestComp2>());
        BOOST_ASSERT(go_1234->AddComponent<TestComp3>());
        BOOST_ASSERT(go_1234->AddComponent<TestComp4>());
        g_gameobject_count_map[go_1234->GetId()] = go_1234->GetComponentCount();
    }

    auto go_12345 = scene->GetEntityMgr()->Create<plugin::ecs::gameobject::GameObject>();
    {
        BOOST_ASSERT(go_12345->AddComponent<TestComp1>());
        BOOST_ASSERT(go_12345->AddComponent<TestComp2>());
        BOOST_ASSERT(go_12345->AddComponent<TestComp3>());
        BOOST_ASSERT(go_12345->AddComponent<TestComp4>());
        BOOST_ASSERT(go_12345->AddComponent<TestComp5>());
        g_gameobject_count_map[go_12345->GetId()] = go_12345->GetComponentCount();
    }

    auto go_234 = scene->GetEntityMgr()->Create<plugin::ecs::gameobject::GameObject>();
    {
        BOOST_ASSERT(go_234->AddComponent<TestComp2>());
        BOOST_ASSERT(go_234->AddComponent<TestComp3>());
        BOOST_ASSERT(go_234->AddComponent<TestComp4>());
        g_gameobject_count_map[go_234->GetId()] = go_234->GetComponentCount();
    }

    scene->Update();

}

BOOST_AUTO_TEST_SUITE_END()