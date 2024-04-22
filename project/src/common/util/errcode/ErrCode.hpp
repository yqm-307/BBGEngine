#pragma once
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <optional>
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

    const char* CWhat() const override;

    int GetCode() const;

    const ErrTypePair& Type() const;
protected:

};

typedef std::optional<util::errcode::ErrCode> ErrOpt;

template<typename ...TRetVal>
using ErrTuple = std::tuple<util::errcode::ErrOpt, TRetVal...>;

}// namespace end