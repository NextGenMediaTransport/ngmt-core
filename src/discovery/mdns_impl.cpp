#include "ngmt/discovery/advertiser.hpp"

#include <mutex>
#include <vector>

namespace ngmt::discovery {

namespace {

constexpr const char* kServiceType = "_ngmt._udp.local";

} // namespace

struct Advertiser::Impl {
  std::mutex mu;
  std::vector<SourceRecord> manual;
  OnSources browse_cb;
  bool advertising{false};
  std::uint16_t ad_port{0};
  std::string ad_name;
  std::string ad_txt;
};

Advertiser::Advertiser() : impl_(new Impl{}) {}

Advertiser::~Advertiser() {
  stop_advertise();
  stop_browse();
  delete impl_;
}

bool Advertiser::start_advertise(std::uint16_t port, const std::string& name, const std::string& txt_extra) {
  std::lock_guard<std::mutex> lock(impl_->mu);
  impl_->advertising = true;
  impl_->ad_port = port;
  impl_->ad_name = name;
  impl_->ad_txt = txt_extra;
  // Phase 3: integrate micro-dns / Servus / raw multicast DNS-SD here.
  (void)kServiceType;
  return true;
}

void Advertiser::stop_advertise() {
  std::lock_guard<std::mutex> lock(impl_->mu);
  impl_->advertising = false;
}

bool Advertiser::start_browse(OnSources callback) {
  std::lock_guard<std::mutex> lock(impl_->mu);
  impl_->browse_cb = std::move(callback);
  if (impl_->browse_cb) {
    impl_->browse_cb(impl_->manual);
  }
  return true;
}

void Advertiser::stop_browse() {
  std::lock_guard<std::mutex> lock(impl_->mu);
  impl_->browse_cb = nullptr;
}

void Advertiser::add_manual_source(SourceRecord s) {
  std::lock_guard<std::mutex> lock(impl_->mu);
  impl_->manual.push_back(std::move(s));
  if (impl_->browse_cb) {
    impl_->browse_cb(impl_->manual);
  }
}

void Advertiser::clear_manual_sources() {
  std::lock_guard<std::mutex> lock(impl_->mu);
  impl_->manual.clear();
  if (impl_->browse_cb) {
    impl_->browse_cb(impl_->manual);
  }
}

} // namespace ngmt::discovery
