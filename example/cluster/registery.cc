#include <cluster/registery/RegisteryTcp.hpp>


int main()
{
    cluster::RegisteryTcp registery_tcp;

    registery_tcp.Init(bbt::net::IPAddress{"127.0.0.1", 10021}, 3000);

    registery_tcp.Start();

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}