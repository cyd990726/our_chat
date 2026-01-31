# OurChat Flutter客户端与服务端联调指南

## 一、概述

本文档指导如何将Flutter客户端与OurChat后端服务器进行联调对接。

## 二、前置条件

### 2.1 确保服务端已启动

```bash
cd /Users/bytedance/Desktop/OurChat
docker-compose up -d

# 或直接运行
./bin/ourchat_server config/server.yaml
```

**服务端口说明：**

| 服务 | 端口 | 说明 |
|------|------|------|
| Auth Service | 50001 | 用户认证 |
| Session Service | 50002 | 会话管理 |
| Gateway Service | 50003 | WebSocket网关 |
| Message Service | 50004 | 消息服务 |
| Group Service | 50005 | 群组服务 |
| Presence Service | 50006 | 在线状态 |

### 2.2 proto文件准备

从服务端复制proto文件到客户端：

```bash
mkdir -p /Users/bytedance/Desktop/OurChat/src/client/flutter/proto
cp /Users/bytedance/Desktop/OurChat/src/proto/*.proto /Users/bytedance/Desktop/OurChat/src/client/flutter/proto/
```

## 三、客户端项目结构

```
src/client/flutter/
├── proto/                           # proto文件目录
│   ├── common.proto
│   ├── user.proto
│   ├── message.proto
│   ├── group.proto
│   ├── session.proto
│   └── presence.proto
├── lib/
│   ├── core/
│   │   ├── network/                 # gRPC客户端
│   │   │   ├── grpc_client.dart
│   │   │   ├── grpc_channel.dart
│   │   │   └── generated/           # 编译生成的代码
│   │   │       ├── user.pb.dart
│   │   │       ├── user.pbgrpc.dart
│   │   │       └── ...
│   │   └── config/
│   │       └── app_config.dart
│   ├── services/                    # 服务层
│   │   ├── auth_service.dart
│   │   ├── message_service.dart
│   │   ├── group_service.dart
│   │   └── presence_service.dart
│   └── ui/
│       └── screens/auth/
│           └── login_screen.dart
└── pubspec.yaml
```

## 四、具体实现步骤

### 4.1 安装依赖

```bash
cd /Users/bytedance/Desktop/OurChat/src/client/flutter
flutter pub add grpc protobuf
```

### 4.2 配置服务端地址

**文件**: `lib/core/config/app_config.dart`

```dart
class AppConfig {
  // 服务端地址 - 开发环境
  static const String grpcHost = 'localhost';
  static const int grpcPort = 50001;  // Auth Service端口
  
  // WebSocket地址
  static const String wsUrl = 'ws://localhost:50003';
  
  // 生产环境配置（部署时使用）
  // static const String grpcHost = 'your-server-ip';
  // static const int grpcPort = 50001;
}
```

### 4.3 创建gRPC客户端

**文件**: `lib/core/network/grpc_client.dart`

```dart
import 'package:grpc/grpc.dart';
import 'package:ourchat_flutter/core/config/app_config.dart';
import 'generated/user.pbgrpc.dart';

class GrpcClient {
  static GrpcClient? _instance;
  late ClientChannel _channel;
  late AuthServiceClient _authClient;

  GrpcClient._internal() {
    _channel = ClientChannel(
      AppConfig.grpcHost,
      port: AppConfig.grpcPort,
      options: const ChannelOptions(
        credentials: ChannelCredentials.insecure(),
        idleTimeout: Duration(minutes: 5),
        connectionTimeout: Duration(seconds: 10),
      ),
    );
    _authClient = AuthServiceClient(_channel);
  }

  factory GrpcClient.getInstance() {
    _instance ??= GrpcClient._internal();
    return _instance!;
  }

  AuthServiceClient get authClient => _authClient;

  Future<void> shutdown() async {
    await _channel.shutdown();
  }
}
```

### 4.4 创建认证服务

**文件**: `lib/services/auth_service.dart`

