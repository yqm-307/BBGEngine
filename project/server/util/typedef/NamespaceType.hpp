#pragma once
#include <memory>

#define SmartPtrTypeDef(classname) \
    typedef std::shared_ptr<classname> classname##SPtr; \
    typedef const std::shared_ptr<classname> classname##CSPtr; \
    typedef std::unique_ptr<classname> classname##UQPtr; \
    typedef const std::unique_ptr<classname> classname##CUQPtr; \
    typedef std::weak_ptr<classname> classname##WKPtr; \
    typedef const std::weak_ptr<classname> classname##CWKPtr; \
    typedef classname* classname##RawPtr;   \
    typedef const classname* classname##CRawPtr;
