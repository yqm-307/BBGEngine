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
    public bbt::errcode::Errcode
{
public:

    explicit        ErrCode(const std::string& err_info, bbt::errcode::ErrType errtype);
    virtual bool    IsErr() const override;

};

typedef std::optional<util::errcode::ErrCode> ErrOpt;

template<typename ...TRetVal>
using ErrTuple = std::tuple<util::errcode::ErrOpt, TRetVal...>;

}// namespace end