```dart
import 'dart:async';
import 'package:ourchat_flutter/core/network/grpc_client.dart';
import 'generated/user.pbgrpc.dart';

class AuthService {
  final _grpcClient = GrpcClient.getInstance();

  Future<LoginResponse> login(String username, String password) async {
    final request = LoginRequest(
      username: username,
      password: password,
      deviceId: 'web-${DateTime.now().millisecondsSinceEpoch}',
      deviceType: 'web',
      clientVersion: '1.0.0',
    );

    try {
      return await _grpcClient.authClient.login(request);
    } catch (e) {
      return LoginResponse()
        ..success = false
        ..error = (Error()..message = 'Network error: $e');
    }
  }

  Future<RegisterResponse> register(RegisterRequest request) async {
    try {
      return await _grpcClient.authClient.register(request);
    } catch (e) {
      return RegisterResponse()
        ..success = false
        ..error = (Error()..message = 'Network error: $e');
    }
  }

  Future<LogoutResponse> logout(int userId, String deviceId) async {
    try {
      return await _grpcClient.authClient.logout(
        LogoutRequest()..userId = userId..deviceId = deviceId,
      );
    } catch (e) {
      return LogoutResponse()
        ..success = false
        ..error = (Error()..message = 'Network error: $e');
    }
  }
}

final authServiceProvider = AuthService();
```

### 4.5 Token管理

**文件**: `lib/core/auth/auth_manager.dart`

```dart
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:flutter_secure_storage/flutter_secure_storage.dart';

class AuthManager {
  static const String _tokenKey = 'access_token';
  static const String _refreshTokenKey = 'refresh_token';
  static const String _userIdKey = 'user_id';

  final FlutterSecureStorage _storage = const FlutterSecureStorage();

  Future<String?> getAccessToken() async {
    return await _storage.read(key: _tokenKey);
  }

  Future<String?> getRefreshToken() async {
    return await _storage.read(key: _refreshTokenKey);
  }

  Future<String?> getUserId() async {
    return await _storage.read(key: _userIdKey);
  }

  Future<void> saveTokens(String accessToken, String refreshToken) async {
    await _storage.write(key: _tokenKey, value: accessToken);
    await _storage.write(key: _refreshTokenKey, value: refreshToken);
  }

  Future<void> saveUserId(String userId) async {
    await _storage.write(key: _userIdKey, value: userId);
  }

  Future<void> clearAuthData() async {
    await _storage.deleteAll();
  }
}

final authManagerProvider = Provider<AuthManager>((ref) {
  return AuthManager();
});
```

### 4.6 修改登录页面联调

**文件**: `lib/ui/screens/auth/login_screen.dart`

```dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:ourchat_flutter/core/auth/auth_manager.dart';
import 'package:ourchat_flutter/services/auth_service.dart';
import 'package:ourchat_flutter/ui/navigation/route_names.dart';

class LoginScreen extends ConsumerStatefulWidget {
  const LoginScreen({super.key});

  @override
  ConsumerState<LoginScreen> createState() => _LoginScreenState();
}

class _LoginScreenState extends ConsumerState<LoginScreen> {
  final usernameController = TextEditingController();
  final passwordController = TextEditingController();
  bool isLoading = false;

  @override
  Widget build(BuildContext context) {
    final authService = ref.read(authServiceProvider);
    final authManager = ref.read(authManagerProvider);

    return Scaffold(
      appBar: AppBar(title: const Text('Login')),
      body: Padding(
        padding: const EdgeInsets.all(24.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Icon(Icons.chat_bubble_outline, size: 60),
            const SizedBox(height: 20),
            const Text(
              'Welcome Back',
              style: TextStyle(fontSize: 24, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 30),
            TextField(
              controller: usernameController,
              decoration: const InputDecoration(
                labelText: 'Username',
                prefixIcon: Icon(Icons.person),
                border: OutlineInputBorder(),
              ),
            ),
            const SizedBox(height: 16),
            TextField(
              controller: passwordController,
              obscureText: true,
              decoration: const InputDecoration(
                labelText: 'Password',
                prefixIcon: Icon(Icons.lock),
                border: OutlineInputBorder(),
              ),
            ),
            const SizedBox(height: 24),
            SizedBox(
              width: double.infinity,
              height: 50,
              child: ElevatedButton(
                onPressed: isLoading
                    ? null
                    : () async {
                        setState(() => isLoading = true);
                        
                        try {
                          final response = await authService.login(
                            usernameController.text.trim(),
                            passwordController.text,
                          );

                          if (response.success) {
                            await authManager.saveTokens(
                              response.token,
                              '',
                            );
                            await authManager.saveUserId(
                              response.userId.toString(),
                            );
                            
                            if (mounted) {
                              context.go(RouteNames.chatList);
                            }
                          } else {
                            if (mounted) {
                              ScaffoldMessenger.of(context).showSnackBar(
                                SnackBar(
                                  content: Text('Login failed: ${response.error.message}'),
                                  backgroundColor: Colors.red,
                                ),
                              );
                            }
                          }
                        } catch (e) {
                          if (mounted) {
                            ScaffoldMessenger.of(context).showSnackBar(
                              SnackBar(
                                content: Text('Error: $e'),
                                backgroundColor: Colors.red,
                              ),
                            );
                          }
                        } finally {
                          if (mounted) {
                            setState(() => isLoading = false);
                          }
                        }
                      },
                child: isLoading
                    ? const SizedBox(
                        width: 20,
                        height: 20,
                        child: CircularProgressIndicator(strokeWidth: 2),
                      )
                    : const Text('Login', style: TextStyle(fontSize: 18)),
              ),
            ),
            const SizedBox(height: 16),
            TextButton(
              onPressed: () => context.go(RouteNames.register),
              child: const Text("Don't have an account? Register"),
            ),
          ],
        ),
      ),
    );
  }
}
```

## 五、proto文件编译

### 5.1 安装工具

```bash
# macOS
brew install protobuf

# 安装Dart protoc插件
dart pub global activate protoc_plugin
```

### 5.2 编译命令

```bash
cd /Users/bytedance/Desktop/OurChat/src/client/flutter

# 编译proto文件生成Dart代码
protoc --dart_out=lib/core/network/generated \
  --grpc_out=lib/core/network/generated \
  -I proto \
  proto/*.proto
```

### 5.3 验证编译结果

编译成功后会在 `lib/core/network/generated/` 目录下生成：
- `user.pb.dart` - Protocol Buffer消息类
- `user.pbgrpc.dart` - gRPC服务客户端类

## 六、联调测试

### 6.1 启动服务端

```bash
cd /Users/bytedance/Desktop/OurChat
docker-compose up -d
```

检查服务状态：
```bash
docker-compose ps
```

### 6.2 启动客户端

```bash
cd /Users/bytedance/Desktop/OurChat/src/client/flutter
flutter run -d chrome --web-hostname localhost --web-port 54321
```

### 6.3 测试场景

| 测试项 | 操作步骤 | 预期结果 |
|--------|----------|----------|
| 登录成功 | 输入正确的用户名密码 | 跳转到聊天列表 |
| 登录失败 | 输入错误的密码 | 显示错误提示 |
| 用户注册 | 填写注册信息提交 | 注册成功跳转登录 |
| 登出 | 点击登出按钮 | 清除Token跳转登录 |

## 七、Web平台特殊配置

由于浏览器安全限制，gRPC-Web需要通过Envoy代理。

### 7.1 Envoy配置

**文件**: `envoy.yaml`

