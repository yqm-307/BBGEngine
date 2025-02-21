#pragma once
#include <engine/ecs/EcsDefine.hpp>
#include <engine/ecs/filter/TagSet.hpp>

namespace engine::ecs
{

class EntityFilter
{
public:
    EntityFilter() {}
    ~EntityFilter() = default;

    template<class TComponent>
    EntityFilter&           AddTag();
    EntityFilter&           AddTag(TagSet tags);
    TagSet&                 GetTags();
    bool                    Filter(EntitySPtr gameobj);

private:
    TagSet m_filter_tags;
};


template<class TComponent>
EntityFilter& EntityFilter::AddTag()
{
    m_filter_tags.AddTag(Tag{BBT_REFLEX_GET_TYPEID(TComponent)});
    return *this;
}

} // namespace engine::ecs