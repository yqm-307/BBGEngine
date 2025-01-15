#include "util/errcode/ErrCode.hpp"

namespace util::errcode
{

ErrCode::ErrCode(const std::string& err_info, bbt::errcode::ErrType errtype, int errnum):
    bbt::errcode::Errcode(err_info, errtype),
    m_errnum(errnum)
{
}

bool ErrCode::IsErr() const
{
    return (Type() != util::errcode::ErrType::Nothing);
}

int ErrCode::GetErrNum() const
{
    return m_errnum;
}

}