# AGENTS.md - OurChat Development Guide

This document provides guidelines and commands for agents working on the OurChat codebase.

## Project Structure

```
OurChat/
├── src/
│   ├── server/           # C++ gRPC server (CMake)
│   ├── client/flutter/   # Flutter client
│   ├── proto/            # Protocol Buffer definitions
│   ├── data/             # Data access layer
│   ├── services/         # gRPC service implementations
│   ├── network/          # Network layer
│   └── common/           # Common utilities
├── CMakeLists.txt        # C++ build configuration
├── Dockerfile            # Docker container
└── tests/                # C++ unit tests
```

## Build Commands

### C++ Server

```bash
# Debug build
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Build without tests
cmake -DBUILD_TESTS=OFF ..

# Full rebuild
rm -rf build && mkdir build && cd build
cmake .. && make -j$(nproc)
```

### Flutter Client

```bash
cd# Install dependencies
flutter pub get

# Run src/client/flutter

 web version
flutter run -d chrome --web-hostname localhost --web-port 54321

# Build web release
flutter build web --release

# Build for specific platform
flutter build apk          # Android
flutter build ipa          # iOS
flutter build macos        # macOS
flutter build windows      # Windows
flutter build linux        # Linux
```

## Lint Commands

### C++ Server

```bash
# clang-tidy (if configured)
clang-tidy src/**/*.cpp --

# Format code
clang-format -i src/**/*.cpp include/**/*.h

# Protobuf lint
protoc --proto_path=src/proto --cpp_out=src/proto --lint-check src/proto/*.proto
```

### Flutter Client

```bash
cd src/client/flutter

# Analyze code
flutter analyze

# Fix analysis issues
flutter analyze --fix

# Format code
flutter format lib/

# Run lints
flutter lint lib/

# Code generation (Riverpod, Drift)
flutter pub run build_runner build
flutter pub run build_runner watch  # Watch mode
```

## Test Commands

### C++ Server

```bash
cd build

# Run all tests
ctest

# Run specific test
ctest -R <test_name>

# Run with verbose output
ctest -V

# Run unit tests only
./tests/unit_tests
```

### Flutter Client

```bash
cd src/client/flutter

# Run all tests
flutter test

# Run specific test file
flutter test test/unit/auth_manager_test.dart

# Run tests with coverage
flutter test --coverage
genhtml coverage/lcov.info -o coverage/html
open coverage/html/index.html

# Run widget tests
flutter test test/widget/

# Run integration tests (requires device/emulator)
flutter test integration_test/

# Run tests in release mode
flutter test --release
```

## Code Style Guidelines

### C++ (Server)

- **Standard**: C++17
- **Formatting**: clang-format (Google style)
- **Naming**:
  - Classes: `PascalCase` (e.g., `AuthService`)
  - Functions: `snake_case` (e.g., `handle_request`)
  - Variables: `snake_case` (e.g., `user_id`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_CONNECTIONS`)
  - Private members: trailing underscore (e.g., `user_id_`)
- **Error Handling**: Use `Result<T, Error>` pattern or exceptions for unrecoverable errors
- **Imports**: Use `#include <>` for system libs, `""` for local files
- **Memory**: Prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`)

### Dart/Flutter (Client)

- **Standard**: Dart 3.0+, Flutter 3.10+
- **Formatting**: `dart format` (Flutter default style)
- **Naming**:
  - Classes: `PascalCase` (e.g., `AuthManager`)
  - Functions/variables: `camelCase` (e.g., `userId`)
  - Constants: `kCamelCase` (e.g., `kDefaultTimeout`)
  - Private members: leading underscore (e.g., `_token`)
- **Error Handling**: Use `try-catch` with async/await, `Result` type for sync operations
- **Imports**: Use `package:` for internal, `dart:` for Dart stdlib
- **State Management**: Use Riverpod providers and `ConsumerWidget`
- **Null Safety**: Always use nullable types (`Type?`) explicitly

### Protocol Buffers

- **Naming**: `snake_case` for fields, `PascalCase` for messages/services
- **Imports**: Use relative paths within `src/proto/`
- **Versioning**: Increment `syntax = "proto3";` version as needed

## Error Handling

### C++
```cpp
// Use Result pattern for recoverable errors
Result<User> AuthService::login(const LoginRequest& req) {
    if (req.username().empty()) {
        return Result<User>::error("Username required");
    }
    // ... business logic
    return Result<User>::ok(user);
}
```

### Dart
```dart
// Use try-catch with async/await
Future<LoginResponse> login(String username, String password) async {
    try {
        final response = await _authClient.login(request);
        return response;
    } catch (e) {
        return LoginResponse()..success = false;
    }
}
```

## Key Dependencies

- **C++**: gRPC 1.50+, Protobuf 3.x, yaml-cpp, MySQL, Redis, Kafka
- **Flutter**: Riverpod, Go Router, gRPC-Dart, Drift, web_socket_channel

## Common Tasks

### Rebuild proto files
```bash
# C++ (from project root)
protoc --cpp_out=src/proto --grpc_out=src/proto -I src/proto src/proto/*.proto

# Flutter (from src/client/flutter)
protoc --dart_out=lib/core/network/generated -I ../proto ../proto/*.proto
```

### Docker
```bash
# Build image
docker build -t ourchat:latest .

# Run container
docker run -p 50001:50001 ourchat:latest
```
