/*================================================================================

File: SPMCQueue.hpp                                                             
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-05-12 18:01:10                                                 
last edited: 2025-06-09 20:07:26                                                

================================================================================*/

#pragma once

#include "IQueueCRTP.hpp"

namespace ipq
{

template <typename Item, size_t Capacity>
class SPMCQueue : public IQueueCRTP<SPMCQueue<Item, Capacity>, Item, Capacity>
{
  using Base = IQueueCRTP<SPMCQueue<Item, Capacity>, Item, Capacity>;
  using Base::data;
  using Base::wrap_mask;
  using Base::flush_mask;

  public:
    explicit SPMCQueue(const int fd) :
      Base(fd),
      _local_write_idx(0),
      _cached_write_idx(0) {}

    template <typename ForwardItem>
    void push_impl(ForwardItem &&item) noexcept
    {
      data->buffer[_local_write_idx & wrap_mask] = std::forward<ForwardItem>(item);

      //TODO FIX this slows the consumer
      if ((_local_write_idx & flush_mask) == 0) [[unlikely]]
        data->write_idx.store(_local_write_idx + 1, std::memory_order_release);

      _local_write_idx++;
    }

    bool pop_impl(Item &out) noexcept
    {
      size_t local_read_idx = data->read_idx.load(std::memory_order_relaxed);

      while (true)
      {
        if (local_read_idx == _cached_write_idx) [[unlikely]]
        {
          _cached_write_idx = data->write_idx.load(std::memory_order_acquire);
          if (local_read_idx == _cached_write_idx) [[unlikely]]
            return false;
        }

        if (data->read_idx.compare_exchange_weak(local_read_idx, local_read_idx + 1, std::memory_order_acquire, std::memory_order_relaxed)) [[likely]]
        {
          out = std::move(data->buffer[local_read_idx & wrap_mask]);
          return true;
        }

        std::this_thread::yield();
      }

      std::unreachable();
    }

  private:
    size_t _local_write_idx = 0;
    size_t _cached_write_idx = 0;
};

}