#include <cluster/registery/RegisteryServer.hpp>
#include <cluster/registery/Registery.hpp>

namespace cluster
{

RegisteryServer::RegisteryServer(std::shared_ptr<bbt::network::libevent::Network> network, std::shared_ptr<Registery> base, const std::string& ip, short port, int connect_timeout):
    util::network::TcpServer(network, ip, port, connect_timeout),
    m_registery_weak(base),
    m_connect_timeout(connect_timeout)
{
}

std::shared_ptr<util::network::Connection> RegisteryServer::CreateConnection(bbt::network::libevent::ConnectionSPtr conn)
{
    auto nr_conn = std::make_shared<util::network::Connection>(conn, m_connect_timeout);
    auto connid = nr_conn->GetConnId();

    nr_conn->SetCallbacks({
        .on_recv_callback = [weak_this{weak_from_this()}, connid](auto, const char* data, size_t len)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                if (auto registery = std::static_pointer_cast<RegisteryServer>(shared_this)->m_registery_weak.lock(); registery != nullptr) {
                    bbt::core::Buffer buffer{data, len};
                    if (auto err = registery->RecvFromNode(connid, buffer); err != std::nullopt) {
                        registery->OnError(err.value());
                    }
                }
            }
        },
        .on_send_callback = [weak_this{weak_from_this()}, connid](auto, const bbt::errcode::Errcode& err, size_t succ_len)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                if (auto registery = std::static_pointer_cast<RegisteryServer>(shared_this)->m_registery_weak.lock(); registery != nullptr) {
                    if (err.IsErr()) {
                        registery->OnError(err);
                    }
                }
            }
        },
        .on_close_callback = [weak_this{weak_from_this()}, connid](auto, auto)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                if (auto registery = std::static_pointer_cast<RegisteryServer>(shared_this)->m_registery_weak.lock(); registery != nullptr) {
                    registery->OnClose(connid);
                }
            }
        },
        .on_timeout_callback = [weak_this{weak_from_this()}, connid](auto)
        {
            // 这里超时后，回继续调用OnClose，所以在onclose中删除连接
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                if (auto registery = std::static_pointer_cast<RegisteryServer>(shared_this)->m_registery_weak.lock(); registery != nullptr) {
                    registery->OnTimeout(connid);
                }
            }
        },
        .on_err_callback = [weak_this{weak_from_this()}, connid](auto, const bbt::errcode::Errcode& err)
        {
            if (auto shared_this = weak_this.lock(); shared_this != nullptr) {
                if (auto registery = std::static_pointer_cast<RegisteryServer>(shared_this)->m_registery_weak.lock(); registery != nullptr) {
                    registery->OnError(err);
                }
            }
        },
    });

    auto shared_node = m_registery_weak.lock();
    if (shared_node != nullptr) {
        shared_node->OnAccept(connid);
    }

    return nr_conn;
}

} // namespace cluster