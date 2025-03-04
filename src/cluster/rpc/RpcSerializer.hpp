#pragma once
#include <cluster/rpc/Define.hpp>

namespace cluster
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
        int32_t int32_value;
        int64_t uint32_value;
        int64_t int64_value;
        uint64_t uint64_value;
    } value;
    std::string string;
};

#pragma pack(pop)

//TODO 先支持最简单的类型，后续拓展
enum FieldType : int8_t
{
    INT64 = 1,
    UINT64,
    INT32,
    UINT32,
    STRING,
};

class RpcSerializer
{
public:
    /**
     * @brief 将args打包为字节流并返回
     * 
     * @tparam Args 
     * @param args 
     * @return bbt::core::Buffer 
     */
    template<typename ...Args>
    bbt::core::Buffer Serialize(Args... args)
    {
        bbt::core::Buffer buffer;
        SerializeArgs(buffer, args...);
        return buffer;
    }

    template<typename ...Args>
    void SerializeAppend(bbt::core::Buffer& buffer, Args... args)
    {
        SerializeArgs(buffer, args...);
    }

    bbt::errcode::ErrTuple<std::vector<FieldValue>> Deserialize(bbt::core::Buffer& buffer)
    {
        std::vector<FieldValue> values;
        while (buffer.Size() > 0)
        {
            FieldValue value;
            auto err = DeserializeOne(buffer, value);
            if (err != std::nullopt)
                return {err, values};
            
            values.push_back(value);
        }

        return {std::nullopt, values};
    }

    bbt::errcode::ErrOpt DeserializeOne(bbt::core::Buffer& buffer, FieldValue& value)
    {
        if (buffer.Size() < sizeof(value.header))
            return bbt::errcode::Errcode{"deserialize failed, buffer too short!", util::errcode::emErr::CommonErr};
        
        buffer.ReadString((char*)&(value.header), sizeof(value.header));
        switch (value.header.field_type)
        {
        case INT64:
            Assert(buffer.Read(&value.value.int64_value));
            break;
        case UINT64:
            Assert(buffer.Read(&value.value.uint64_value));
            break;
        case INT32:
            Assert(buffer.Read(&value.value.int32_value));
            break;
        case UINT32:
            Assert(buffer.Read(&value.value.uint32_value));
            break;
        case STRING:
            value.string.resize(value.header.field_len);
            buffer.ReadString(value.string.data(), value.string.size());
            break;
        default:
            return bbt::errcode::Errcode("deserialize failed, not support this type!", util::errcode::emErr::CommonErr);
        }

        return std::nullopt;
    }
    

private:
    template<typename T>
    void SerializeArg(bbt::core::Buffer& bytes, T arg)
    {
        AssertWithInfo(false, "Not support this type");
    }

    void SerializeArg(bbt::core::Buffer& buffer, const std::string& arg)
    {
        FieldHeader header;
        header.field_type = STRING;
        header.field_len = arg.size();
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.WriteString(arg.c_str(), arg.size());
    }

    void SerializeArg(bbt::core::Buffer& buffer, const char* arg)
    {
        FieldHeader header;
        header.field_type = STRING;
        header.field_len = strlen(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.WriteString(arg, strlen(arg));
    }

    void SerializeArg(bbt::core::Buffer& buffer, int64_t arg)
    {
        FieldHeader header;
        header.field_type = INT64;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.Write(arg);
    }

    void SerializeArg(bbt::core::Buffer& buffer, uint64_t arg)
    {
        FieldHeader header;
        header.field_type = UINT64;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.Write(arg);
    }

    void SerializeArg(bbt::core::Buffer& buffer, int32_t arg)
    {
        FieldHeader header;
        header.field_type = INT32;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.Write(arg);
    }

    void SerializeArg(bbt::core::Buffer& buffer, uint32_t arg)
    {
        FieldHeader header;
        header.field_type = UINT32;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.Write(arg);
    }

    template<typename T, typename... Args>
    void SerializeArgs(bbt::core::Buffer& buffer, T arg, Args... args)
    {
        SerializeArg(buffer, arg);
        SerializeArgs(buffer, args...);
    }

    void SerializeArgs(bbt::core::Buffer& buffer) {}
};

}