#pragma once
#include <bbt/base/buffer/Buffer.hpp>
#include "share/ecs/network/Connection.hpp"

namespace share::ecs::network
{

typedef std::function<bool(bbt::buffer::Buffer&, bbt::buffer::Buffer&)> DBServiceCPPFuncPtr;

class DBServiceSession;

class DBServiceConnection:
    public Connection
{
public:
    DBServiceConnection(ConnMgr* mgr, bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms);
    virtual ~DBServiceConnection();
    static void             RegistHandler(int protoid, DBServiceCPPFuncPtr handler);
protected:
    void                    OnRecv(const char* data, size_t len) override;
    virtual void            OnClose() override;
    virtual void            OnTimeout() override;
    static bool             Dispatch(int protoid, bbt::buffer::Buffer& buf, bbt::buffer::Buffer& resp);

    std::pair<bool, int>    GetAProtocol(bbt::buffer::Buffer& protocol);
    /**
     * @brief 是否有一个完整的buffer
     * 
     * @param data 数据
     * @param remain_size buffer剩余长度 
     * @return true 
     * @return false 
     */
    int                     _HasAProtocol(const char* data, size_t remain_size);
private:

    static std::unordered_map<int, DBServiceCPPFuncPtr> m_proto_handler_map;
    bbt::buffer::Buffer m_recv_buffer; // 接收缓存，保存不完整数据包
};

}