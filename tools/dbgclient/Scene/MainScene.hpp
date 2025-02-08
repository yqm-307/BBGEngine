#include "plugin/ecs/network/NetworkComponent.hpp"
#include "plugin/scene/testscene/SampleScene.hpp"

namespace dbgclient::scene
{

class MainScene:
    public engine::scene::Scene
{
public:
    ~MainScene();

protected:
    MainScene(const char* ip, short port);
private:
};

}