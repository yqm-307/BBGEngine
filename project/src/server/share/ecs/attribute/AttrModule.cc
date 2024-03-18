#include "share/ecs/attribute/AttrModule.hpp"


namespace share::ecs::attr
{

bool AttrModule::IsModify()
{
    return m_is_modified;
}

AttributeData* AttrModule::GetAttributeData()
{
    return m_attribute_data;
}

void AttrModule::MarkModify(bool is_modify)
{
    m_is_modified = is_modify;
}


}