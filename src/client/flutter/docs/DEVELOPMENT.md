# OurChat Flutter 客户端开发文档

## 一、项目概述

### 1.1 项目简介

OurChat Flutter客户端是面向Web平台的即时通讯应用，采用Flutter框架开发，支持跨平台部署。该客户端与OurChat后端服务器进行通信，提供用户认证、即时消息、好友管理、群组聊天等核心功能。客户端采用gRPC协议与服务器进行高效通信，同时集成WebSocket实现实时消息推送，确保消息的即时性和可靠性。

### 1.2 技术栈概览

本项目采用以下核心技术栈：

- **编程语言**: Dart 3.0+
- **UI框架**: Flutter 3.10+
- **UI组件库**: GetWidget (第三方UI组件库)
- **状态管理**: Riverpod
- **网络通信**: gRPC + WebSocket
- **本地存储**: SQLite (Drift)
- **路由管理**: GoRouter
- **安全存储**: Flutter Secure Storage

### 1.3 环境要求

开发环境要求：
- Flutter SDK 3.10+
- Dart SDK 3.0+
- Xcode 14+ (iOS开发)
- Android Studio (Android开发)
- VS Code (推荐编辑器)

## 二、项目结构

### 2.1 目录结构

```
src/client/flutter/
├── android/                    # Android平台特定代码
├── ios/                        # iOS平台特定代码
├── web/                        # Web平台特定配置
├── lib/                        # Flutter核心代码
│   ├── main.dart              # 应用入口
│   ├── ourchat_app.dart       # 应用配置
│   ├── core/                   # 核心模块
│   │   ├── config/            # 配置管理
│   │   └── auth/              # 认证模块
│   ├── data/                   # 数据层
│   │   ├── local/             # 本地存储
│   │   └── repository/        # 数据仓库
│   ├── domain/                 # 领域层
│   │   └── entities/          # 业务实体
│   ├── services/              # 服务层
│   │   ├── message/           # 消息服务
│   │   ├── group/             # 群组服务
│   │   └── presence/          # 在线状态服务
│   └── ui/                     # 表现层
│       ├── screens/           # 页面组件
│       ├── components/        # 公共组件
│       ├── theme/             # 主题配置
│       └── navigation/        # 路由管理
├── assets/                     # 静态资源
├── pubspec.yaml               # 依赖配置
└── docs/                       # 文档
```

### 2.2 核心模块说明

