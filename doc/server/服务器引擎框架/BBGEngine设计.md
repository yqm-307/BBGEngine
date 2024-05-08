## Engine设计

---
<br>

## BBGEngine
---
<br>

### 1、背景
<br>

&emsp;我想阐述一下我对于服务器驱动方式的理解。服务器是长期运行的，且需要对外部事件进行相应，那么常见的方式是建立在一个循环之上的。且可以知道的是循环中比如需要获取外部数据并加以处理，以及内部处理，因此把这一个逻辑整合就是一个服务器的驱动方式。


&emsp;在我看来如何组织驱动服务器内部和外部的事件是问题的关键。在学习libevent的设计之时，深刻感受到了libevent对事件驱动的理解，如果对一个服务器来说，无非就是外部事件（IO事件、信号事件）和内部事件（定时事件、其他事件），而libevent完全支持对于这几种事件的支持，所以使用libevent实现EventLoop来驱动服务器进程是可行的。

---
<br>

### 2、Engine驱动设计
<br>

#### （1）ECS架构
&emsp;BBGEngine根据ECS（Entity-Component-System）架构，该设计模式遵循组合优于继承的设计理念，即每个基本单元都是一个实体，实体由多个组件组成，组件只包含其提供的功能需要的特定数据，例如：Person是一个实体，其具有组件PosComponent，而PosComponent中含有坐标、速度、朝向等数据，我们可以通过MoveSystem修改Person对象中PosComponent的数据来实现实体的移动。下面用伪代码描述上面的例子：

<br>

``` cpp
class Person:
    public Gameobject
{};

class Pos:
    public Component
{};

class MoveSystem
{
public:
    // 此函数大致逻辑：取出obj中的组件，并找到Pos组件，修改Pos组件的数据，完成移动
    template<typename Args>
    static void PersonMove(Gameobject* obj, Args ...arg);
};

// Person移动
{
    Person* person = new Person();
    Pos* pos_comp = new Pos();

    // 将pos component加到person上，此时person具有移动的能力
    person.AddComponent(pos_comp);

    // 操作person的Pos组件完成移动
    MoveSystem::PersonMove(person, ...);
}


```
<br>

&emsp;通过上面的例子介绍了ECS架构的特点，其特点就是以组合代替继承，通过组合为对象提供功能，可以看的出来Entity的存在类似一个容器，通过保存不同的Component来获得不同的能力，而且我们通过System来操作Component可以屏蔽掉外部对Component的访问。

&emsp;对游戏来说无疑是一个很好的设计方式。比如此时有个需求，需要让Person获得开门的功能，那么我们只需要横向拓展（即添加新的Component、System），即可完成需求。伪代码如下：

``` cpp

// Door组件赋予身份
class Door:
    public Component
{};

// OpenDoor组件赋予能力，可以对具有Door组件的对象进行操作
class OpenDoor:
    public Component
{};

class OpenDoorSystem
{
public:
    static void OpenDoor(Gameobject* obj, Door* door_obj);
};

{
    // 创建两个对象
    Person* player = new Person();
    Person* door = new Person();

    // 创建door和open door组件
    OpenDoor* open_door_comp = new Person();
    Door* door_comp = new Door();

    // 分别赋予两个对象不同的组件
    door.AddComponent(door_comp);
    player.AddComponent(open_door_comp);

    // 进行开门操作
    OpenDoorSystem::OpenDoor(player, door);
}

```

<br>

&emsp;因为目前没有见到有服务器是运用这种模式进行开发的，所以我体会不到优缺点，只能是自己尝试实现一个简单的游戏服务器来感受一下，但是对我来说是一个非常感兴趣的设计思路，其使用组件提供功能，让对象功能的动态性很强，可以在运行时对对象身上的组件增加或减少，可能实现很多有趣的机制，所以我决定尝试深入一下。

<br>

#### （2）Scene和游戏世界
<br>

&emsp;游戏世界。我的认知中，游戏服务器逻辑上应当分为两个部分，游戏世界和游戏外逻辑。游戏世界应该以玩法为主（战斗、地图等），而游戏外逻辑应该以功能和逻辑为主（活动、背包等）。前者更像是真实的，可以在脑海中浮现出的仿真世界，有角色在运动、攻击，npc在跑动等等；后者更像是纯数据结构的组合。


&emsp;服务器应该如何驱动着整个游戏世界的变化呢？大多都是通过定时事件来做的，定时遍历游戏世界里面的每个对象，并使其状态进行更新。因此引入了Scene，Scene作为世界的基石，保证了前面说的驱动，Scene可以协助更新整个游戏世界（挂载在Scene下的）。伪代码如下：

<br>

``` cpp
class MainScene:
    public Scene
{};

class Person:
    public Gameobject
{};

// 场景的应用
{
    Gameobject* person = new Person();
    Scene* scene = new MainScene();

    scene->Mount(person);

    // 游戏世界开始更新，这里借助libevent来做了，BBGEngine可以让其他时间处理其他事件
    while (true)
    {
        scene->Update();
        sleep(1);
    }
}
```
&emsp;场景就是用来承载Gameobject的东西，并驱动游戏世界更新，我们只需要更新场景即可。其实不止游戏，很多服务也可以使用这个设计来开发。

---
<br>

### 3、基于事件的服务器驱动设计

<br>

&emsp;BBGEngine提供了Scene可以遍历整个游戏世界进行更新，不过把调用更新的权利提供出来，让使用者自己控制以什么规则和频率来更新整个世界。

&emsp;现在对于BBGEngine的设计基本上已经确定了，但是作为一个服务器框架仍然需要其他的功能，比如network模块、mysql、redis的接入。因为BBGEngine本身也需要被驱动，所以我选择用libevent来做事件驱动，Scene通过定时事件驱动，网络等io相关的使用io事件驱动。

&emsp;如果使用libevent融合其他的库，就需要封装一层，屏蔽掉具体功能之间的差异。这一步需要用到一些复杂的手法。