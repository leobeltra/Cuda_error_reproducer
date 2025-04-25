#ifndef HeterogeneousCore_AlpakaInterface_interface_Backend_h
#define HeterogeneousCore_AlpakaInterface_interface_Backend_h

#include <algorithm>
#include <array>
#include <string_view>

namespace cms::alpakatools {
  // Enumeration whose value EDModules can put in the event
  enum class Backend : unsigned short { SerialSync = 0, CudaAsync = 1, ROCmAsync = 2, TbbAsync = 3, size };

  Backend toBackend(std::string_view name);
  std::string_view toString(Backend backend);
}  // namespace cms::alpakatools

namespace {
  constexpr const std::array<std::string_view, static_cast<short>(cms::alpakatools::Backend::size)> backendNames = {
      {"SerialSync", "CudaAsync", "ROCmAsync", "TbbAsync"}};
}

namespace cms::alpakatools {
  Backend toBackend(std::string_view name) {
    auto found = std::find(backendNames.begin(), backendNames.end(), name);
    if (found == backendNames.end()) {
      // cms::Exception ex("EnumNotFound");
      // ex << "Invalid backend name '" << name << "'";
      // ex.addContext("Calling cms::alpakatools::toBackend()");
      throw "EnumNotFound";
    }
    return static_cast<Backend>(std::distance(backendNames.begin(), found));
  }

  std::string_view toString(Backend backend) {
    auto val = static_cast<unsigned short>(backend);
    if (val >= static_cast<unsigned short>(Backend::size)) {
      // cms::Exception ex("InvalidEnumValue");
      // ex << "Invalid backend enum value " << val;
      // ex.addContext("Calling cms::alpakatools::toString()");
      throw "InvalidEnumValue";
    }
    return backendNames[val];
  }
}  // namespace cms::alpakatools

#endif
