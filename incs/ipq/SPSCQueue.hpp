/*================================================================================

File: SPSCQueue.hpp                                                             
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
class SPSCQueue : public IQueueCRTP<SPSCQueue<Item, Capacity>, Item, Capacity>
{
  using Base = IQueueCRTP<SPSCQueue<Item, Capacity>, Item, Capacity>;
  using Base::data;
  using Base::wrap_mask;
  using Base::flush_mask;

  public:
    explicit SPSCQueue(const int fd) :
      Base(fd),
      _local_read_idx(0),
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
      if (_local_read_idx == _cached_write_idx) [[unlikely]]
      {
        _cached_write_idx = data->write_idx.load(std::memory_order_acquire);
        if (_local_read_idx == _cached_write_idx) [[unlikely]]
          return false;
      }

      out = std::move(data->buffer[_local_read_idx & wrap_mask]);
      _local_read_idx++;
      return true;
    }

  private:
    size_t _local_read_idx;
    size_t _local_write_idx;
    size_t _cached_write_idx;
};

}