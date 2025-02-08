#pragma once
#include "plugin/ecs/Define.hpp"

namespace share::ecs::buff
{

typedef int64_t BuffId;
typedef int32_t BuffTid;

enum BuffType : int32_t
{

};

// Buff事件的类型
enum BuffTriggerType
{
    ON_REAL_HP_CHANGE                   = 1,    // 当前生命值发生改变
};

}