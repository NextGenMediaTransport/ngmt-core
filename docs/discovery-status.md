# Discovery status (`ngmt-core` vs `ngmt-studio`)

## Summary

| Layer | `_ngmt._udp` browse / advertise | Notes |
|-------|-----------------------------------|--------|
| **`ngmt-studio`** (Rust) | **Yes** — [`mdns-sd`](https://crates.io/crates/mdns-sd) in [`ngmt-studio-common`](https://github.com/NextGenMediaTransport/ngmt-studio) | Reference implementation for Phase 3/4 LAN tooling; sources show up without manual IP when multicast DNS-SD works on the LAN. |
| **`ngmt-core`** (C++) | **Not yet** — [`Advertiser`](../include/ngmt/discovery/advertiser.hpp) / [`mdns_impl.cpp`](../src/discovery/mdns_impl.cpp) | Facade + **manual** `add_manual_source` / unicast fallback; real mDNS send/receive is **not** wired. Comments in `mdns_impl.cpp` mark the integration point. |

## Implications

- **Generator and Monitor** validate discovery and QUIC together on the LAN using the **Rust** stack.
- **C++-only** builds should assume **manual** endpoints until the `Advertiser` backend is implemented or delegates to a shared library.
- Service type and protocol text match [`PROTOCOL.md`](../PROTOCOL.md) and, in the meta workspace checkout, [`ngmt-wire-format.md`](../../docs/protocol/ngmt-wire-format.md) (`_ngmt._udp`).

## Related

- Meta plan: [Phase 3 — Discovery and WAN](../../docs/project-plan/03-Phase-3-Core-Features-Discovery-and-WAN.md) (relative to meta workspace layout)
