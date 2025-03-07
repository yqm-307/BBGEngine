#pragma once
#include <bbt/core/log/Logger.hpp>

/**
 *  Debug log 和 Release log 的区别
 *  前者有更详细的信息，后者消耗更小
 * 
 *  也许在线上环境，我们可以考这些减少磁盘IO的开销，因此我们允许查log时麻烦一点
 */

//-------------------------------------------------------------------------------------
// Begin

#define GAME_BASE_LOG_DEBUG(fmt, ...)   BBT_BASE_LOG_DEBUG(fmt,  ##__VA_ARGS__)
#define GAME_BASE_LOG_WARN(fmt,  ...)   BBT_BASE_LOG_WARN(fmt,  ##__VA_ARGS__)
#define GAME_BASE_LOG_INFO(fmt,  ...)   BBT_BASE_LOG_INFO(fmt,  ##__VA_ARGS__)
#define GAME_BASE_LOG_ERROR(fmt, ...)   BBT_BASE_LOG_ERROR(fmt, ##__VA_ARGS__)
#define GAME_BASE_LOG_FATAL(fmt, ...)   BBT_BASE_LOG_FATAL(fmt, ##__VA_ARGS__)

// End
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
// Begin

// TODO 接入SysLog的相关接口
/* 携带拓展信息的日志接口如下: */
#ifdef Debug

#define GAME_EXT1_LOG_TRACE(fmt, ...)   BBT_FULL_LOG_TRACE(fmt, ##__VA_ARGS__)
#define GAME_EXT1_LOG_DEBUG(fmt, ...)   BBT_FULL_LOG_DEBUG(fmt, ##__VA_ARGS__)
#define GAME_EXT1_LOG_WARN(fmt,  ...)   BBT_FULL_LOG_WARN(fmt,  ##__VA_ARGS__)
#define GAME_EXT1_LOG_INFO(fmt,  ...)   BBT_FULL_LOG_INFO(fmt,  ##__VA_ARGS__)

#else

#define GAME_EXT1_LOG_TRACE(fmt, ...)   
#define GAME_EXT1_LOG_DEBUG(fmt, ...)   
#define GAME_EXT1_LOG_WARN(fmt,  ...)   
#define GAME_EXT1_LOG_INFO(fmt,  ...)   

#endif

#define GAME_EXT1_LOG_ERROR(fmt, ...)   BBT_FULL_LOG_ERROR(fmt, ##__VA_ARGS__)
#define GAME_EXT1_LOG_FATAL(fmt, ...)   BBT_FULL_LOG_FATAL(fmt, ##__VA_ARGS__)

// End
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
// Begin

#ifdef OpenSysLog

#define GAME_SYS_LOG_WARN(fmt, ...)     BBT_FULL_LOG_WARN(fmt, ##__VA_ARGS__)
#define GAME_SYS_LOG_INFO(fmt, ...)     BBT_FULL_LOG_INFO(fmt, ##__VA_ARGS__)
#define GAME_SYS_LOG_ERROR(fmt, ...)    BBT_FULL_LOG_ERROR(fmt, ##__VA_ARGS__)
#define GAME_SYS_LOG_FATAL(fmt, ...)    BBT_FULL_LOG_FATAL(fmt, ##__VA_ARGS__)

#else

#define GAME_SYS_LOG_WARN(fmt, ...)
#define GAME_SYS_LOG_INFO(fmt, ...)
#define GAME_SYS_LOG_ERROR(fmt, ...)
#define GAME_SYS_LOG_FATAL(fmt, ...)

#endif

// End
//-------------------------------------------------------------------------------------
