/*================================================================================

File: SharedSnapshot.tpp                                                        
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-10 18:45:29                                                 
last edited: 2025-06-10 18:45:29                                                

================================================================================*/

#pragma once

#include "SharedSnapshot.hpp"

#include <new>

template <typename Item>
template <typename ForwardItem>
HOT void SharedSnapshot<Item>::push(ForwardItem &&data) noexcept
{
  _version.fetch_add(1, std::memory_order_release);
  _data = std::forward<ForwardItem>(data);
  _version.fetch_add(1, std::memory_order_release);
}

template <typename Item>
template <typename... Args>
HOT void SharedSnapshot<Item>::emplace(Args &&...args)
{
  _version.fetch_add(1, std::memory_order_release);
  std::destroy_at(&_data);
  new (&_data) Item(std::forward<Args>(args)...);
  _version.fetch_add(1, std::memory_order_release);
}

template <typename Item>
HOT bool SharedSnapshot<Item>::try_pop(Item &data, uint64_t &last_version) noexcept
{
  const uint64_t version = _version.load(std::memory_order_acquire);

  const bool is_odd = (version & 1);
  const bool is_old = (last_version == version);

  if (is_odd | is_old) [[likely]]
    return false;

  last_version = version;
  data = std::move(_data);

  return version == _version.load(std::memory_order_acquire);
}
