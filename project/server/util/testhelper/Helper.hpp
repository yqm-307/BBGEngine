#pragma once
#include <functional>
#include <map>
#include <string>
#include <unordered_set>

#include <bbt/uuid/EasyID.hpp>

namespace util::test
{

enum EasyHelperResult
{
    emOk = 0,
    emFailed = 1,
    emWarning = 2,
};

typedef int32_t SampleId;

static const std::unordered_set<EasyHelperResult> g_easyhelper_level_info = {
    EasyHelperResult::emOk,
    EasyHelperResult::emFailed,
    EasyHelperResult::emWarning,
};
static const std::unordered_map<EasyHelperResult, std::string> g_easyhelper_level_map = {
    {EasyHelperResult::emOk, "success"},
    {EasyHelperResult::emFailed, "failed"},
    {EasyHelperResult::emWarning, "warning"},
};

/**
 * @brief 测试样本
 */
template<typename T>
class TestSample
{
public:
    TestSample(T value):m_sample(value),m_sample_id(m_id_generate.GenerateID())  {}
    ~TestSample() {}
    
    SampleId GetID() { return m_sample_id; }
    T m_sample;
private:
    SampleId m_sample_id;
    static bbt::uuid::EasyID<bbt::uuid::emEasyID::EM_AUTO_INCREMENT, 100> m_id_generate;
};


/**
 * @brief 批量测试类
 */
template<typename SmplType, typename ResultType>
class Helper
{
public:
    typedef TestSample<SmplType> SampleType; /* 测试样本类型 */
    typedef std::function<EasyHelperResult(const SampleType&)> OnTestCallback;
    typedef std::function<SampleType(int32_t)> SampleGeneratorFunc; 
    typedef ResultType TestResultType;

    explicit Helper(uint32_t max_round, std::unordered_map<TestResultType, std::string> level_map, TestResultType default_type);
    ~Helper();

    void SetTestHandler(const OnTestCallback& cb);


    virtual void Start();
protected:
    virtual void __Process() final;
    virtual void __SetFixedSamples(std::vector<SampleType>&& vec) final;
    virtual void __SetSampleGenerator(const SampleGeneratorFunc& generator) final;
    virtual void __PrintHelperInfo() final;

private:
    SampleType  __GetASample();
    void        __OnResult(SampleType& sample, const TestResultType& result);
private:
    OnTestCallback  m_test_handler{nullptr};
    int             m_cur_round{1};
    const int       m_max_round{0};
    
    TestResultType  m_default_type; /* 一个未知的测试结果，会被认为是default类型的 */
    std::unordered_map<TestResultType, std::string> m_result_level_map;
    std::map<TestResultType, std::map<SampleId, SampleType>> m_result_map;

    /* 样本集:(1)固定样本；(2)样本生成器; */
    std::vector<SampleType>     m_fixed_sample_arr;
    SampleGeneratorFunc         m_generate_sample_func{nullptr};

};

template<typename SampleType>
class EasyHelper:
    public Helper<SampleType, EasyHelperResult>
{
    typedef Helper<SampleType, EasyHelperResult> BaseClassType;
public:
    EasyHelper(uint32_t max_round); 
    ~EasyHelper() {}

    void Start();
    void SetSample(std::vector<TestSample<SampleType>>&& vec, const typename BaseClassType::SampleGeneratorFunc& generator);
    void SetHandler(const typename BaseClassType::OnTestCallback& cb);
    void PrintResult();
private:


};


} // namespace util::test


#include "__THelper.hpp"