# OurChat Auth Service 集成测试指南

本文档介绍如何对 Auth Service 进行完整的集成测试。

## 前置条件

### 1. Docker 服务运行

```bash
# 启动 MySQL 和 Redis
docker-compose up -d mysql redis

# 验证服务状态
docker-compose ps
```

### 2. 安装 grpcurl

```bash
# macOS
brew install grpcurl

# Linux
go install github.com/fullstorydev/grpcurl@latest

# Windows (Chocolatey)
choco install grpcurl
```

### 3. 启动 OurChat 服务器

```bash
# 方式一：使用 Docker
docker-compose up -d ourchat-server

# 方式二：本地编译运行
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
./src/server/ourchat_server ../config/server.yaml
```

## 快速测试

### 步骤 1：初始化数据库表

```bash
docker-compose exec mysql mysql -u ourchat -pourchat_password ourchat_db -e "
CREATE TABLE IF NOT EXISTS im_user (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(64) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    email VARCHAR(128),
    create_time BIGINT,
    update_time BIGINT
);
"
```

### 步骤 2：运行测试

```bash
# 验证服务列表
grpcurl -plaintext localhost:50051 list

# 测试注册
grpcurl -plaintext -d '{
  "username": "testuser001",
  "password": "test123456",
  "email": "test@example.com"
}' localhost:50051 im.AuthService/Register

# 测试登录
grpcurl -plaintext -d '{
  "username": "testuser001",
  "password": "test123456"
}' localhost:50051 im.AuthService/Login
```

## 完整测试脚本

保存为 `test_auth.sh` 并执行：

```bash
#!/bin/bash
set -e

SERVER="localhost:50051"
TIMESTAMP=$(date +%s)
TEST_USER="testuser_${TIMESTAMP}"

echo "=========================================="
echo "OurChat Auth Service Integration Test"
echo "=========================================="

# 1. 测试注册
echo -e "\n[1] Testing Register..."
REGISTER_RESULT=$(grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"test123456\",
  \"email\": \"test@example.com\"
}" $SERVER im.AuthService/Register)
echo "$REGISTER_RESULT"
USER_ID=$(echo "$REGISTER_RESULT" | jq -r '.userId')

# 2. 测试重复注册（应该失败）
echo -e "\n[2] Testing Duplicate Register (should fail)..."
grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"test123456\",
  \"email\": \"test@example.com\"
}" $SERVER im.AuthService/Register

# 3. 测试登录
echo -e "\n[3] Testing Login..."
LOGIN_RESULT=$(grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"test123456\"
}" $SERVER im.AuthService/Login)
echo "$LOGIN_RESULT"
TOKEN=$(echo "$LOGIN_RESULT" | jq -r '.token')
LOGIN_USER_ID=$(echo "$LOGIN_RESULT" | jq -r '.userId')

# 4. 测试错误密码登录
echo -e "\n[4] Testing Wrong Password Login (should fail)..."
grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"wrongpassword\"
}" $SERVER im.AuthService/Login

# 5. 测试不存在的用户登录
echo -e "\n[5] Testing Non-existent User Login (should fail)..."
grpcurl -plaintext -d '{
  "username": "nonexistent_user",
  "password": "test123456"
}' $SERVER im.AuthService/Login

# 6. 测试 Token 验证
echo -e "\n[6] Testing ValidateToken..."
grpcurl -plaintext -d "{\"token\":\"${TOKEN}\"}" $SERVER im.AuthService/ValidateToken

# 7. 测试无效 Token 验证
echo -e "\n[7] Testing Invalid Token (should fail)..."
grpcurl -plaintext -d '{"token":"invalid_token_here"}' $SERVER im.AuthService/ValidateToken

# 8. 测试 Token 刷新
echo -e "\n[8] Testing RefreshToken..."
REFRESH_RESULT=$(grpcurl -plaintext -d "{\"token\":\"${TOKEN}\"}" $SERVER im.AuthService/RefreshToken)
echo "$REFRESH_RESULT"
NEW_TOKEN=$(echo "$REFRESH_RESULT" | jq -r '.token')

# 9. 使用新 Token 验证
echo -e "\n[9] Testing New Token Validation..."
grpcurl -plaintext -d "{\"token\":\"${NEW_TOKEN}\"}" $SERVER im.AuthService/ValidateToken

# 10. 测试登出
echo -e "\n[10] Testing Logout..."
grpcurl -plaintext -d "{\"userId\":${LOGIN_USER_ID}}" $SERVER im.AuthService/Logout

# 11. 验证登出后 Token 无效
echo -e "\n[11] Testing Token After Logout (should fail)..."
grpcurl -plaintext -d "{\"token\":\"${TOKEN}\"}" $SERVER im.AuthService/ValidateToken

echo -e "\n=========================================="
echo "All tests completed!"
echo "=========================================="
```

```bash
chmod +x test_auth.sh
./test_auth.sh
```

## 预期结果

