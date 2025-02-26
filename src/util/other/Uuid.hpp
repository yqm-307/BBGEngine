#pragma once
#include <memory>
#include <string>
#include <set>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/operators.hpp>

namespace util::other
{

class Uuid:
    public boost::equality_comparable<Uuid>,
    public boost::less_than_comparable<Uuid>
{
public:
    typedef std::shared_ptr<Uuid> SPtr;

    struct Hash{ std::size_t operator()(const Uuid& uuid) const; };

    Uuid();
    Uuid(const Uuid& other);
    Uuid(Uuid&& other);
    explicit Uuid(const char* uuid, size_t len);
    virtual ~Uuid();

    Uuid& operator=(const Uuid& other);
    Uuid& operator=(Uuid&& other);

    bool operator==(const Uuid& other) const;
    bool operator<(const Uuid& other) const;  // 实现 < 运算符

    bool FromByte(const char* uuid, size_t len);
    bool ToByte(char* uuid, size_t len) const;
    bool ToString(std::string& uuid) const;
    std::string ToString() const;

    bool IsNil() const;
    void Clear();

private:
    static void Generator(boost::uuids::uuid& uuid);
private:
    boost::uuids::uuid                      m_uuid;
};

}