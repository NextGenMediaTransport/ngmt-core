#pragma once

#include <functional>
#include <string>
#include <vector>

namespace ngmt::discovery {

struct SourceRecord {
  std::string instance_name;
  std::string host;
  std::uint16_t port{0};
  std::string txt;
};

/// mDNS advertisement / browse facade (`_ngmt._udp` service type).
///
/// Implementation may use embedded mDNS (see `mdns_impl.cpp`) or system DNS-SD.
/// **Unicast fallback:** `add_manual_source` for APs that filter multicast.
class Advertiser {
public:
  using OnSources = std::function<void(const std::vector<SourceRecord>&)>;

  Advertiser();
  ~Advertiser();

  Advertiser(const Advertiser&) = delete;
  Advertiser& operator=(const Advertiser&) = delete;

  /// Begin advertising `port` as `_ngmt._udp` with optional TXT.
  bool start_advertise(std::uint16_t port, const std::string& name, const std::string& txt_extra);

  void stop_advertise();

  /// Begin browsing for sources (stub may no-op until full mDNS parse lands).
  bool start_browse(OnSources callback);

  void stop_browse();

  /// Manual override when mDNS is blocked on Wi-Fi.
  void add_manual_source(SourceRecord s);

  void clear_manual_sources();

private:
  struct Impl;
  Impl* impl_{nullptr};
};

} // namespace ngmt::discovery
