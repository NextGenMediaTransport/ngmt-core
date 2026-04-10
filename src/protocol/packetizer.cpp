#include "ngmt/protocol/packetizer.hpp"

#include <algorithm>
#include <cstring>

namespace ngmt::protocol {

namespace {

void write_i32_le(std::uint8_t* p, std::int32_t v) {
  auto u = static_cast<std::uint32_t>(v);
  for (int i = 0; i < 4; ++i) {
    p[i] = static_cast<std::uint8_t>((u >> (8 * i)) & 0xff);
  }
}

void write_i64_le(std::uint8_t* p, std::int64_t v) {
  auto u = static_cast<std::uint64_t>(v);
  for (int i = 0; i < 8; ++i) {
    p[i] = static_cast<std::uint8_t>((u >> (8 * i)) & 0xff);
  }
}

void write_u16_le(std::uint8_t* p, std::uint16_t v) {
  p[0] = static_cast<std::uint8_t>(v & 0xff);
  p[1] = static_cast<std::uint8_t>((v >> 8) & 0xff);
}

} // namespace

std::vector<std::uint8_t> encode_omt_frame_header_le(const OmtFrameHeader& h) {
  std::vector<std::uint8_t> out(kOmtHeaderSize);
  out[0] = h.version;
  out[1] = h.frame_type;
  write_i64_le(out.data() + 2, h.timestamp);
  write_u16_le(out.data() + 10, h.metadata_length);
  write_i32_le(out.data() + 12, h.data_length);
  return out;
}

std::vector<std::vector<std::uint8_t>> fragment_payload(std::uint32_t track_id, std::uint64_t group_id,
                                                        std::uint64_t object_id, const std::uint8_t* payload,
                                                        std::size_t payload_len, std::size_t max_fragment_body) {
  if (max_fragment_body == 0) {
    max_fragment_body = 1200;
  }
  std::vector<std::vector<std::uint8_t>> chunks;
  if (payload_len == 0) {
    NgmtObjectHeader hdr{};
    hdr.track_id = track_id;
    hdr.group_id = group_id;
    hdr.object_id = object_id;
    hdr.fragment_index = 0;
    hdr.fragment_total = 1;
    hdr.payload_length = 0;
    std::array<std::uint8_t, NgmtObjectHeader::kWireSize> wire{};
    hdr.write_le(wire);
    std::vector<std::uint8_t> one(wire.begin(), wire.end());
    chunks.push_back(std::move(one));
    return chunks;
  }

  const std::size_t n = (payload_len + max_fragment_body - 1) / max_fragment_body;
  chunks.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    const std::size_t off = i * max_fragment_body;
    const std::size_t len = std::min(max_fragment_body, payload_len - off);
    NgmtObjectHeader hdr{};
    hdr.track_id = track_id;
    hdr.group_id = group_id;
    hdr.object_id = object_id;
    hdr.fragment_index = static_cast<std::uint16_t>(i);
    hdr.fragment_total = static_cast<std::uint16_t>(n);
    hdr.payload_length = static_cast<std::uint32_t>(len);
    std::array<std::uint8_t, NgmtObjectHeader::kWireSize> wire{};
    hdr.write_le(wire);
    std::vector<std::uint8_t> buf;
    buf.reserve(NgmtObjectHeader::kWireSize + len);
    buf.insert(buf.end(), wire.begin(), wire.end());
    buf.insert(buf.end(), payload + off, payload + off + len);
    chunks.push_back(std::move(buf));
  }
  return chunks;
}

} // namespace ngmt::protocol
