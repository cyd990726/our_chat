# OurChat Flutter Client

OurChat Flutter客户端是面向Web平台的即时通讯应用，采用Flutter框架开发，支持跨平台部署。

## 技术栈

| 类别 | 技术选择 |
|------|----------|
| 框架 | Flutter 3.38+ |
| 状态管理 | Riverpod 2.4+ |
| 路由管理 | Go Router 12+ |
| 网络通信 | gRPC + WebSocket |
| 本地存储 | SQLite (Drift) |
| 安全存储 | Flutter Secure Storage |
| 偏好存储 | Shared Preferences |

## 系统架构

### 分层架构设计

```
┌─────────────────────────────────────────────────────────┐
│                    UI Layer (表现层)                     │
│  ┌──────────┬──────────┬──────────┬──────────┐         │
│  │ Screens  │Components │  Theme   │Navigation│         │
│  └──────────┴──────────┴──────────┴──────────┘         │
├─────────────────────────────────────────────────────────┤
│               ViewModel / Store (状态管理)               │
│              Riverpod Providers & ViewModels             │
├─────────────────────────────────────────────────────────┤
│                  Service Layer (服务层)                  │
│  ┌──────────┬──────────┬──────────┬──────────┐         │
│  │  Auth    │ Message  │  Group   │Presence  │         │
│  │ Service  │ Service  │ Service  │ Service  │         │
│  └──────────┴──────────┴──────────┴──────────┘         │
├─────────────────────────────────────────────────────────┤
│              Network Layer (网络通信层)                  │
│  ┌─────────────────┬─────────────────┐                  │
│  │   gRPC Client   │WebSocket Manager│                  │
│  └─────────────────┴─────────────────┘                  │
├─────────────────────────────────────────────────────────┤
│                 Data Layer (数据层)                      │
│  ┌─────────────────┬─────────────────┐                  │
│  │  SQLite (Drift) │  Repositories   │                  │
│  └─────────────────┴─────────────────┘                  │
└─────────────────────────────────────────────────────────┘
```

### 通信架构

```
┌──────────────┐    gRPC (请求响应)     ┌──────────────┐
│   Flutter    │ ◄────────────────────► │  Auth Service│
│   Client     │                        │  (50001)     │
└──────────────┘                        └──────────────┘
       │
       │ WebSocket (实时推送)
       ▼
┌──────────────┐    gRPC (请求响应)     ┌──────────────┐
│   Gateway    │ ◄────────────────────► │ Message Svc  │
│   Service    │                        │  (50004)     │
│   (50003)    │                        └──────────────┘
└──────────────┘
```

### 数据流

```
用户操作 → ViewModel → Service → gRPC/WebSocket → 更新UI
                ↓
         Local Database (SQLite)
                ↓
         离线消息同步
```

## 项目结构

```
src/client/flutter/
├── android/                    # Android平台特定代码
├── ios/                        # iOS平台特定代码
├── web/                        # Web平台配置
├── macos/                      # macOS平台特定代码
├── windows/                    # Windows平台特定代码
├── linux/                      # Linux平台特定代码
├── lib/                        # Flutter核心代码
│   ├── main.dart              # 应用入口
│   ├── ourchat_app.dart       # 应用配置
│   ├── core/                   # 核心模块
│   │   ├── config/            # 配置管理
│   │   │   └── app_config.dart
│   │   └── auth/              # 认证模块
│   │       └── auth_manager.dart
│   ├── data/                   # 数据层
│   │   ├── local/             # 本地存储
│   │   │   └── database/      # SQLite数据库
│   │   └── repository/        # 数据仓库
│   ├── domain/                 # 领域层
│   │   └── entities/          # 业务实体
│   ├── services/              # 服务层
│   │   ├── message/           # 消息服务
│   │   ├── group/             # 群组服务
│   │   ├── session/           # 会话服务
│   │   └── presence/          # 在线状态服务
│   └── ui/                     # 表现层
│       ├── screens/           # 页面组件
│       │   ├── auth/         # 认证页面
│       │   ├── chat/         # 聊天页面
│       │   ├── contacts/     # 联系人页面
│       │   ├── profile/      # 个人主页
│       │   └── settings/     # 设置页面
│       ├── components/        # 公共组件
│       ├── theme/             # 主题配置
│       │   ├── app_theme.dart
│       │   ├── app_colors.dart
│       │   └── app_text_styles.dart
│       └── navigation/        # 路由管理
│           ├── app_router.dart
│           └── route_names.dart
├── assets/                     # 静态资源
│   ├── images/                # 图片资源
│   ├── icons/                 # 图标资源
│   └── fonts/                 # 字体资源
├── test/                       # 单元测试
├── integration_test/           # 集成测试
├── docs/                       # 开发文档
│   └── DEVELOPMENT.md
├── pubspec.yaml               # 依赖配置
├── analysis_options.yaml      # 代码规范配置
└── README.md                  # 项目说明
```