#### lib/core 核心模块
- **config/**: 应用配置管理，包括服务器地址、主题设置等
- **auth/**: 认证模块，管理JWT Token、登录状态等

#### lib/data 数据层
- **local/**: SQLite数据库操作，使用Drift ORM
- **repository/**: 数据仓库，封装数据访问逻辑

#### lib/ui 表现层
- **screens/**: 按功能模块组织的页面组件
- **components/**: 可复用的UI组件
- **theme/**: 应用主题和样式
- **navigation/**: 路由配置

## 三、快速开始

### 3.1 环境配置

```bash
# 安装Flutter SDK
flutter doctor

# 获取依赖
cd src/client/flutter
flutter pub get

# 运行Web版本
flutter run -d chrome

# 构建Web版本
flutter build web --release
```

### 3.2 项目配置

编辑 `lib/core/config/app_config.dart` 配置服务器地址：

```dart
String get grpcHost => _prefs.getString('grpc_host') ?? 'localhost';
int get grpcPort => _prefs.getInt('grpc_port') ?? 50001;
String get wsUrl => _prefs.getString('ws_url') ?? 'ws://localhost:50003';
```

## 四、核心功能实现

### 4.1 认证模块

认证模块负责用户登录、注册、Token管理等核心认证功能。

#### 主要功能：
- 用户登录/注册
- JWT Token管理
- 自动登录状态检测
- 安全的Token存储

#### 核心文件：
- `lib/core/auth/auth_manager.dart`: 认证管理器

### 4.2 消息功能

消息功能是IM应用的核心，包括：

- 实时消息发送和接收
- 消息列表展示
- 聊天详情页面
- 消息本地存储（SQLite）

#### 核心文件：
- `lib/ui/screens/chat/chat_list_screen.dart`: 聊天列表
- `lib/ui/screens/chat/chat_detail_screen.dart`: 聊天详情

### 4.3 联系人功能

联系人管理功能包括：

- 联系人列表展示
- 在线状态显示
- 添加/删除联系人
- 联系人搜索

#### 核心文件：
- `lib/ui/screens/contacts/contacts_screen.dart`: 联系人列表

### 4.4 设置功能

设置页面包括：

- 隐私设置
- 通知设置
- 深色模式切换
- 语言设置
- 账号安全

#### 核心文件：
- `lib/ui/screens/settings/settings_screen.dart`: 设置页面

## 五、第三方库使用

### 5.1 UI组件库 - GetWidget

GetWidget是一个基于Flutter的UI组件库，提供了丰富的预制组件：

```dart
import 'package:getwidget/getwidget.dart';

// 使用示例
GFButton(
  text: 'Login',
  fullWidth: true,
  size: GFSize.LARGE,
  onPressed: () {},
);

GFTextField(
  hintText: 'Username',
  controller: usernameController,
  prefixIcon: const Icon(Icons.person),
);
```

### 5.2 状态管理 - Riverpod

Riverpod是Flutter的状态管理解决方案：

```dart
import 'package:flutter_riverpod/flutter_riverpod.dart';

final counterProvider = StateProvider((ref) => 0);

class MyWidget extends ConsumerWidget {
  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final count = ref.watch(counterProvider);
    return Text('$count');
  }
}
```

### 5.3 本地存储 - Drift

Drift是Flutter的SQLite ORM：

```dart
import 'package:drift/drift.dart';

@DataClassName('Message')
class Messages extends Table {
  IntColumn get id => integer().autoIncrement()();
  TextColumn get content => text()();
  IntColumn get senderId => integer()();
  IntColumn get receiverId => integer()();
  IntColumn get timestamp => integer()();
}
```

## 六、API对接

### 6.1 gRPC服务调用

客户端通过gRPC与服务端通信：

```dart
// 示例：发送消息
final channel = ClientChannel('localhost', port: 50001);
final stub = MessageServiceClient(channel);

final request = SendMessageRequest(
  senderId: currentUserId,
  receiverId: peerId,
  content: message,
);

await stub.sendMessage(request);
```

### 6.2 WebSocket实时通信

WebSocket用于实时消息推送：

```dart
final channel = WebSocketChannel.connect(Uri.parse('ws://localhost:50003'));

channel.stream.listen((message) {
  // 处理接收到的消息
});
```

## 七、构建与部署

### 7.1 Web构建

```bash
# 构建生产版本
flutter build web --release

# 输出目录
build/web/
```

### 7.2 部署配置

Web部署需要配置Nginx或Envoy代理gRPC-Web请求：

```nginx
location /grpc {
  grpc_pass grpc://localhost:50001;
}
```

## 八、测试

### 8.1 单元测试

```bash
flutter test
```

### 8.2 集成测试

```bash
flutter test integration_test/
```

## 九、性能优化

### 9.1 消息列表优化
- 使用ListView.builder实现虚拟滚动
- 消息图片懒加载
- 本地消息缓存

### 9.2 网络优化
- gRPC连接复用
- 请求超时控制
- 断线重连机制

## 十、常见问题

### 10.1 Web平台兼容性问题
确保使用支持WebSocket的浏览器，推荐Chrome 80+、Firefox 75+、Safari 14+。

### 10.2 gRPC-Web配置
Web平台需要通过Envoy代理gRPC调用，配置正确的路由规则。

## 十一、后续开发计划

1. 实现完整的gRPC客户端对接
2. 添加WebSocket心跳和断线重连
3. 实现离线消息同步
4. 添加群组功能
5. 实现文件图片发送
6. 添加消息推送通知
7. 完善测试覆盖

## 十二、贡献指南

1. Fork项目
2. 创建特性分支
3. 提交代码
4. 发起Pull Request

## 十三、许可证

MIT License
