#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace ngmt::protocol {

/// Wire header for one NGMT object or fragment — **32 bytes**, all multi-byte fields **little-endian**
/// on the wire (safe across M2 ARM64 and x86_64).
///
/// MoQ alignment: **track** ≈ `track_id`; **group** ≈ `group_id`; **object** ≈ `object_id` + payload;
/// fragmentation uses `fragment_index` / `fragment_total`.
struct NgmtObjectHeader {
  std::uint8_t version{1};
  std::uint8_t flags{0};
  std::uint16_t reserved{0};
  std::uint32_t track_id{0};
  std::uint64_t group_id{0};
  std::uint64_t object_id{0};
  std::uint16_t fragment_index{0};
  std::uint16_t fragment_total{1};
  std::uint32_t payload_length{0};

  static constexpr std::size_t kWireSize = 32;

  /// Serialize to 32-byte little-endian buffer (explicit; do not rely on `reinterpret_cast`).
  void write_le(std::array<std::uint8_t, kWireSize>& out) const;

  /// Parse from 32-byte little-endian buffer.
  [[nodiscard]] static bool read_le(const std::uint8_t* data, std::size_t len, NgmtObjectHeader& out);
};

static_assert(sizeof(NgmtObjectHeader) == 32, "NgmtObjectHeader must remain 32 bytes for FFI");

} // namespace ngmt::protocol
