#pragma once






//---------------------------------- 一些方便使用的属性的宏定义 ----------------------------------//

//TODO 需要放在bbt里面

/* 出现在函数定义的最前端（返回值之前）。如果调用此函数丢弃了返回值，编译时会报错 */
#define Attr_RetVal [[nodiscard("返回值(错误码)需要进行处理")]]

