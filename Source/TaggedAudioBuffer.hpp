#pragma once

#include "macros.h"
DISABLE_WARNINGS
#include <JuceHeader.h>
ENABLE_WARNINGS

#include "OwnedMap.hpp"
#include "StaticVector.hpp"

template<typename KeyType, size_t Capacity>
class TaggedAudioBuffer
{
public:
    static constexpr auto MAX_NUM_SAMPLES = 4096;
    static constexpr size_t CAPACITY = Capacity;
    using key_type = KeyType;
    using value_type = juce::AudioBuffer<float>;

private:
    using container_type = OwnedMap<key_type, value_type, Capacity>;
    container_type mBuffers{};
    int mNumSamples{};

public:
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    void init(juce::Array<key_type> const & channels)
    {
        JUCE_ASSERT_MESSAGE_THREAD;
        jassert(channels.size() <= narrow<int>(CAPACITY));

        mBuffers.clear();
        for (auto const key : channels) {
            mBuffers.add(key, std::make_unique<juce::AudioBuffer<float>>(1, MAX_NUM_SAMPLES));
        }
    }

    void setNumSamples(int const numSamples)
    {
        JUCE_ASSERT_MESSAGE_THREAD;
        jassert(numSamples <= MAX_NUM_SAMPLES);
        mNumSamples = numSamples;
        for (auto buffer : mBuffers) {
            buffer.value->setSize(1, numSamples);
        }
    }

    [[nodiscard]] int size() const { return mBuffers.size(); }

    void silence()
    {
        for (auto buffer : mBuffers) {
            buffer.value->clear();
        }
    }
    void silence(key_type const channel, int const numSamples) { (*this)[channel].clear(); }

    [[nodiscard]] value_type & operator[](key_type const key) { return mBuffers[key]; }

    [[nodiscard]] value_type const & operator[](key_type const key) const { return mBuffers[key]; }

    [[nodiscard]] juce::Array<float const *> getArrayOfReadPointers(juce::Array<key_type> const & keys) const
    {
        JUCE_ASSERT_MESSAGE_THREAD;
        juce::Array<float const *> result{};
        result.ensureStorageAllocated(keys.size());
        for (auto const key : keys) {
            result.add((*this)[key].getReadPointer(0));
        }
        return result;
    }

    [[nodiscard]] StaticVector<float *, CAPACITY>
        getArrayOfWritePointers(StaticVector<key_type, CAPACITY> const & channels)
    {
        StaticVector<float *, CAPACITY> result{};
        for (auto const key : channels) {
            result.push_back((*this)[key].getWritePointer(0));
        }
        return result;
    }

    [[nodiscard]] int getNumSamples() const { return mNumSamples; }

    void copyToPhysicalOutput(float * const * outs, int const numOutputs) const
    {
        for (auto const buffer : mBuffers) {
            auto const outIndex{ buffer.key.removeOffset<decltype(numOutputs)>() };
            jassert(outIndex >= 0);
            if (outIndex < numOutputs) {
                auto const * origin{ buffer.value->getReadPointer(0) };
                auto * dest{ outs[outIndex] };
                std::transform(origin, origin + mNumSamples, dest, dest, std::plus());
            }
        }
    }

    [[nodiscard]] iterator begin() { return mBuffers.begin(); }
    [[nodiscard]] iterator end() { return mBuffers.end(); }
    [[nodiscard]] const_iterator begin() const { return mBuffers.cbegin(); }
    [[nodiscard]] const_iterator end() const { return mBuffers.cend(); }
    [[nodiscard]] const_iterator cbegin() const { return mBuffers.cbegin(); }
    [[nodiscard]] const_iterator cend() const { return mBuffers.cend(); }
};

using SourceAudioBuffer = TaggedAudioBuffer<source_index_t, MAX_INPUTS>;
using SpeakerAudioBuffer = TaggedAudioBuffer<output_patch_t, MAX_OUTPUTS>;