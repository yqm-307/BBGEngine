#include "util/errcode/ErrCode.hpp"

namespace game::util::errcode
{

ErrCode::ErrCode(const std::string& err_info,ErrType errtype,int errcode)
    :m_err_info(err_info),
    m_err_type(errtype),
    m_errcode(errcode)
{}

ErrCode::ErrCode()
    :m_err_info("nothing"),
    m_err_type(ErrType::Nothing),
    m_errcode(0)
{}

void ErrCode::SetInfo(const std::string& errinfo)
{
    m_err_info = errinfo;
}


void ErrCode::SetInfo(const char* fmt,...)
{
    char data[128];
    size_t i = 0;
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(data + i, 128 - i, fmt, ap);
    va_end(ap);
    m_err_info = data;
}

void ErrCode::SetCode(int code)
{ 
    m_errcode = code; 
}


void ErrCode::SetType(ErrType type)
{ 
    m_err_type = type; 
}


void ErrCode::Set(const std::string& err_info,ErrType errtype,int errcode)
{
    m_err_info = err_info;
    m_err_type = errtype;
    m_errcode = errcode;
}


const std::string& ErrCode::What() const
{ 
    return m_err_info; 
} 

int ErrCode::Err() const
{ 
    return m_errcode; 
}

}