# OurChat

基于C++、gRPC、MySQL和Redis的百万级并发即时通讯系统

## 目录

- [项目简介](#项目简介)
- [系统架构](#系统架构)
- [核心功能](#核心功能)
- [技术栈](#技术栈)
- [快速开始](#快速开始)
- [客户端开发](#客户端开发)
- [项目结构](#项目结构)
- [API文档](#api文档)
- [部署指南](#部署指南)
- [性能优化](#性能优化)
- [监控运维](#监控运维)

## 项目简介

OurChat是一个高性能分布式即时通讯服务器，专为百万级并发用户设计。系统采用微服务架构，使用C++17和gRPC实现高性能通信，支持用户登录注册、点对点聊天、好友管理、群组聊天等核心功能。

## 系统架构

### 整体架构

```
┌─────────────────────────────────────────────────────────────────────┐
│                          用户端 (iOS/Android/Web)                     │
└─────────────────────────────────────────────────────────────────────┘
                                    │
                    ┌───────────────┴───────────────┐
                    ▼                               ▼
            ┌──────────────┐               ┌──────────────┐
            │   Nginx      │               │   Nginx      │
            │  (负载均衡)   │               │  (负载均衡)   │
            └──────┬───────┘               └──────┬───────┘
                   │                               │
                   └───────────┬───────────────────┘
                               ▼
┌─────────────────────────────────────────────────────────────────────┐
│                          API Gateway (Envoy/Nginx)                    │
├─────────────────────────────────────────────────────────────────────┤
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐               │
│  │  Auth Svc    │  │  Session     │  │  Gateway     │               │
│  │  (gRPC)      │  │  Svc (gRPC)  │  │  Svc (WS)    │               │
│  │  :50001      │  │  :50002      │  │  :50003      │               │
│  └──────────────┘  └──────────────┘  └──────────────┘               │
├─────────────────────────────────────────────────────────────────────┤
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐               │
│  │  Message     │  │  Group       │  │  Presence    │               │
│  │  Svc (gRPC)  │  │  Svc (gRPC)  │  │  Svc (gRPC)  │               │
│  │  :50004      │  │  :50005      │  │  :50006      │               │
│  └──────────────┘  └──────────────┘  └──────────────┘               │
├─────────────────────────────────────────────────────────────────────┤
│                         gRPC Service Mesh (可选)                      │
├─────────────────────────────────────────────────────────────────────┤
│    Kafka (消息队列)     │    Redis Cluster (缓存)    │  MySQL Shards │
│  ┌─────────────────┐   │   ┌───────────────────┐    │ ┌───────────┐  │
│  │ im_message_raw  │   │   │ Session Cache     │    │ │ Shard 0-15│  │
│  │ im_presence     │   │   │ Online Status     │    │ └───────────┘  │
│  │ im_group_events │   │   │ Rate Limiting     │    │                │
│  └─────────────────┘   │   └───────────────────┘    │                │
└────────────────────────┴───────────────────────────┴────────────────┘
```

### 微服务划分

| 服务名称 | 端口 | 功能职责 | 实例数建议 |
|---------|------|---------|-----------|
| Auth Service | 50001 | 用户认证、登录注册、Token管理 | 4-6 |
| Session Service | 50002 | 会话管理、好友关系 | 4-6 |
| Gateway Service | 50003 | WebSocket网关、长连接管理 | 10-20 |
| Message Service | 50004 | 消息存储、转发、已读回执 | 8-12 |
| Group Service | 50005 | 群组管理、群成员管理 | 4-6 |
| Presence Service | 50006 | 用户在线状态、心跳处理 | 4-6 |

## 核心功能

### 用户系统
- 用户注册和登录
- JWT Token认证
- 多设备登录管理
- 用户信息管理

### 消息系统
- 点对点消息发送
- 消息类型支持（文本、图片、语音、视频、文件、位置）
- 消息已读/未读状态
- 消息历史记录
- 离线消息推送

### 好友系统
- 好友添加/删除
- 好友列表管理
- 好友信息查询
- 好友搜索

### 群组系统
- 创建/解散群组
- 群成员管理
- 群消息广播
- 群成员角色（群主、管理员、普通成员）

### 在线状态
- 用户在线状态查询
- 好友在线状态展示
- 心跳保活机制
- 多设备状态同步

## 技术栈

### 核心框架
- **C++17** - 主要开发语言
- **gRPC** - 服务间通信框架
- **Protocol Buffers** - 接口定义和数据序列化
- **CMake** - 构建系统

### 数据库
- **MySQL 8.0** - 主数据存储
  - 分片策略：用户ID取模分片
  - 读写分离：主从复制
- **Redis 7.x** - 缓存和状态管理
  - 在线状态存储
  - 会话缓存
  - 消息序列号
  - 限流控制

### 消息队列
- **Apache Kafka** - 消息异步处理
  - 消息持久化
  - 消息重放
  - 事件驱动

### 基础设施
- **Docker** - 容器化部署
- **Kubernetes** - 容器编排（可选）
- **Nginx** - 负载均衡
- **Prometheus** - 指标监控
- **Grafana** - 可视化面板

## 快速开始

### 环境要求

- GCC 12+
- CMake 3.16+
- MySQL 8.0+
- Redis 7.x+
- Apache Kafka 3.x+

### 安装依赖

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libssl-dev \
    libmysqlclient-dev libhiredis-dev libyaml-cpp-dev

# macOS
brew install cmake openssl mysql-client hiredis yaml-cpp
```

### 编译项目

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### 配置修改

编辑 `config/server.yaml`，修改以下配置项：

```yaml
mysql:
  host: "localhost"
  port: 3306
  username: "ourchat"
  password: "your_password"
  database: "ourchat_db"

redis:
  host: "localhost"
  port: 6379
```

### 初始化数据库

```bash
mysql -u ourchat -p ourchat_db < scripts/init.sql
```

### 启动服务

```bash
./bin/ourchat_server config/server.yaml
```

### Docker部署

```bash
docker-compose up -d
```

## 客户端开发

OurChat提供Flutter客户端，支持Web、iOS、Android和桌面平台。

### 技术栈

| 类别 | 技术选择 |
|------|----------|
| 框架 | Flutter 3.38+ |
| 状态管理 | Riverpod 2.4+ |
| 路由管理 | Go Router 12+ |
| 网络通信 | gRPC + WebSocket |
| 本地存储 | SQLite (Drift) |

### 快速开始

```bash
# 进入客户端目录
cd src/client/flutter

# 安装依赖
flutter pub get

# 运行Web版本
flutter run -d chrome --web-hostname localhost --web-port 54321
```

### 项目结构

```
src/client/flutter/
├── lib/
│   ├── main.dart              # 应用入口
│   ├── core/                  # 核心模块
│   │   ├── config/           # 配置管理
│   │   └── auth/             # 认证模块
│   ├── data/                  # 数据层
│   │   ├── local/            # 本地存储
│   │   └── repository/       # 数据仓库
│   ├── services/              # 服务层
│   │   ├── message/          # 消息服务
│   │   ├── group/            # 群组服务
│   │   └── presence/         # 在线状态服务
│   └── ui/                    # 表现层
│       ├── screens/          # 页面组件
│       ├── components/       # 公共组件
│       ├── theme/            # 主题配置
│       └── navigation/       # 路由管理
└── docs/                      # 开发文档
```

### 功能特性

- [x] 用户登录/注册
- [x] 聊天列表
- [x] 聊天详情
- [x] 联系人管理
- [x] 个人资料
- [x] 设置页面
- [x] 深色模式支持

详细文档请参考 [src/client/flutter/README.md](src/client/flutter/README.md) 和 [src/client/flutter/docs/DEVELOPMENT.md](src/client/flutter/docs/DEVELOPMENT.md)

### 编译项目

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### 配置修改

编辑 `config/server.yaml`，修改以下配置项：

```yaml
mysql:
  host: "localhost"
  port: 3306
  username: "ourchat"
  password: "your_password"
  database: "ourchat_db"

redis:
  host: "localhost"
  port: 6379
```

### 初始化数据库

```bash
mysql -u ourchat -p ourchat_db < scripts/init.sql
```

### 启动服务

```bash
./bin/ourchat_server config/server.yaml
```

### Docker部署

```bash
docker-compose up -d
```

## 项目结构

```
ourchat-server/
├── CMakeLists.txt              # 顶层CMake配置
├── cmake/
│   └── FindGRPC.cmake
├── src/
│   ├── common/                 # 公共组件
│   │   ├── config/            # 配置管理
│   │   ├── logger/            # 日志系统
│   │   └── utils/             # 工具函数
│   ├── proto/                 # Protocol Buffers
│   │   ├── common.proto
│   │   ├── user.proto
│   │   ├── message.proto
│   │   ├── group.proto
│   │   ├── session.proto
│   │   └── presence.proto
│   ├── services/              # 业务服务层
│   │   ├── auth/             # 认证服务
│   │   ├── message/          # 消息服务
│   │   ├── group/            # 群组服务
│   │   ├── session/          # 会话服务
│   │   └── presence/         # 在线状态服务
│   ├── data/                  # 数据访问层
│   │   ├── mysql/            # MySQL访问
│   │   ├── redis/            # Redis访问
│   │   └── repository/       # 数据仓库
│   ├── network/               # 网络层
│   │   ├── grpc/             # gRPC服务器
│   │   └── websocket/        # WebSocket网关
│   └── server/                # 服务器主程序
├── include/                   # 头文件
├── config/                    # 配置文件
├── scripts/                   # 部署脚本
├── tests/                     # 测试代码
├── third_party/              # 第三方库
├── Dockerfile
├── docker-compose.yml
└── README.md
```

## API文档

### 认证服务 (Auth Service)

#### 用户注册
```protobuf
rpc Register(RegisterRequest) returns (RegisterResponse);
```

Request:
```protobuf
message RegisterRequest {
    string username = 1;  // 用户名
    string password = 2;  // 密码
    string phone = 3;     // 手机号
    string email = 4;     // 邮箱
    string nickname = 5;  // 昵称
}
```

Response:
```protobuf
message RegisterResponse {
    bool success = 1;
    Error error = 2;
    int64 user_id = 3;
}
```

#### 用户登录
```protobuf
rpc Login(LoginRequest) returns (LoginResponse);
```

Request:
```protobuf
message LoginRequest {
    string username = 1;
    string password = 2;
    string device_id = 3;
    string device_type = 4;  // ios/android/web
}
```

Response:
```protobuf
message LoginResponse {
    bool success = 1;
    int64 user_id = 2;
    string token = 3;       // JWT Token
    int64 expire_at = 4;    // 过期时间戳
}
```

### 消息服务 (Message Service)

#### 发送消息
```protobuf
rpc SendMessage(SendMessageRequest) returns (SendMessageResponse);
```

Request:
```protobuf
message SendMessageRequest {
    int64 sender_id = 1;
    int64 receiver_id = 2;
    MessageType message_type = 3;  // 消息类型
    string content = 4;            // 消息内容
    int64 client_message_id = 5;   // 客户端消息ID
}
```

### 群组服务 (Group Service)

#### 创建群组
```protobuf
rpc CreateGroup(CreateGroupRequest) returns (CreateGroupResponse);
```

Request:
```protobuf
message CreateGroupRequest {
    int64 owner_id = 1;
    string group_name = 2;
    repeated int64 member_ids = 3;
    string description = 4;
}
```

## 部署指南

### 单机部署

```bash
# 1. 编译项目
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# 2. 配置数据库
mysql -u root -p < scripts/init.sql

# 3. 启动服务
./bin/ourchat_server config/server.yaml
```

### Docker部署

```bash
# 构建镜像
docker build -t ourchat-server .

# 启动所有服务
docker-compose up -d

# 查看日志
docker-compose logs -f ourchat-server
```

### Kubernetes部署

```bash
# 创建ConfigMap
kubectl create configmap ourchat-config --from-file=config/server.yaml

# 部署服务
kubectl apply -f k8s/

# 扩容
kubectl scale deployment ourchat-server --replicas=10
```

## 性能优化

### 连接优化
- gRPC连接复用
- HTTP/2多路复用
- Keepalive保活机制
- 连接池管理

### 数据库优化
- MySQL连接池
- Redis缓存策略
- 分库分表
- 读写分离

### 消息优化
- Kafka批量处理
- 消息压缩
- 异步写入
- 消息压缩

### 缓存策略
- 多级缓存架构
- 缓存预热
- 缓存过期策略
- 热点数据缓存

## 监控运维

### 监控指标
- QPS/TPS
- 响应时间
- 错误率
- 连接数
- 消息队列积压

### 日志规范
- 结构化日志
- 日志级别控制
- 日志轮转
- 分布式追踪

### 健康检查
- 服务健康检查
- 依赖服务检查
- 自动故障转移
- 熔断降级

## 许可证

MIT License

## 贡献指南

欢迎提交Issue和Pull Request。
