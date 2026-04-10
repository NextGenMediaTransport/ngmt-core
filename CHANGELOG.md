# Changelog

All notable changes to this repository will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html)
where versioning applies.

## 2026-04-10 — Phase 3: Protocol objects, jitter, discovery stub, wire docs

### Added

- MoQ-aligned **`NgmtObjectHeader`** and helpers (`include/ngmt/protocol/object.hpp`, `src/protocol/object.cpp`).
- **Packetizer** for OMT-style frame header encoding and payload fragmentation (`include/ngmt/protocol/packetizer.hpp`, `src/protocol/packetizer.cpp`).
- **`JitterBuffer`** for variable datagram arrival (`include/ngmt/jitter_buffer.hpp`, `src/jitter_buffer.cpp`).
- **Discovery** `Advertiser` facade with manual unicast fallback (`include/ngmt/discovery/advertiser.hpp`, `src/discovery/mdns_impl.cpp`); full mDNS integration pending library choice.
- **`PROTOCOL.md`** appendix linking NGMT over QUIC and `docs/protocol/ngmt-wire-format.md`.

## 2026-04-10 — Phase 2: Build standardization and infrastructure

### Added

- Initial publish to the **NextGenMediaTransport** organization (`main`); foundational Phase 2 infrastructure commit.
- Root **CMake** build for the `ngmt-core` C++17 library (`CMakeLists.txt`), with `include/ngmt_core.hpp` and `src/ngmt_core.cpp` as a minimal API surface.
- [`.clang-format`](.clang-format) (LLVM-based defaults) for C++.
- [`.github/workflows/ci.yml`](.github/workflows/ci.yml) building the CMake project on Ubuntu, Windows, and macOS.

### Changed

- **Quarantined** the upstream .NET (`libomtnet`) tree under [`legacy-reference-csharp/`](legacy-reference-csharp/) (sources, `libomtnet.sln`, `libomtnet.csproj`, and `build/` scripts). The C++ tree is now the primary layout at the repo root.
- [`LICENSE.txt`](LICENSE.txt): added `Copyright (c) 2026 NextGenMediaTransport Contributors` alongside the OMT copyright line.
