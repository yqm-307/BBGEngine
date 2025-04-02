#include <cluster/protocol/Protocol.hpp>


namespace cluster::protocol
{

using namespace util::errcode;

ErrOpt Helper::ParseProtocolFromBuffer(bbt::core::Buffer& buffer, std::vector<bbt::core::Buffer>& protocols)
{
    // 协议解析
    while (true)
    {
        if (buffer.Size() < sizeof(ProtocolHead))
            break;

        ProtocolHead* head = (ProtocolHead*)buffer.Peek();
        if (buffer.Size() < head->protocol_length)
            break;

        bbt::core::Buffer protocol{head->protocol_length};
        protocol.WriteNull(head->protocol_length);

        if (buffer.ReadString(protocol.Peek(), head->protocol_length))
            protocols.emplace_back(std::move(protocol));
        else
            return Errcode{"parse buffer failed!", util::errcode::emErr::CommonErr};
    }

    return std::nullopt;
}


} // namespace cluster