#pragma once

#include <string>

namespace ngmt::core {

/// Returns the library version string (semantic versioning: MAJOR.MINOR.PATCH).
[[nodiscard]] std::string version_string();

} // namespace ngmt::core
