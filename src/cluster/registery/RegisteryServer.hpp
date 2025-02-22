#pragma once
#include <util/network/TcpServer.hpp>
#include <cluster/connection/NRConnection.hpp>

namespace cluster
{

class RegisteryServer:
    public util::network::TcpServer
{
public:
    RegisteryServer(std::weak_ptr<RegisteryBase> base, const std::string& ip, short port, int connect_timeout);
    virtual ~RegisteryServer() = default;
protected:
    std::shared_ptr<util::network::Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) override
    {
        auto nr_conn = std::make_shared<NRConnection>(conn, 5000);
        auto connid = nr_conn->GetConnId();
        nr_conn->SetCallbacks({
            .on_recv_callback = [=](auto, const char* data, size_t len)
            {
                if (m_registery_weak.expired())
                    return;
                auto registery = m_registery_weak.lock();
                registery->RecvFromNode(connid, bbt::core::Buffer{data, len});
            },
            .on_send_callback = [=](auto, const bbt::errcode::Errcode& err, size_t succ_len) { nr_conn->OnSend(err, succ_len); },
            .on_close_callback = [=](auto, auto) { nr_conn->OnClose(); },
            .on_timeout_callback = [=](auto) { nr_conn->OnTimeout(); },
            .on_err_callback = [=](auto, const bbt::errcode::Errcode& err) { nr_conn->OnError(err); },
        });

        return nr_conn;
    }
private:
    std::weak_ptr<RegisteryBase> m_registery_weak;
};

}