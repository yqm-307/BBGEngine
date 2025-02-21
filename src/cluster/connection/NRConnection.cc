#include <cluster/connection/NRConnection.hpp>
#include <cluster/registery/RegisteryBase.hpp>
#

namespace cluster
{

NRConnection::NRConnection(RegisteryBase* base, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms):
    util::network::Connection(conn, timeout_ms),
    m_registery(base)
{
}

void NRConnection::OnRecv(const char* data, size_t len)
{
    bbt::core::Buffer buffer{data, len};
    auto err = m_registery->RecvFromNode(buffer);
    if (err != std::nullopt)
        OnError(err.value());
}

void NRConnection::OnSend(const bbt::errcode::Errcode& err, size_t succ_len)
{
    if (err.IsErr())
        OnError(err);
}

void NRConnection::OnTimeout()
{
}

void NRConnection::OnClose()
{
    m_registery->OnNodeLoseConnection(GetConnId());
}

void NRConnection::OnError(const bbt::errcode::Errcode& err)
{
    m_registery->OnError(err);
}

} // namespace cluster