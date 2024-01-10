#pragma once
#include <iostream>
#include <optional>
#include <map>

#include <bbt/buffer/Buffer.hpp>
#include <bbt/uuid/EasyID.hpp>

#include "util/typedef/NamespaceType.hpp"
#include "util/errcode/ErrCode.hpp"
#include "util/managerbase/ManagerBase.hpp"
#include "util/network/IPAddress.hpp"

namespace util::network
{

class IOThread;
SharedWithUniqueDef(IOThread);

class Connection;
SharedWithUniqueDef(Connection);

typedef int64_t ConnectId;
typedef int32_t ThreadId;

enum GenerateKey {
    EM_ConnectId = 1,
    EM_ThreadId  = 2,
};

extern inline ConnectId GenerateConnectID()
{ return bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT, GenerateKey::EM_ConnectId>::GenerateID(); }

extern inline ConnectId GenerateThreadID()
{ return bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT_SAFE, GenerateKey::EM_ThreadId>::GenerateID(); }

}