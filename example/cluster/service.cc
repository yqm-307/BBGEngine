#include <cluster/node/ClusterNode.hpp>

class CustomService : public cluster::ClusterNode
{
public:
    CustomService() = default;
    virtual ~CustomService() = default;

    virtual void OnError(const bbt::errcode::Errcode& err) override
    {
        std::cout << "error: " << err.CWhat() << std::endl;
    }
};


int main()
{
    
}