#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace util::other
{

class Uuid
{
public:
    explicit Uuid();
    explicit Uuid(const Uuid& other);
    explicit Uuid(Uuid&& other);
    virtual ~Uuid();

    Uuid& operator=(const Uuid& other);
    Uuid& operator=(Uuid&& other);

    bool ToString(char* uuid, size_t len) const;
    bool ToString(std::string& uuid) const;
    std::string ToString() const;

    bool IsNil() const;

private:
    static void Generator(boost::uuids::uuid& uuid);
private:
    boost::uuids::uuid                      m_uuid;
};

}