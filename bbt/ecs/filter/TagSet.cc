#include <bbt/ecs/filter/TagSet.hpp>

namespace bbt::ecs
{

bool operator<(const Tag& lval, const Tag& rval)
{
    return lval.m_tag_id < rval.m_tag_id;
}

bool Tag::operator==(const Tag& other) const
{
    return m_tag_id == other.m_tag_id;
}


bool TagSet::operator==(const TagSet& other)
{
    return m_set_tags == other.m_set_tags;
}

bool TagSet::AddTag(Tag tag)
{
    m_changed = true;
    return m_set_tags.insert(tag).second;
}

bool TagSet::HasTag(TagId id) const
{
    return m_set_tags.find(Tag{id}) != m_set_tags.end();
}

bool TagSet::DelTag(TagId id)
{
    m_changed = true;
    return m_set_tags.erase(Tag{id}) > 0;
}

const std::vector<Tag> TagSet::GetTags()
{
    if (m_changed) {
        m_tag_arr.clear();
        for (auto& it : m_set_tags) {
            m_tag_arr.push_back(it);
        }
        m_changed = false;
    }
    return m_tag_arr;
}

} // namespace bbt::ecs