#pragma once
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include "util/errcode/ErrCodeDef.hpp"

namespace util::errcode
{


class ErrCode
{
public:
    explicit ErrCode(const std::string& err_info,ErrType errtype,int errcode);

    ErrCode();
    void SetInfo(const std::string& errinfo);

    void SetInfo(const char* fmt, ...);


    void SetCode(int code);


    void SetType(ErrType type);


    void Set(const std::string& err_info,ErrType errtype,int errcode);


    const std::string& What() const;

    int GetErrCode() const;

    ErrType GetErrType() const;


private:
    std::string     m_err_info;
    ErrType         m_err_type;
    int             m_errcode;

};

}// namespace end