## 快速开始

### 环境要求

- Flutter SDK 3.38+
- Dart SDK 3.0+
- Chrome 80+ (Web测试)
- Xcode 14+ (iOS开发)
- Android Studio (Android开发)

### 安装依赖

```bash
cd src/client/flutter
flutter pub get
```

### 运行项目

#### Web平台（推荐）

```bash
flutter run -d chrome --web-hostname localhost --web-port 54321
```

运行地址: http://localhost:54321

#### 其他平台

```bash
# macOS
flutter run -d macos

# Windows
flutter run -d windows

# Linux
flutter run -d linux

# iOS (需要Xcode)
flutter run -d ios

# Android (需要Android Studio)
flutter run -d android
```

### 热重载与热重启

在运行状态下：
- 按 `r` - 热重载（保留状态）
- 按 `R` - 热重启（重置状态）
- 按 `h` - 显示帮助
- 按 `q` - 退出

## 构建部署

### Web平台

```bash
# Debug构建
flutter build web --debug

# Release构建（生产环境）
flutter build web --release

# 输出目录
build/web/
```

### 部署Web

使用任意Web服务器部署 `build/web/` 目录：

```bash
# 使用nginx
nginx -g 'daemon off;' -c /path/to/nginx.conf

# 使用 serve
npx serve build/web

# 使用 Python
python3 -m http.server 8080 -d build/web
```

#### Nginx配置示例

```nginx
server {
    listen 80;
    server_name your-domain.com;
    root /path/to/build/web;
    index index.html;

    location / {
        try_files $uri $uri/ /index.html;
    }

    # gRPC-Web代理配置
    location /grpc {
        grpc_pass grpc://localhost:50001;
    }
}
```

### 移动平台

```bash
# iOS (需要macOS和Xcode)
flutter build ipa

# Android
flutter build apk --split-per-abi    # 按CPU架构分离
flutter build apk                    # 通用APK
flutter build appbundle              # AppBundle (推荐)
```

### 桌面平台

```bash
# macOS
flutter build macos

# Windows
flutter build windows

# Linux
flutter build linux
```

## 功能特性

### 已完成

- [x] 启动页面 (Splash Screen)
- [x] 用户登录
- [x] 用户注册
- [x] 聊天列表
- [x] 聊天详情
- [x] 联系人管理
- [x] 个人资料
- [x] 设置页面
- [x] 深色模式支持

### 开发中

- [ ] 群组功能
- [ ] 群聊详情
- [ ] 创建群组
- [ ] 文件图片发送
- [ ] 消息推送通知
- [ ] 消息已读回执
- [ ] 在线状态显示

### 规划中

- [ ] 语音消息
- [ ] 视频通话
- [ ] 消息加密
- [ ] 多设备同步

## 配置

### 服务器配置

编辑 `lib/core/config/app_config.dart`：

```dart
// gRPC服务器地址
String get grpcHost => _prefs.getString('grpc_host') ?? 'localhost';
int get grpcPort => _prefs.getInt('grpc_port') ?? 50001;

// WebSocket地址
String get wsUrl => _prefs.getString('ws_url') ?? 'ws://localhost:50003';

// 深色模式
bool get isDarkMode => _prefs.getBool('dark_mode') ?? false;
```

### 环境变量

可以在运行时通过 Shared Preferences 修改配置，或创建配置文件：

```dart
class AppConfig {
  // 开发环境
  static const String grpcHost = 'localhost';
  static const int grpcPort = 50001;
  static const String wsUrl = 'ws://localhost:50003';
  
  // 生产环境
  // static const String grpcHost = 'api.ourchat.com';
  // static const int grpcPort = 50001;
  // static const String wsUrl = 'wss://api.ourchat.com/ws';
}
```

## 测试

### 单元测试

```bash
# 运行所有单元测试
flutter test

# 运行特定测试文件
flutter test test/unit/auth_manager_test.dart

# 生成测试覆盖率报告
flutter test --coverage
genhtml coverage/lcov.info -o coverage/html
open coverage/html/index.html
```

### Widget测试

```bash
flutter test test/widget/
```

### 集成测试

```bash
# 连接真机或模拟器后运行
flutter test integration_test/
```

## 文档

详细开发文档请参考：
- [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md) - 完整开发文档
- [API对接说明](docs/DEVELOPMENT.md#六api对接) - gRPC和WebSocket接口
- [测试指南](docs/DEVELOPMENT.md#三测试方法) - 单元测试、Widget测试、集成测试

## 贡献

1. Fork项目
2. 创建特性分支 `git checkout -b feature/your-feature`
3. 提交代码 `git commit -m 'Add your feature'`
4. 推送分支 `git push origin feature/your-feature`
5. 发起Pull Request

## 许可证

MIT License
