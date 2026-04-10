#include "ngmt/jitter_buffer.hpp"

#include <algorithm>

namespace ngmt {

JitterBuffer::JitterBuffer(std::chrono::milliseconds depth) : depth_(depth) {}

void JitterBuffer::push(std::uint64_t sequence, std::vector<std::uint8_t> data) {
  q_.push_back(Item{sequence, std::move(data)});
  std::sort(q_.begin(), q_.end(), [](const Item& a, const Item& b) { return a.sequence < b.sequence; });
}

std::optional<JitterBuffer::Item> JitterBuffer::pop_ready() {
  if (q_.empty()) {
    return std::nullopt;
  }
  // Minimal policy: deliver lowest sequence (full reordering is codec-specific).
  Item x = std::move(q_.front());
  q_.pop_front();
  return x;
}

} // namespace ngmt
