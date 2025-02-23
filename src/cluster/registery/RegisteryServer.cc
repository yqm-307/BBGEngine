#include <cluster/registery/RegisteryServer.hpp>
#include <cluster/registery/RegisteryBase.hpp>
#include <cluster/connection/NRConnection.hpp>

namespace cluster
{

RegisteryServer::RegisteryServer(std::weak_ptr<RegisteryBase> base, const std::string& ip, short port, int connect_timeout):
    util::network::TcpServer(ip, port, connect_timeout),
    m_registery_weak(base)
{
}

std::shared_ptr<util::network::Connection> RegisteryServer::CreateConnection(bbt::network::libevent::ConnectionSPtr conn)
{
    auto nr_conn = std::make_shared<NRConnection>(conn, 5000);
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
                shared_this->DelConnect(connid);
            }
        },
        .on_timeout_callback = [weak_this{weak_from_this()}, connid](auto)
        {
            // 这里超时后，回继续调用OnClose，所以在onclose中删除连接
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

    return nr_conn;
}

} // namespace cluster