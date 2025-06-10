/*================================================================================

File: SharedSnapshot.hpp                                                        
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-10 18:45:29                                                 
last edited: 2025-06-10 18:45:29                                                

================================================================================*/

#pragma once

#include "macros.hpp"

#include <cstdint>
#include <atomic>

template <typename Item>
class SharedSnapshot
{
  static_assert(std::is_trivially_constructible_v<Item>, "Item must be trivially constructible");
  static_assert(std::is_trivially_destructible_v<Item>, "Item must be trivially destructible");

  public:
    SharedSnapshot(void) = default;
    ~SharedSnapshot(void) = default;

    template <typename ForwardItem>
    void push(ForwardItem &&data) noexcept;

    template <typename... Args>
    void emplace(Args &&...args);

    bool try_pop(Item &data, size_t &last_version) noexcept;

  private:
    SharedSnapshot(const SharedSnapshot &) = delete;
    SharedSnapshot(SharedSnapshot &&) = delete;
    SharedSnapshot &operator=(const SharedSnapshot &) = delete;

    alignas(CACHELINE_SIZE) std::atomic<uint64_t> _version{0};
    alignas(CACHELINE_SIZE) Item _data;
};

#include "SharedSnapshot.tpp"