#pragma once
#include <functional>
#include <map>
#include <string>
#include <unordered_set>


namespace util::test
{

enum DoTestResult
{
    emOk = 0,
    emFailed = 1,
    emWarning = 2,
};

typedef int32_t SampleId;

/**
 * @brief 测试样本
 */
template<typename T>
struct TestSample
{ 
    SampleId m_sample_id;
    T m_sample;
};

template<typename SampleType>
class EasyHelper:
    public Helper
{
public:
    EasyHelper(uint32_t max_round); 
    ~EasyHelper() {}

    void Start();
private:
    static std::unordered_set<std::string> m_level_info;
    static std::unordered_map<DoTestResult, std::string> m_level_map;

};

/**
 * @brief 批量测试类
 */
template<typename SmplType, typename ResultType>
class Helper
{
public:
    typedef TestSample<SmplType> SampleType; /* 测试样本类型 */
    typedef std::function<DoTestResult(const SampleType&)> OnTestCallback;
    typedef std::function<SampleType(int32_t)> SampleGeneratorFunc; 
    typedef ResultType TestResultType;

    Helper(int max_round, std::unordered_set<TestResultType> level_set, TestResultType default_type);
    ~Helper();

    void SetTestHandler(const OnTestCallback& cb);


    virtual void Start();
protected:
    virtual void Process() final;
    virtual void SetFixedSamples(std::vector<SampleType>&& vec) final;
    virtual void SetSampleGenerator(const SampleGeneratorFunc& generator) final;

private:
    const SampleType& GetASample();
    void OnResult(SampleType& sample, const TestResultType& result);
private:
    OnTestCallback  m_test_handler{nullptr};
    int             m_cur_round{1};
    const int       m_max_round{0};
    
    TestResultType  m_default_type; /* 一个未知的测试结果，会被认为是default类型的 */
    std::unordered_set<TestResultType> m_result_level_set;
    std::map<TestResultType, std::map<SampleId, SampleType>> m_result_map;

    /* 样本集:(1)固定样本；(2)样本生成器; */
    std::vector<SampleType>     m_fixed_sample_arr;
    SampleGeneratorFunc         m_generate_sample_func{nullptr};

};

} // namespace util::test


#include "__THelper.hpp"