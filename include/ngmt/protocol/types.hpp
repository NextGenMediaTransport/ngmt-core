#pragma once

#include <cstdint>

namespace ngmt::protocol {

/// OMT-compatible frame kinds (legacy `OMTFrameType`).
enum class FrameKind : std::uint8_t {
  Metadata = 1,
  Video = 2,
  Audio = 4,
};

/// Logical MoQ-style track for routing (maps to QUIC stream/datagram policy).
enum class TrackId : std::uint32_t {
  Control = 0,
  Video = 1,
  Audio = 2,
};

} // namespace ngmt::protocol
