#pragma once
#include <engine/ecs/EcsDefine.hpp>

namespace engine::ecs
{

class Tag:
    public boost::less_than_comparable<Tag>,
    public boost::equality_comparable<Tag>
{
public:
    friend bool operator<(const Tag&, const Tag&);
    bool operator==(const Tag& other) const;
    Tag(TagId id):m_tag_id(id) {}
    Tag(const Tag& other) = default;
    Tag(Tag&& other) noexcept = default;
    ~Tag() = default;

    Tag& operator=(const Tag& other) = default;
    Tag& operator=(Tag&& other) noexcept = default;

    TagId GetId() const { return m_tag_id; }
    void SetId(TagId id) { m_tag_id = id; }
private:
    TagId m_tag_id{0};
};

bool operator<(const Tag& lval, const Tag& rval);

class TagSet:
    public boost::equality_comparable<Tag>
{
public:
    TagSet() = default;
    TagSet(const TagSet& other) = default;
    TagSet(TagSet&& other) noexcept = default;
    ~TagSet() = default;
    TagSet& operator=(const TagSet& other) = default;
    TagSet& operator=(TagSet&& other) noexcept = default;


    bool operator==(const TagSet& other);

    bool AddTag(Tag tag);
    bool HasTag(TagId id) const;
    bool DelTag(TagId id);
    const std::vector<Tag> GetTags();

private:
    bool                m_changed{true};
    std::set<Tag>       m_set_tags;
    std::vector<Tag>    m_tag_arr;
};

} // namespace engine::ecs