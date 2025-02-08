#pragma once
#include "Helper.hpp"
#include "util/assert/Assert.hpp"
#include <bbt/base/Logger/Logger.hpp>

namespace util::test
{


template<typename SampleType>
EasyHelper<SampleType>::EasyHelper(uint32_t max_round):
    Helper<SampleType, EasyHelperResult>(max_round, g_easyhelper_level_map, EasyHelperResult::emFailed)
{
}

template<typename SampleType>
void EasyHelper<SampleType>::Start()
{
    BaseClassType::__Process();
}

template<typename SampleType>
void EasyHelper<SampleType>::SetSample(const std::vector<TestSample<SampleType>>& vec, const typename BaseClassType::SampleGeneratorFunc& generator)
{
    BaseClassType::__SetFixedSamples(vec);
    BaseClassType::__SetSampleGenerator(generator);
}

template<typename SampleType>
void EasyHelper<SampleType>::SetHandler(const typename BaseClassType::OnTestCallback& cb)
{
    BaseClassType::SetTestHandler(cb);
}

template<typename SampleType>
void EasyHelper<SampleType>::PrintResult()
{
    BaseClassType::__PrintHelperInfo();
}

template<typename SampleType>
int32_t EasyHelper<SampleType>::GetCountByResultType(EasyHelperResult type)
{
    return BaseClassType::__GetNumByResultType(type);
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
    __Process();
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::__SetFixedSamples(const std::vector<SampleType>& vec)
{
    m_fixed_sample_arr = vec;
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::__SetSampleGenerator(const SampleGeneratorFunc& generator)
{
    m_generate_sample_func = generator;
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::__PrintHelperInfo()
{
    std::string str = "";
    str = str + "total sample: " + std::to_string(m_max_round) + "\n"; 
    for (auto && set : m_result_map)
    {
        str = str + "sample type: " + m_result_level_map[set.first] + "\tnum: " + std::to_string(set.second.size()) + "\n";
    }
    
    printf("%s\n", bbt::log::format_green(str.c_str(), str.size()).c_str());
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::__Process()
{
    AssertWithInfo(m_test_handler != nullptr, "please check and make sure test handler not empty!");

    for (int cur_round = 0; cur_round < m_max_round; ++cur_round) 
    {
        auto sample = __GetASample();
        auto result = m_test_handler(sample, m_cur_round);
        __OnResult(sample, result);
    }
}

template<typename SmplType, typename ResultType>
typename Helper<SmplType, ResultType>::SampleType Helper<SmplType, ResultType>::__GetASample()
{
    if (m_cur_round > 0 && m_cur_round <= m_fixed_sample_arr.size()) {
        return m_fixed_sample_arr[m_cur_round - 1];
    }

    return m_generate_sample_func(m_cur_round);
}

template<typename SmplType, typename ResultType>
void Helper<SmplType, ResultType>::__OnResult(SampleType& sample, const TestResultType& result)
{
    TestResultType type = result;

    // 未知的结果，视为失败
    if (m_result_level_map.find(result) == m_result_level_map.end()) {
        type = m_default_type;
    }

    // 结果集不存在，则创建新的集合
    if (m_result_map.find(type) == m_result_map.end()) {
        m_result_map.insert(std::make_pair(type, std::map<SampleId, SampleType>()));
    }

    m_result_map[type].insert(std::make_pair(sample.GetID(), sample));
    m_cur_round++;
}

template<typename SmplType, typename ResultType>
Helper<SmplType, ResultType>::Helper(
    uint32_t max_round,
    std::unordered_map<TestResultType, std::string> level_map,
    TestResultType default_type)
    :m_max_round(max_round),
    m_result_level_map(std::move(level_map)),
    m_default_type(default_type)
{
}

template<typename SmplType, typename ResultType>
Helper<SmplType, ResultType>::~Helper(){}

template<typename SmplType, typename ResultType>
int32_t Helper<SmplType, ResultType>::__GetNumByResultType(EasyHelperResult type)
{
    auto it = m_result_map.find(type);
    if (it == m_result_map.end()) {
        return -1;
    }

    return it->second.size();
}

}