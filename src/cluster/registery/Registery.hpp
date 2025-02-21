#pragma once
#include <cluster/registery/RegisterInfo.hpp>

namespace cluster
{



class Registery
{
public:
    Registery();
    ~Registery();

    util::errcode::ErrCode Recv(bbt::core::Buffer& buffer);
    util::errcode::ErrCode Send(bbt::core::Buffer& buffer);

private:
    util::errcode::ErrCode OnRegister(bbt::core::Buffer& buffer);
    util::errcode::ErrCode OnUnRegister(bbt::core::Buffer& buffer);
    util::errcode::ErrCode OnHeartBeat(bbt::core::Buffer& buffer);

private:
    std::unordered_map<std::string, int> m_registery_map;
};

}