#pragma once
#include <iostream>
#include "bbt/uuid/EasyID.hpp"
#include "util/managerbase/ManagerBase.hpp"

namespace util::network
{

class IOThread;
SharedWithUniqueDef(IOThread);

class Connection;
SharedWithUniqueDef(Connection);

typedef int64_t ConnectId;

enum GenerateKey {
    EM_ConnectId = 1,
};

extern inline ConnectId GenerateConnectID()
{ return bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT, GenerateKey::EM_ConnectId>::GenerateID(); }

}