#pragma once

#include "ngmt/protocol/object.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace ngmt::protocol {

/// OMT 1.0 frame header (16 bytes) + extended headers per `PROTOCOL.md`.
struct OmtFrameHeader {
  std::uint8_t version{1};
  std::uint8_t frame_type{0};
  std::int64_t timestamp{0};
  std::uint16_t metadata_length{0};
  std::int32_t data_length{0};
};

constexpr std::size_t kOmtHeaderSize = 16;
constexpr std::size_t kOmtVideoExtSize = 32;
constexpr std::size_t kOmtAudioExtSize = 24;

/// Encodes OMT frame layout (ported from legacy `OMTFrame` / `OMTBinary`).
[[nodiscard]] std::vector<std::uint8_t> encode_omt_frame_header_le(const OmtFrameHeader& h);

/// Fragments a payload into NGMT object records with `NgmtObjectHeader` prefix per chunk.
/// Each output element is: **32-byte LE header || fragment bytes**.
[[nodiscard]] std::vector<std::vector<std::uint8_t>> fragment_payload(
    std::uint32_t track_id,
    std::uint64_t group_id,
    std::uint64_t object_id,
    const std::uint8_t* payload,
    std::size_t payload_len,
    std::size_t max_fragment_body);

} // namespace ngmt::protocol
