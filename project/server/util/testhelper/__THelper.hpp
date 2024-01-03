#pragma once
#include "Helper.hpp"
#include "util/assert/Assert.hpp"

namespace util::test
{


template<typename SampleType>
EasyHelper<SampleType>::EasyHelper(uint32_t max_round):
    Helper<SampleType, EasyHelperResult>(max_round, g_easyhelper_level_info, EasyHelperResult::emFailed)
{
}

template<typename SampleType>
void EasyHelper<SampleType>::Start()
{
    BaseClassType::Process();
}

template<typename SampleType>
void EasyHelper<SampleType>::SetSample(std::vector<TestSample<SampleType>>&& vec, const typename BaseClassType::SampleGeneratorFunc& generator)
{
    BaseClassType::SetFixedSamples(std::move(vec));
    BaseClassType::SetSampleGenerator(generator);
}

template<typename SampleType>
void EasyHelper<SampleType>::SetHandler(const typename BaseClassType::OnTestCallback& cb)
{
    BaseClassType::SetTestHandler(cb);
}


//-------------------------------------------------------------------------

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::SetTestHandler(const OnTestCallback& cb)
{
    AssertWithInfo(cb != nullptr, "test handler must not be empty!");
    m_test_handler = cb;
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::Start()
{
    Process();
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::SetFixedSamples(std::vector<SampleType>&& vec)
{
    m_fixed_sample_arr = std::move(vec);
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::SetSampleGenerator(const SampleGeneratorFunc& generator)
{
    m_generate_sample_func = generator;
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::Process()
{
    AssertWithInfo(m_test_handler != nullptr, "please check and make sure test handler not empty!");

    for (int cur_round = 0; cur_round < m_max_round; ++cur_round) 
    {
        auto sample = GetASample();
        auto result = m_test_handler(GetASample());
        OnResult(sample, result);
    }
}

template<typename SmplType, typename ResultType>
const typename Helper<SmplType, ResultType>::SampleType& Helper<SmplType, ResultType>::GetASample()
{
    if (m_cur_round <= m_fixed_sample_arr.size()) {
        return m_fixed_sample_arr[m_cur_round];
    }

    return m_generate_sample_func(m_cur_round);
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::OnResult(SampleType& sample, const TestResultType& result)
{
    TestResultType type = result;

    // 未知的结果，视为失败
    if (m_result_level_set.find(result) != m_result_level_set.end()) {
        type = m_default_type;
    }

    if (m_result_map.find(type) == m_result_map.end()) {
        m_result_map.insert(std::make_pair(type, std::map<SampleId, SampleType>()));
    }

    m_result_map[type].insert(std::make_pair(sample.GetID(), sample));
    m_cur_round++;
}

template<typename SmplType, typename ResultType>
Helper<SmplType, ResultType>::Helper(uint32_t max_round, std::unordered_set<TestResultType> level_set, TestResultType default_type)
    :m_max_round(max_round),
    m_result_level_set(std::move(level_set)),
    m_default_type(default_type)
{
}

template<typename SmplType, typename ResultType>
Helper<SmplType, ResultType>::~Helper()
{

}

}