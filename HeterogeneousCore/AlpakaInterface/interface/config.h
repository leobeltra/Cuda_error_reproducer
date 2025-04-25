#ifndef HeterogeneousCore_AlpakaInterface_interface_config_h
#define HeterogeneousCore_AlpakaInterface_interface_config_h

#include <type_traits>

#include <alpaka/alpaka.hpp>

#define EDM_STRINGIZE_(token) #token
#define EDM_STRINGIZE(token) EDM_STRINGIZE_(token)

namespace alpaka_common {

  // common types and dimensions
  using Idx = uint32_t;
  using Extent = uint32_t;
  using Offsets = Extent;

  using Dim0D = alpaka::DimInt<0u>;
  using Dim1D = alpaka::DimInt<1u>;
  using Dim2D = alpaka::DimInt<2u>;
  using Dim3D = alpaka::DimInt<3u>;

  template <typename TDim>
  using Vec = alpaka::Vec<TDim, Idx>;
  using Vec1D = Vec<Dim1D>;
  using Vec2D = Vec<Dim2D>;
  using Vec3D = Vec<Dim3D>;
  using Scalar = Vec<Dim0D>;

  template <typename TDim>
  using WorkDiv = alpaka::WorkDivMembers<TDim, Idx>;
  using WorkDiv1D = WorkDiv<Dim1D>;
  using WorkDiv2D = WorkDiv<Dim2D>;
  using WorkDiv3D = WorkDiv<Dim3D>;

  // host types
  using DevHost = alpaka::DevCpu;
  using PlatformHost = alpaka::Platform<DevHost>;

}  // namespace alpaka_common

#ifdef ALPAKA_ACC_GPU_CUDA_ENABLED
namespace alpaka_cuda_async {
  using namespace alpaka_common;

  using Platform = alpaka::PlatformCudaRt;
  using Device = alpaka::DevCudaRt;
  using Queue = alpaka::QueueCudaRtNonBlocking;
  using Event = alpaka::EventCudaRt;

  template <typename TDim>
  using Acc = alpaka::AccGpuCudaRt<TDim, Idx>;
  using Acc1D = Acc<Dim1D>;
  using Acc2D = Acc<Dim2D>;
  using Acc3D = Acc<Dim3D>;

}  // namespace alpaka_cuda_async

#ifdef ALPAKA_HOST_ONLY

namespace alpaka {

  template <typename TApi, typename TAcc, typename TDim, typename TIdx, typename TKernelFnObj, typename... TArgs>
  class TaskKernelGpuUniformCudaHipRt final {
    static_assert(std::is_same_v<TApi, alpaka::ApiCudaRt> and BOOST_LANG_CUDA,
                  "You should move this files to a .dev.cc file under the alpaka/ subdirectory.");

  public:
    template <typename TWorkDiv>
    ALPAKA_FN_HOST TaskKernelGpuUniformCudaHipRt(TWorkDiv&& workDiv, TKernelFnObj const& kernelFnObj, TArgs&&... args) {
    }
  };

}  // namespace alpaka

#endif  // ALPAKA_HOST_ONLY

#define ALPAKA_ACCELERATOR_NAMESPACE alpaka_cuda_async
#define ALPAKA_TYPE_SUFFIX CudaAsync

#endif  // ALPAKA_ACC_GPU_CUDA_ENABLED

#if defined ALPAKA_ACCELERATOR_NAMESPACE

// create a new backend-specific identifier based on the original type name and a backend-specific suffix
#define ALPAKA_TYPE_ALIAS__(TYPE, SUFFIX) TYPE##SUFFIX
#define ALPAKA_TYPE_ALIAS_(TYPE, SUFFIX) ALPAKA_TYPE_ALIAS__(TYPE, SUFFIX)
#define ALPAKA_TYPE_ALIAS(TYPE) ALPAKA_TYPE_ALIAS_(TYPE, ALPAKA_TYPE_SUFFIX)

// declare the backend-specific identifier as an alias for the namespace-based type name
#define DECLARE_ALPAKA_TYPE_ALIAS(TYPE) using ALPAKA_TYPE_ALIAS(TYPE) = ALPAKA_ACCELERATOR_NAMESPACE::TYPE

// define a null-terminated string containing the backend-specific identifier
#define ALPAKA_TYPE_ALIAS_NAME(TYPE) EDM_STRINGIZE(ALPAKA_TYPE_ALIAS(TYPE))

#endif  // ALPAKA_ACCELERATOR_NAMESPACE

#endif  // HeterogeneousCore_AlpakaInterface_interface_config_h
