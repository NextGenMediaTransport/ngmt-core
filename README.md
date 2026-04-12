# ngmt-core

<p align="center"><img src="branding/svg/marks/ngmt-core.svg" width="96" height="96" alt="NGMT Core mark"/></p>

**ngmt-core** is the NextGenMediaTransport (NGMT) **C++** library for Open Media Transport (OMT)–style core protocol work. The primary build is **CMake** with **C++17**.

The upstream **.NET reference implementation** (formerly `libomtnet`) lives under [`legacy-reference-csharp/`](legacy-reference-csharp/) as a **blueprint** only; it is not built by the default CMake flow.

Protocol details: [`PROTOCOL.md`](PROTOCOL.md). NGMT wire layout: [`../docs/protocol/ngmt-wire-format.md`](../docs/protocol/ngmt-wire-format.md) (when using the meta-repo checkout).

Phase 3 additions: `include/ngmt/protocol/` (MoQ-aligned objects + packetizer), `include/ngmt/jitter_buffer.hpp`, `include/ngmt/discovery/advertiser.hpp`.

## Prerequisites

- **CMake** 3.15 or newer
- A **C++17** compiler (Clang, GCC, or MSVC)

Optional: **.NET SDK** if you want to compile the legacy C# reference (see below).

## Build (C++ library)

From the repository root:

```sh
cmake -B build -S .
cmake --build build --config Release
```

On Linux and macOS with single-configuration generators, use a Release configure step:

```sh
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Static vs shared library

CMake follows the standard [`BUILD_SHARED_LIBS`](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html) option. Example for a **shared** library:

```sh
cmake -B build -S . -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Legacy C# reference (blueprint)

The original OMT **.NET** sources, solution, and project files were moved to **`legacy-reference-csharp/`** for comparison and porting. They are **not** part of the C++ CMake target.

To build that reference implementation (requires a [.NET SDK](https://dotnet.microsoft.com/download)):

```sh
cd legacy-reference-csharp
dotnet build
```

Or build the solution in Release:

```sh
cd legacy-reference-csharp
dotnet build libomtnet.sln -c Release
```

CI for this repository builds the **C++** CMake project only; validating the legacy project is optional and done locally (or in a future workflow) using the commands above.

## Optional: `ngmt-transport` C ABI smoke

With a **sibling checkout** of `ngmt-transport` and `cargo` on your `PATH`, you can build a small C++ tool that links the Rust **cdylib** and exercises [`ngmt_transport.h`](https://github.com/NextGenMediaTransport/ngmt-transport/blob/main/include/ngmt_transport.h) (ABI version + header LE helpers):

```sh
cmake -B build-ffi -S . -DCMAKE_BUILD_TYPE=Release -DNGMT_ENABLE_TRANSPORT_FFI_SMOKE=ON
cmake --build build-ffi --config Release
./build-ffi/ngmt_transport_ffi_smoke
```

**Discovery:** LAN mDNS in this crate vs Rust Studio is documented in [`docs/discovery-status.md`](docs/discovery-status.md).

## Formatting

C++ formatting uses the checked-in [`.clang-format`](.clang-format) (LLVM-based). Example:

```sh
clang-format -i include/ngmt_core.hpp src/ngmt_core.cpp
```

## License

See [`LICENSE.txt`](LICENSE.txt).
