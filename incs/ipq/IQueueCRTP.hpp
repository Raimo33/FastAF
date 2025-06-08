/*================================================================================

File: IQueueCRTP.hpp                                                            
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-05-12 18:01:10                                                 
last edited: 2025-06-08 13:31:29                                                

================================================================================*/

#pragma once

#include <cstddef>
#include <fcntl.h>
#include <sys/mman.h>
#include <atomic>
#include <array>
#include <exception>

#ifndef CACHELINE_SIZE
# define CACHELINE_SIZE 64
#endif

template <size_t N>
concept PowerOfTwo = (N > 0) && ((N & (N - 1)) == 0);

namespace ipq
{

template <typename Derived, typename Item, size_t Capacity>
  requires PowerOfTwo<Capacity>
class IQueueCRTP
{
  public:

    explicit IQueueCRTP(const int fd)
    {
      bool error = false;

      error |= ftruncate(fd, sizeof(SharedData)) == -1;
      void *addr = mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, fd, 0);
      error |= (addr == MAP_FAILED);
      error |= mlock(addr, sizeof(SharedData)) == -1;

      data = static_cast<SharedData*>(addr);

      if (error) [[unlikely]]
        throw_error("Failed to initialize shared memory queue");
    }

    ~IQueueCRTP(void) noexcept {
      munmap(data, sizeof(SharedData));
    }

    template <typename ForwardItem>
    inline void push(ForwardItem &&item) noexcept {
      derived()->push_impl(std::forward<ForwardItem>(item));
    }
    
    inline bool pop(Item &out) noexcept {
      return derived()->pop_impl(out);
    }

  protected:
    static constexpr size_t wrap_mask = Capacity - 1;
    static constexpr size_t flush_frequency = Capacity / 4;
    static constexpr size_t flush_mask = flush_frequency - 1;

    struct SharedData
    {
      alignas(CACHELINE_SIZE) std::atomic<size_t> write_idx;
      alignas(CACHELINE_SIZE) std::atomic<size_t> read_idx;
      alignas(CACHELINE_SIZE) std::array<Item, Capacity> buffer;
    };

    SharedData *data;

  private:
    inline Derived *derived(void) noexcept { return static_cast<Derived*>(this); }
    inline const Derived *derived(void) const noexcept { return static_cast<const Derived*>(this); }

    static void throw_error(std::string_view message)
    {
    #ifdef __cpp_exceptions
      throw std::runtime_error(message);
    #else
      write(STDERR_FILENO, message.data(), message.size());
      write(STDERR_FILENO, "\n", 1);
      std::abort();
    #endif
    }
};

}
