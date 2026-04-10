// SPDX-License-Identifier: MIT
// Optional smoke: link against ngmt-transport cdylib and exercise the C ABI (no init required).

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <ngmt_transport.h>

int main() {
  if (ngmt_transport_abi_version() != 1u) {
    std::fprintf(stderr, "ngmt_transport_ffi_smoke: unexpected ABI version\n");
    return 1;
  }

  NgmtObjectHeader h{};
  std::memset(&h, 0, sizeof(h));
  h.version = 1;
  h.track_id = 42;
  h.payload_length = 0;

  uint8_t buf[32];
  ngmt_object_header_write_le(&h, buf);

  NgmtObjectHeader out{};
  if (!ngmt_object_header_read_le(buf, &out)) {
    std::fprintf(stderr, "ngmt_transport_ffi_smoke: header read failed\n");
    return 2;
  }
  if (out.track_id != 42u) {
    std::fprintf(stderr, "ngmt_transport_ffi_smoke: roundtrip mismatch\n");
    return 3;
  }

  std::printf("ngmt_transport_ffi_smoke ok (ABI=%u)\n",
              static_cast<unsigned>(ngmt_transport_abi_version()));
  return 0;
}
