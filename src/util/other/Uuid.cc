#include <sstream>
#include "util/other/Uuid.hpp"

namespace util::other
{

void Uuid::Generator(boost::uuids::uuid& uuid)
{
    static boost::uuids::random_generator _generator;
    uuid = _generator();
}

std::size_t Uuid::Hash::operator()(const Uuid& uuid) const
{
    if (uuid.IsNil())
        return 0;

    std::hash<std::string> hash;
    return hash(uuid.ToString());
}


Uuid::Uuid()
{
    Generator(m_uuid);
}

Uuid::Uuid(const Uuid& other):
    m_uuid(other.m_uuid)
{
}

Uuid::Uuid(Uuid&& other):
    m_uuid(std::move(other.m_uuid))
{
}

Uuid::Uuid(const char* uuid, size_t len)
{
    FromByte(uuid, len);
}



Uuid::~Uuid() {}

Uuid& Uuid::operator=(const Uuid& other)
{
    m_uuid = other.m_uuid;
    return *this;
}

Uuid& Uuid::operator=(Uuid&& other)
{
    m_uuid = std::move(other.m_uuid);
    return *this;
}

bool Uuid::operator==(const Uuid& other) const
{
    return (m_uuid == other.m_uuid);
}

bool Uuid::FromByte(const char* uuid, size_t len)
{
    if (len != m_uuid.size())
        return false;

    for (int i = 0; i < len; ++i)
    {
        m_uuid.data[i] = uuid[i];
    }

    return true;
}

bool Uuid::ToByte(char* uuid, size_t len) const
{
    if (len <= m_uuid.size())
        return false;

    int index = 0;
    for (auto it = m_uuid.begin(); it != m_uuid.end(); ++it)
    {
        uuid[index] = *it;
    }

    return true;
}

bool Uuid::ToString(std::string& uuid) const
{
    std::stringstream f;

    if (m_uuid.is_nil())
        return false;

    f << m_uuid;
    uuid = f.str();
    return true;
}

std::string Uuid::ToString() const
{
    std::stringstream f;

    if (m_uuid.is_nil())
        return "";

    f << m_uuid;
    return f.str();
}

bool Uuid::IsNil() const
{
    return m_uuid.is_nil();
}

void Uuid::Clear()
{
    m_uuid = boost::uuids::nil_uuid();
}

// 实现 < 运算符
bool Uuid::operator<(const Uuid& other) const
{
    return m_uuid < other.m_uuid;
}

}