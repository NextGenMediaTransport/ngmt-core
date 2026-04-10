#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <vector>

namespace ngmt {

/// De-jitter queue for **unordered / variable-delay** QUIC datagram arrivals (WLAN).
///
/// Holds object payloads keyed by arrival; caller supplies wall/rtp-like timestamps.
/// MoQ alignment: smooths delivery of **object** sequences before decode.
class JitterBuffer {
public:
  explicit JitterBuffer(std::chrono::milliseconds depth);

  struct Item {
    std::uint64_t sequence{};
    std::vector<std::uint8_t> data;
  };

  void push(std::uint64_t sequence, std::vector<std::uint8_t> data);
  /// Returns next playable item if within jitter window (FIFO by sequence).
  std::optional<Item> pop_ready();

  [[nodiscard]] std::size_t size() const { return q_.size(); }

private:
  [[maybe_unused]] std::chrono::milliseconds depth_;
  std::deque<Item> q_;
};

} // namespace ngmt
