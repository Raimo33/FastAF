#pragma once

#include "macros.hpp"

//TODO seqlock buffer
//TODO explore smart pointers for zero-copy

template<typename T>
class SharedSnapshot
{
  public:
    void store(const T &data);
    bool load(T &data) const;

  private:
    std::atomic<uint64_t> _version;
    alignas(CACHELINE_SIZE) T _data;
};

#include "SharedSnapshot.tpp"