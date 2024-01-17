#pragma once
#include <cstring>
#include <bbt/errcode/Errcode.hpp>

namespace util::errcode
{



enum ErrType
{
    Nothing = 1,
    Common = 2,
    NetWorkErr = 100,   // 网络err
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
namespace errenum { 

enum MODULE_COMMON
{
    Failed = 1,
    Success = 2,
};

enum MODULE_NETWORK
{
    Default                         = 0,    // 未知，如果遇到，需要确定err原因并完善err枚举表
    TimeOut                         = 1,    // 操作超时
    Failed                          = 2,    // 通用。失败通知

    //--------- socket ---------//     
    Socket_BadFd                    = 101,  // 错误的socket fd

    //--------- network recv ---------//     
    Recv_Success                    = 2000, // 读取成功
    Recv_Eof                        = 2001, // 对端连接已关闭
    Recv_Connect_Refused            = 2002, // 连接被拒绝
    Recv_TryAgain                   = 2003, // 当前繁忙，重试
    Recv_Other_Err                  = 2004, // 其他错误，查看errno
    Send_Success                    = 2005, // 发送完毕

    //--------- event ---------//     
    Event_Register_Failed           = 2101, // 事件注册失败

    //--------- network accept ---------//     
    Accept_TryAgain                 = 2151, // 重试一次

};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
enum MODULE_ECS_AOI
{

};

}


}// namespace end