#include <sstream>
#include "util/other/Uuid.hpp"

namespace util::other
{

void Uuid::Generator(boost::uuids::uuid& uuid)
{
    static boost::uuids::random_generator _generator;
    uuid = _generator();
}

std::size_t Uuid::Hash::operator()(const Uuid& uuid)
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


bool Uuid::ToString(char* uuid, size_t len) const
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

const std::string& Uuid::ToString() const
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


}