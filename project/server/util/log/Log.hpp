#pragma once
#include <bbt/Logger/Logger.hpp>

/**
 *  Debug log 和 Release log 的区别
 *  前者有更详细的信息，后者消耗更小
 */

//-------------------------------------------------------------------------------------
// Begin

#define GAME_BASE_LOG_WARN(fmt,  ...)   BBT_BASE_LOG_WARN(fmt,  ##__VA_ARGS__)
#define GAME_BASE_LOG_INFO(fmt,  ...)   BBT_BASE_LOG_INFO(fmt,  ##__VA_ARGS__)
#define GAME_BASE_LOG_ERROR(fmt, ...)   BBT_BASE_LOG_ERROR(fmt, ##__VA_ARGS__)
#define GAME_BASE_LOG_FATAL(fmt, ...)   BBT_BASE_LOG_FATAL(fmt, ##__VA_ARGS__)

// End
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
// Begin

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