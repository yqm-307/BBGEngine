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
     * @return bbt::buffer::Buffer 
     */
    template<typename ...Args>
    bbt::buffer::Buffer Serialize(Args... args)
    {
        bbt::buffer::Buffer buffer;
        SerializeArgs(buffer, args...);
        return buffer;
    }

    template<typename ...Args>
    void SerializeAppend(bbt::buffer::Buffer& buffer, Args... args)
    {
        SerializeArgs(buffer, args...);
    }

    util::errcode::ErrTuple<std::vector<FieldValue>> Deserialize(bbt::buffer::Buffer& buffer)
    {
        std::vector<FieldValue> values;
        while (buffer.ReadableBytes() > 0)
        {
            FieldValue value;
            auto err = DeserializeOne(buffer, value);
            if (err != std::nullopt)
                return {err, values};
            
            values.push_back(value);
        }

        return {std::nullopt, values};
    }

    util::errcode::ErrOpt DeserializeOne(bbt::buffer::Buffer& buffer, FieldValue& value)
    {
        if (buffer.ReadableBytes() < sizeof(value.header))
            return util::errcode::ErrCode("deserialize failed, buffer too short!", util::errcode::ErrType::CommonErr);
        
        buffer.ReadString((char*)&(value.header), sizeof(value.header));
        switch (value.header.field_type)
        {
        case INT64:
            value.value.int64_value = buffer.ReadInt64();
            break;
        case UINT64:
            value.value.uint64_value = buffer.ReadInt64();
            break;
        case INT32:
            value.value.int32_value = buffer.ReadInt32();
            break;
        case UINT32:
            value.value.uint32_value = buffer.ReadInt32();
            break;
        case STRING:
            value.string.resize(value.header.field_len);
            buffer.ReadString(value.string.data(), value.string.size());
            break;
        default:
            return util::errcode::ErrCode("deserialize failed, not support this type!", util::errcode::ErrType::CommonErr);
        }

        return std::nullopt;
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

    void SerializeArg(bbt::buffer::Buffer& buffer, const char* arg)
    {
        FieldHeader header;
        header.field_type = STRING;
        header.field_len = strlen(arg);
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

    void SerializeArg(bbt::buffer::Buffer& buffer, int32_t arg)
    {
        FieldHeader header;
        header.field_type = INT32;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.WriteInt32(arg);
    }

    void SerializeArg(bbt::buffer::Buffer& buffer, uint32_t arg)
    {
        FieldHeader header;
        header.field_type = UINT32;
        header.field_len = sizeof(arg);
        buffer.WriteString((char*)&header, sizeof(header));
        buffer.WriteInt32(arg);
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