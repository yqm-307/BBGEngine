#include "util/errcode/ErrCode.hpp"

namespace util::errcode
{

ErrCode::ErrCode(const std::string& err_info,ErrType errtype,int errcode)
    :BaseClassType(err_info, {errtype, errcode})
{}

ErrCode::ErrCode()
    :BaseClassType("nothing", {ErrType::Nothing, 0})
{}

void ErrCode::SetInfo(const std::string& errinfo)
{
    SetMsg(errinfo);
}


void ErrCode::SetInfo(const char* fmt,...)
{
    char data[128];
    size_t i = 0;
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(data + i, 128 - i, fmt, ap);
    va_end(ap);
    SetMsg(std::string(data));
}

void ErrCode::SetCode(int code)
{ 
    auto pair = GetErrType();
    pair.second = code;
    SetErrType(pair);
}


void ErrCode::SetType(ErrType type)
{ 
    auto pair = GetErrType();
    pair.first = type;
    SetErrType(pair); 
}


void ErrCode::Set(const std::string& err_info,ErrType errtype,int errcode)
{
    SetMsg(err_info);
    SetErrType({errtype, errcode});
}


const std::string& ErrCode::What() const
{ 
    return GetMsg(); 
} 

const char* ErrCode::CWhat() const
{
    return GetCMsg();
}


int ErrCode::GetCode() const
{ 
    return GetErrType().second; 
}

const ErrType& ErrCode::Type() const
{
    return GetErrType().first;
}

}