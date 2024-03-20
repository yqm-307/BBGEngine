#pragma once
#include "share/ecs/attribute/AttrDefine.hpp"

namespace share::ecs::attr
{

class AttrModule
{
public:
    typedef std::shared_ptr<AttrModule> SPtr;

    /* 当前模块属性是否发生了修改 */
    bool                    IsModify();
    /* 标记当前模块发生了修改 */
    void                    MarkModify(bool is_modify);
    /* 此模块属性进行重算 */
    virtual void            Recalculate() = 0;
    /* 获取此模块的属性数据 */
    AttributeData*          GetAttributeData();
protected:
    AttributeData*      m_attribute_data{nullptr};
    bool                m_is_modified{false};
};

} // namespace share::ecs::attr