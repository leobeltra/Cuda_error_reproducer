# Compiler and tools
CXX := c++
NVCC := /usr/local/cuda/bin/nvcc

# Paths
CUDA_PATH := /usr/local/cuda
BOOST_PATH := /cvmfs/cms.cern.ch/el8_amd64_gcc12/external/boost/1.80.0-7642db88b69862429c1c9c9980662789
ALPAKA_PATH := external/alpaka/include
EIGEN_PATH := external/eigen

BOOST_MACRO := -DBOOST_SPIRIT_THREADSAFE -DBOOST_MATH_DISABLE_STD_FPCLASSIFY -DBOOST_UUID_RANDOM_PROVIDER_FORCE_POSIX -DBOOST_MPL_IGNORE_PARENTHESES_WARNING
ALPAKA_MACRO := -DALPAKA_DEFAULT_HOST_MEMORY_ALIGNMENT=128 -DALPAKA_DISABLE_VENDOR_RNG
ALPAKA_DEVICE_MACRO := $(ALPAKA_MACRO) -DALPAKA_ACC_GPU_CUDA_ENABLED -DALPAKA_ACC_GPU_CUDA_ONLY_MODE -UALPAKA_HOST_ONLY
ALPAKA_HOST_MACRO := $(ALPAKA_MACRO) -DALPAKA_ACC_GPU_CUDA_ENABLED -DALPAKA_ACC_GPU_CUDA_ONLY_MODE -DALPAKA_HOST_ONLY
EIGEN_MACRO := -DEIGEN_DONT_PARALLELIZE -DEIGEN_MAX_ALIGN_BYTES=64
GNU_MACRO := -DCMS_MICRO_ARCH='x86-64-v3' -DGNU_GCC -D_GNU_SOURCE -DPHOENIX_THREADSAFE



# Includes
INCLUDES := -I. -I$(ALPAKA_PATH) -I$(BOOST_PATH)/include -I$(CUDA_PATH)/include -I$(EIGEN_PATH)
LINK_STUBS := -L$(CUDA_PATH)/stubs -lcudadevrt
LINK_CUDA := $(LINK_STUBS) -L$(CUDA_PATH)/lib64

NVCC_ARCH := -gencode arch=compute_60,code=[sm_60,compute_60] \
             -gencode arch=compute_70,code=[sm_70,compute_70] \
             -gencode arch=compute_75,code=[sm_75,compute_75] \
             -gencode arch=compute_80,code=[sm_80,compute_80] \
             -gencode arch=compute_89,code=[sm_89,compute_89] \
             -gencode arch=compute_90,code=[sm_90,compute_90]

CUDA_HOST_COMPILER := --compiler-options '-O3 -pthread -pipe -Werror=main -Werror=pointer-arith -Werror=overlength-strings -Wno-vla -Werror=overflow -ftree-vectorize -Werror=array-bounds -Werror=format-contains-nul -Werror=type-limits -fvisibility-inlines-hidden -fno-math-errno --param vect-max-version-for-alias-checks=50 -Xassembler --compress-debug-sections -Wno-error=array-bounds -Warray-bounds -fuse-ld=bfd -felide-constructors -fmessage-length=0 -Wall -Wno-non-template-friend -Wno-long-long -Wreturn-type -Wextra -Wpessimizing-move -Wclass-memaccess -Wno-cast-function-type -Wno-unused-but-set-parameter -Wno-ignored-qualifiers -Wno-unused-parameter -Wunused -Wparentheses -Werror=return-type -Werror=missing-braces -Werror=unused-value -Werror=unused-label -Werror=address -Werror=format -Werror=sign-compare -Werror=write-strings -Werror=delete-non-virtual-dtor -Werror=strict-aliasing -Werror=narrowing -Werror=unused-but-set-variable -Werror=reorder -Werror=unused-variable -Werror=conversion-null -Werror=return-local-addr -Wnon-virtual-dtor -Werror=switch -fdiagnostics-show-option -Wno-unused-local-typedefs -Wno-attributes -Wno-psabi -DEIGEN_DONT_PARALLELIZE -DEIGEN_MAX_ALIGN_BYTES=64 -Wno-error=unused-variable $(ALPAKA_HOST_MACRO) -DBOOST_DISABLE_ASSERTS -g -DGPU_DEBUG -DONE_KERNEL -std=c++20 -march=x86-64-v2 -fPIC '

