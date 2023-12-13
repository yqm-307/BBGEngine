#pragma once
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include "util/errcode/ErrCodeDef.hpp"


namespace util::errcode
{

typedef std::pair<util::errcode::ErrType, int> ErrTypePair;

class ErrCode:
    public bbt::errcode::Errcode<ErrTypePair>
{
    typedef bbt::errcode::Errcode<ErrTypePair> BaseClassType;
public:

    explicit ErrCode(const std::string& err_info,ErrType errtype,int errcode);

    ErrCode();
    void SetInfo(const std::string& errinfo);

    void SetInfo(const char* fmt, ...);


    void SetCode(int code);


    void SetType(ErrType type);


    void Set(const std::string& err_info,ErrType errtype,int errcode);


    const std::string& What() const;

    int GetCode() const;

    ErrType GetType() const;
protected:

};

}// namespace end