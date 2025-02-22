#include "util/errcode/ErrCode.hpp"

namespace util::errcode
{

ErrCode::ErrCode(const std::string& err_info, bbt::errcode::ErrType errtype):
    bbt::errcode::Errcode(err_info, errtype)
{
}

bool ErrCode::IsErr() const
{
    return (Type() != util::errcode::ErrType::Nothing);
}

}