NVCC_FLAGS :=--diag-suppress 20014 -std=c++20 -O3 --generate-line-info --source-in-ptx --display-error-number --expt-relaxed-constexpr --extended-lambda -Wno-deprecated-gpu-targets -diag-suppress=3012 -diag-suppress=3189 -Xcudafe --diag_suppress=esa_on_defaulted_function_ignored -Xcudafe --gnu_version=120300 --cudart shared

CXX_FLAGS :=-O3 -pthread -pipe -Werror=main -Werror=pointer-arith -Werror=overlength-strings -Wno-vla -Werror=overflow -std=c++20 -ftree-vectorize -Werror=array-bounds -Werror=format-contains-nul -Werror=type-limits -fvisibility-inlines-hidden -fno-math-errno --param vect-max-version-for-alias-checks=50 -Xassembler --compress-debug-sections -Wno-error=array-bounds -Warray-bounds -fuse-ld=bfd -march=x86-64-v3 -felide-constructors -fmessage-length=0 -Wall -Wno-non-template-friend -Wno-long-long -Wreturn-type -Wextra -Wpessimizing-move -Wclass-memaccess -Wno-cast-function-type -Wno-unused-but-set-parameter -Wno-ignored-qualifiers -Wno-unused-parameter -Wunused -Wparentheses -Werror=return-type -Werror=missing-braces -Werror=unused-value -Werror=unused-label -Werror=address -Werror=format -Werror=sign-compare -Werror=write-strings -Werror=delete-non-virtual-dtor -Werror=strict-aliasing -Werror=narrowing -Werror=unused-but-set-variable -Werror=reorder -Werror=unused-variable -Werror=conversion-null -Werror=return-local-addr -Wnon-virtual-dtor -Werror=switch -fdiagnostics-show-option -Wno-unused-local-typedefs -Wno-attributes -Wno-psabi -DEIGEN_DONT_PARALLELIZE -DEIGEN_MAX_ALIGN_BYTES=64 -Wno-error=unused-variable -DBOOST_DISABLE_ASSERTS -g -flto=auto -fipa-icf -flto-odr-type-merging -fno-fat-lto-objects -Wodr -fPIC

LINK_FLAGS := -Wl,-E -Wl,--hash-style=gnu -Wl,--as-needed -Wl,-z,noexecstack

LIBS := $(LINK_CUDA) -lcudart -lcuda -lcrypt -ldl -lrt -lstdc++fs #-lnvidia-ml

# Sources
GPU_SRC := VertexFinder_t.dev.cc
DLINK_OBJ := VertexFinder_cudadlink.o

# Objects
GPU_OBJ := $(GPU_SRC:.cc=.cc.o)

all: VertexFinder_t

%.dev.cc.o: %.dev.cc
	$(NVCC) -x cu -dc $(BOOST_MACRO) $(GNU_MACRO) $(INCLUDES) $(NVCC_FLAGS) $(NVCC_ARCH) $(ALPAKA_DEVICE_MACRO) $(CUDA_HOST_COMPILER) $< -o $@

$(DLINK_OBJ): $(GPU_OBJ)
	$(NVCC) -dlink $(LINK_STUBS) $(NVCC_FLAGS) $(NVCC_ARCH) $(ALPAKA_DEVICE_MACRO) $(CUDA_HOST_COMPILER) $^ -o $@

VertexFinder_t: $(GPU_OBJ) $(DLINK_OBJ)
	$(CXX) $(CXX_FLAGS) $(LINK_FLAGS) $^ $(LIBS) -o $@

clean:

	rm -f *.o VertexFinder_t

.PHONY: all clean
