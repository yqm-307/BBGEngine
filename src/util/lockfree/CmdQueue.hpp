#pragma once
#include <bbt/core/buffer/Buffer.hpp>
#include <bbt/network/Define.hpp>
#include <bbt/core/thread/sync/Queue.hpp>

namespace util
{

struct Cmd
{
    int cmd;
    bbt::network::ConnId connid;
    bbt::core::Buffer buffer;
};

class CmdQueue:
    boost::noncopyable
{
public:
    CmdQueue(size_t default_size):m_cmd_queue(default_size) {}
    ~CmdQueue() = default;

    void Push(Cmd* cmd)
    {
        Assert(m_cmd_queue.Push(cmd));
    }

    bool Pop(Cmd*& cmd)
    {
        return m_cmd_queue.Pop(cmd);
    }

    bool Empty()
    {
        return m_cmd_queue.Empty();
    }
private:
    bbt::core::thread::Queue<Cmd*> m_cmd_queue;
};

} // namespace util