### 成功响应格式

```json
{
  "success": true,
  "userId": 1,
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "message": ""
}
```

### 失败响应格式

```json
{
  "success": false,
  "message": "Username already exists or database error"
}
```

## 接口详细说明

### 1. Register - 用户注册

**请求：**
```json
{
  "username": "string",
  "password": "string",
  "email": "string"
}
```

**响应：**
```json
{
  "success": bool,
  "userId": int64,
  "message": "string"
}
```

### 2. Login - 用户登录

**请求：**
```json
{
  "username": "string",
  "password": "string"
}
```

**响应：**
```json
{
  "success": bool,
  "userId": int64,
  "token": "string",
  "message": "string"
}
```

### 3. Logout - 用户登出

**请求：**
```json
{
  "userId": int64,
  "token": "string"
}
```

**响应：**
```json
{
  "success": bool,
  "message": "string"
}
```

### 4. RefreshToken - 刷新 Token

**请求：**
```json
{
  "token": "string"
}
```

**响应：**
```json
{
  "success": bool,
  "token": "string",
  "message": "string"
}
```

### 5. ValidateToken - 验证 Token

**请求：**
```json
{
  "token": "string"
}
```

**响应：**
```json
{
  "success": bool,
  "userId": int64
}
```

## 故障排查

### 服务器日志

```bash
# 查看服务器日志
docker-compose logs -f ourchat-server

# 查看最后 100 行
docker-compose logs --tail 100 ourchat-server
```

### MySQL 连接测试

```bash
# 检查 MySQL 连接
docker-compose exec mysql mysql -u ourchat -pourchat_password -e "SELECT 1"

# 查看数据库表
docker-compose exec mysql mysql -u ourchat -pourchat_password ourchat_db -e "SHOW TABLES"

# 查看用户数据
docker-compose exec mysql mysql -u ourchat -pourchat_password ourchat_db -e "SELECT * FROM im_user;"
```

### Redis 连接测试

```bash
# 检查 Redis 连接
docker-compose exec redis redis-cli ping

# 查看 Token 缓存
docker-compose exec redis redis-cli keys "*token*"

# 查看特定用户 Token
docker-compose exec redis redis-cli get "user_token:1"
```

### 常见错误

| 错误 | 原因 | 解决方案 |
|------|------|----------|
| `Database connection failed` | MySQL 未运行或配置错误 | 检查 `docker-compose ps` 和配置文件 |
| `User not found` | 用户名不存在 | 先注册用户 |
| `Invalid credentials` | 密码错误 | 检查密码是否正确 |
| `Username already exists` | 用户名已被注册 | 使用其他用户名 |
| `Invalid token` | Token 过期或伪造 | 重新登录获取新 Token |

## Docker Compose 测试环境

使用独立的测试环境，不影响生产配置：

```yaml
# docker-compose.test.yml
version: '3.8'

services:
  mysql:
    image: mysql:8.0
    environment:
      MYSQL_ROOT_PASSWORD: rootpass
      MYSQL_DATABASE: ourchat_db
      MYSQL_USER: ourchat
      MYSQL_PASSWORD: ourchat_password
    ports:
      - "3306:3306"
    healthcheck:
      test: ["CMD", "mysqladmin", "ping", "-h", "localhost"]
      interval: 5s
      timeout: 5s
      retries: 10

  redis:
    image: redis:7-alpine
    ports:
      - "6379:6379"
    healthcheck:
      test: ["CMD", "redis-cli", "ping"]
      interval: 5s
      timeout: 5s
      retries: 5

  ourchat-server:
    build: .
    ports:
      - "50051:50051"
    depends_on:
      mysql:
        condition: service_healthy
      redis:
        condition: service_healthy
    environment:
      - CONFIG_PATH=/app/config/server.yaml

  prometheus:
    image: prom/prometheus:latest
    ports:
      - "9090:9090"
    volumes:
      - ./prometheus/prometheus.yml:/etc/prometheus/prometheus.yml

  grafana:
    image: grafana/grafana:latest
    ports:
      - "3000:3000"
    depends_on:
      - prometheus
```

启动测试环境：
```bash
docker-compose -f docker-compose.test.yml up -d
```

## CI/CD 自动化测试

在 CI 流水线中添加测试步骤：

```yaml
# .github/workflows/test.yml
name: Auth Service Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Start dependencies
        run: docker-compose up -d mysql redis
        
      - name: Wait for MySQL
        run: |
          until docker-compose exec -T mysql mysqladmin ping -h localhost; do
            sleep 1
          done
          
      - name: Build and test
        run: |
          mkdir -p build && cd build
          cmake -DCMAKE_BUILD_TYPE=Release ..
          make -j$(nproc)
          ./src/server/ourchat_server ../config/server.yaml &
          SERVER_PID=$!
          
          # Run integration tests
          sleep 5
          ./test_auth.sh
          
          # Cleanup
          kill $SERVER_PID
```
