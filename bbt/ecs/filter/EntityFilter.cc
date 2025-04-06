#include <bbt/ecs/entity/Entity.hpp>
#include <bbt/ecs/filter/EntityFilter.hpp>


namespace bbt::ecs
{

bool EntityFilter::Filter(EntitySPtr gameobj)
{
    for (auto& tag : m_filter_tags.GetTags())
    {
        if (!gameobj->GetTagSet().HasTag(tag.GetId()))
        {
            return false;
        }
    }
    return true;
}

EntityFilter& EntityFilter::AddTag(TagSet tags)
{
    for (auto& tag : tags.GetTags())
    {
        m_filter_tags.AddTag(tag);
    }
    return *this;
}

TagSet& EntityFilter::GetTags()
{
    return m_filter_tags;
}



} // namespace bbt::ecs