```yaml
static_resources:
  listeners:
  - name: listener_0
    address:
      socket_address: { address: 0.0.0.0, port_value: 8080 }
    filter_chains:
    - filters:
      - name: envoy.filters.network.http_connection_manager
        typed_config:
          "@type": type.googleapis.com/envoy.extensions.filters.network.http_connection_manager.v3.HttpConnectionManager
          codec_type: auto
          route_config:
            name: local_route
            virtual_hosts:
            - name: local_service
              domains: ["*"]
              routes:
              - match: { prefix: "/im.AuthService" }
                route: { cluster: auth_service }
              - match: { prefix: "/im.MessageService" }
                route: { cluster: message_service }
              - match: { prefix: "/im.GroupService" }
                route: { cluster: group_service }
              - match: { prefix: "/im.SessionService" }
                route: { cluster: session_service }
              - match: { prefix: "/im.PresenceService" }
                route: { cluster: presence_service }
          http_filters:
          - name: envoy.filters.http.grpc_web
          - name: envoy.filters.http.cors
          typed_config: {}
  clusters:
  - name: auth_service
    connect_timeout: 0.25s
    type: static
    hosts:
    - socket_address: { address: localhost, port_value: 50001 }
  - name: message_service
    connect_timeout: 0.25s
    type: static
    hosts:
    - socket_address: { address: localhost, port_value: 50004 }
  - name: group_service
    connect_timeout: 0.25s
    type: static
    hosts:
    - socket_address: { address: localhost, port_value: 50005 }
  - name: session_service
    connect_timeout: 0.25s
    type: static
    hosts:
    - socket_address: { address: localhost, port_value: 50002 }
  - name: presence_service
    connect_timeout: 0.25s
    type: static
    hosts:
    - socket_address: { address: localhost, port_value: 50006 }
```

### 7.2 启动Envoy

```bash
docker run -d \
  --name envoy \
  -p 8080:8080 \
  -v /path/to/envoy.yaml:/envoy.yaml:ro \
  envoyproxy/envoy:latest \
  -c /envoy.yaml
```

### 7.3 客户端配置修改

```dart
class AppConfig {
  // 通过Envoy代理访问gRPC
  static const String grpcHost = 'localhost';
  static const int grpcPort = 8080;  // Envoy端口
}
```

## 八、常见问题排查

### 8.1 连接问题

| 问题 | 可能原因 | 解决方案 |
|------|----------|----------|
| 连接超时 | 服务端未启动 | 检查 `docker-compose ps` |
| 连接被拒绝 | 端口错误 | 确认端口号是否正确 |
| SSL错误 | 生产环境未配置证书 | 使用`ChannelCredentials.secure()` |

### 8.2 gRPC编译问题

| 问题 | 解决方案 |
|------|----------|
| proto文件找不到 | 检查 `-I` 参数路径 |
| 字段未定义 | 重新编译proto文件 |
| 服务类未找到 | 检查proto中的service定义 |

### 8.3 登录问题

| 问题 | 可能原因 | 解决方案 |
|------|----------|----------|
| 用户名不存在 | 用户未注册 | 先注册用户 |
| 密码错误 | 密码不正确 | 输入正确密码 |
| Token无效 | Token过期 | 重新登录 |

## 九、API参考

### 9.1 Auth Service

#### Login

```protobuf
rpc Login(LoginRequest) returns (LoginResponse);

message LoginRequest {
    string username = 1;
    string password = 2;
    string device_id = 3;
    string device_type = 4;  // ios/android/web
}

message LoginResponse {
    bool success = 1;
    Error error = 2;
    int64 user_id = 3;
    string token = 4;
    int64 expire_at = 5;
}
```

#### Register

```protobuf
rpc Register(RegisterRequest) returns (RegisterResponse);

message RegisterRequest {
    string username = 1;
    string password = 2;
    string phone = 3;
    string email = 4;
    string nickname = 5;
}

message RegisterResponse {
    bool success = 1;
    Error error = 2;
    int64 user_id = 3;
}
```

## 十、后续步骤

完成登录联调后，可继续对接以下功能：

1. **消息服务** - 消息发送/接收
2. **会话服务** - 会话列表
3. **群组服务** - 群组管理
4. **在线状态** - 用户在线状态
5. **WebSocket** - 实时消息推送

---

## 相关文档

- 服务端文档: `README.md`
- Flutter客户端文档: `src/client/flutter/README.md`
- 开发指南: `src/client/flutter/docs/DEVELOPMENT.md`
