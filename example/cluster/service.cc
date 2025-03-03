#include <cluster/node/ClusterNode.hpp>

class CustomService : public cluster::ClusterNode
{
public:
    CustomService() = default;
    virtual ~CustomService() = default;

    virtual void OnError(const bbt::errcode::Errcode& err) override
    {
        std::cout << "[error][Node] " << err.CWhat() << std::endl;
    }
    void OnInfo(const std::string& info) override
    {
        std::cout << "[info ][Node] " << info << std::endl;
    }
    void OnDebug(const std::string& debug) override
    {
        std::cout << "[debug][Node] " << debug << std::endl;
    }
};


int main()
{
    auto node = std::make_shared<CustomService>();

    node->Init(bbt::net::IPAddress{"127.0.0.1", 10022}, bbt::net::IPAddress{"127.0.0.1", 10021}, 5000);
    auto err = node->Start();
    if (err != std::nullopt) {
        std::cout << "start failed: " << err->CWhat() << std::endl;
        return -1;
    }

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        node->Update();
    }

    node->Stop();
}