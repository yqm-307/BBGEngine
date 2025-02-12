#pragma once
#include <plugin/ecs/rpc/Define.hpp>

namespace plugin::ecs::rpc
{

#pragma pack(push, 1)

struct FieldHeader
{
    int8_t field_type;
    int16_t field_len;
};

struct FieldValue
{
    FieldHeader header;
    union RpcSerializerValue
    {
        int64_t int64_value;
        uint64_t uint64_value;
        std::string string_value;
    } value;
};

#pragma pack(pop)

//TODO 先支持最简单的类型，后续拓展
enum FieldType : int8_t
{
    INT64 = 1,
    UINT64,
    STRING,
};

class RpcSerializer
{
public:
    template<typename ...Args>
    bbt::buffer::Buffer Serialize(Args... args)
    {
        bbt::buffer::Buffer buffer;
        SerializeArgs(buffer, args...);
        return buffer;
    }

    template<typename ...Args>
    std::vector<FieldValue> Deserialize(bbt::buffer::Buffer& buffer)
    {
        std::vector<FieldValue> values;
        while (buffer.ReadableBytes() > 0)
        {
            FieldValue value;
            buffer.ReadString((char*)&(value.header), sizeof(value.header));
            switch (value.header.field_type)
            {
            case INT64:
                value.value.int64_value = buffer.ReadInt64();
                break;
            case UINT64:
                value.value.uint64_value = buffer.ReadInt64();
                break;
            case STRING:
                buffer.ReadString(value.value.string_value, value.header.field_len);
                break;
            default:
                AssertWithInfo(false, "Not support this type");
                break;
            }
            values.push_back(value);
        }
        return values;
    }

private:
    template<typename T>
    void SerializeArg(bbt::buffer::Buffer& bytes, T arg)
    {
        AssertWithInfo(false, "Not support this type");
    }

    void SerializeArg(bbt::buffer::Buffer& buffer, const std::string& arg)
    {
        FieldHeader header;
        header.field_type = STRING;
        header.field_len = arg.size();
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.WriteString(arg);
    }

    void SerializeArg(bbt::buffer::Buffer& buffer, int64_t arg)
    {
        FieldHeader header;
        header.field_type = INT64;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.WriteInt64(arg);
    }

    void SerializeArg(bbt::buffer::Buffer& buffer, uint64_t arg)
    {
        FieldHeader header;
        header.field_type = UINT64;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.WriteInt64(arg);
    }

    template<typename T, typename... Args>
    void SerializeArgs(bbt::buffer::Buffer& buffer, T arg, Args... args)
    {
        SerializeArg(buffer, arg);
        SerializeArgs(buffer, args...);
    }

    void SerializeArgs(bbt::buffer::Buffer& buffer) {}
};

}