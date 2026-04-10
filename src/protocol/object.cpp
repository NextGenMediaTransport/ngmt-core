#include "ngmt/protocol/object.hpp"

namespace ngmt::protocol {

namespace {

void write_u16_le(std::uint8_t* p, std::uint16_t v) {
  p[0] = static_cast<std::uint8_t>(v & 0xff);
  p[1] = static_cast<std::uint8_t>((v >> 8) & 0xff);
}

void write_u32_le(std::uint8_t* p, std::uint32_t v) {
  for (int i = 0; i < 4; ++i) {
    p[i] = static_cast<std::uint8_t>((v >> (8 * i)) & 0xff);
  }
}

void write_u64_le(std::uint8_t* p, std::uint64_t v) {
  for (int i = 0; i < 8; ++i) {
    p[i] = static_cast<std::uint8_t>((v >> (8 * i)) & 0xff);
  }
}

std::uint16_t read_u16_le(const std::uint8_t* p) {
  return static_cast<std::uint16_t>(p[0]) | (static_cast<std::uint16_t>(p[1]) << 8);
}

std::uint32_t read_u32_le(const std::uint8_t* p) {
  return static_cast<std::uint32_t>(p[0]) | (static_cast<std::uint32_t>(p[1]) << 8) |
         (static_cast<std::uint32_t>(p[2]) << 16) | (static_cast<std::uint32_t>(p[3]) << 24);
}

std::uint64_t read_u64_le(const std::uint8_t* p) {
  std::uint64_t v = 0;
  for (int i = 0; i < 8; ++i) {
    v |= static_cast<std::uint64_t>(p[i]) << (8 * i);
  }
  return v;
}

} // namespace

void NgmtObjectHeader::write_le(std::array<std::uint8_t, kWireSize>& out) const {
  out[0] = version;
  out[1] = flags;
  write_u16_le(out.data() + 2, reserved);
  write_u32_le(out.data() + 4, track_id);
  write_u64_le(out.data() + 8, group_id);
  write_u64_le(out.data() + 16, object_id);
  write_u16_le(out.data() + 24, fragment_index);
  write_u16_le(out.data() + 26, fragment_total);
  write_u32_le(out.data() + 28, payload_length);
}

bool NgmtObjectHeader::read_le(const std::uint8_t* data, std::size_t len, NgmtObjectHeader& out) {
  if (len < kWireSize || data == nullptr) {
    return false;
  }
  out.version = data[0];
  out.flags = data[1];
  out.reserved = read_u16_le(data + 2);
  out.track_id = read_u32_le(data + 4);
  out.group_id = read_u64_le(data + 8);
  out.object_id = read_u64_le(data + 16);
  out.fragment_index = read_u16_le(data + 24);
  out.fragment_total = read_u16_le(data + 26);
  out.payload_length = read_u32_le(data + 28);
  return true;
}

} // namespace ngmt::protocol
