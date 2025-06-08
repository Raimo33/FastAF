/*================================================================================

File: SPSCQueue.hpp                                                             
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-05-12 18:01:10                                                 
last edited: 2025-06-08 13:31:29                                                

================================================================================*/

#pragma once

#include "IQueueCRTP.hpp"

namespace ipq
{

template <typename Item, size_t Capacity>
class SPSCQueue : public IQueueCRTP<SPSCQueue<Item, Capacity>, Item, Capacity>
{
  using Base = IQueueCRTP<SPSCQueue<Item, Capacity>, Item, Capacity>;
  using Base::data;
  using Base::wrap_mask;
  using Base::flush_mask;

  public:
    explicit SPSCQueue(const int fd) : Base(fd) {}

    template <typename ForwardItem>
    void push_impl(ForwardItem &&item) noexcept
    {
      static thread_local size_t local_write_idx;

      data->buffer[local_write_idx & wrap_mask] = std::forward<ForwardItem>(item);

      if ((local_write_idx & flush_mask) == 0) [[unlikely]]
        data->write_idx.store(local_write_idx + 1, std::memory_order_release);

      local_write_idx++;
    }

    bool pop_impl(Item &out) noexcept
    {
      static thread_local size_t local_read_idx;
      static thread_local size_t cached_write_idx;

      if (local_read_idx == cached_write_idx) [[unlikely]]
      {
        cached_write_idx = data->write_idx.load(std::memory_order_acquire);
        if (local_read_idx == cached_write_idx) [[unlikely]]
          return false;
      }

      out = std::move(data->buffer[local_read_idx & wrap_mask]);
      local_read_idx++;
      return true;
    }
};

}