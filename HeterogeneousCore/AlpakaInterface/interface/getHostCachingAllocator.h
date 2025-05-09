#ifndef HeterogeneousCore_AlpakaInterface_interface_getHostCachingAllocator_h
#define HeterogeneousCore_AlpakaInterface_interface_getHostCachingAllocator_h

#include <alpaka/alpaka.hpp>

// #include "FWCore/Utilities/interface/thread_safety_macros.h"
#include "HeterogeneousCore/AlpakaInterface/interface/AllocatorConfig.h"
#include "HeterogeneousCore/AlpakaInterface/interface/CachingAllocator.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/host.h"

#if !defined __CLING__ && !defined __INTEL_COMPILER && !defined __NVCC__
#define CMS_THREAD_SAFE [[cms::thread_safe]]
#define CMS_SA_ALLOW [[cms::sa_allow]]
#define CMS_THREAD_GUARD(_var_) [[cms::thread_guard(#_var_)]]
#else
#define CMS_THREAD_SAFE
#define CMS_SA_ALLOW
#define CMS_THREAD_GUARD(_var_)
#endif

namespace cms::alpakatools {

  template <typename TQueue, typename = std::enable_if_t<alpaka::isQueue<TQueue>>>
  inline CachingAllocator<alpaka_common::DevHost, TQueue>& getHostCachingAllocator(
      AllocatorConfig const& config = AllocatorConfig{}, bool debug = false) {
    // thread safe initialisation of the host allocator
    CMS_THREAD_SAFE static CachingAllocator<alpaka_common::DevHost, TQueue> allocator(
        host(),
        config,
        false,  // reuseSameQueueAllocations
        debug);

    // the public interface is thread safe
    return allocator;
  }

}  // namespace cms::alpakatools

#endif  // HeterogeneousCore_AlpakaInterface_interface_getHostCachingAllocator_h
