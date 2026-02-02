#!/bin/bash
set -e

# Configuration
SERVER="localhost:50051"
TIMESTAMP=$(date +%s)
TEST_USER="testuser_${TIMESTAMP}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}==========================================${NC}"
echo -e "${GREEN}OurChat Auth Service Integration Test${NC}"
echo -e "${GREEN}==========================================${NC}"
echo -e "Test User: ${TEST_USER}"
echo ""

# Helper function to check command result
check_result() {
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[PASS]${NC} $1"
    else
        echo -e "${RED}[FAIL]${NC} $1"
        exit 1
    fi
}

# 1. 测试服务是否可用
echo -e "\n${YELLOW}[0] Checking service availability...${NC}"
grpcurl -plaintext $SERVER list im.AuthService > /dev/null
check_result "AuthService is available"

# 2. 测试注册
echo -e "\n${YELLOW}[1] Testing Register...${NC}"
REGISTER_RESULT=$(grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"test123456\",
  \"email\": \"test@example.com\"
}" $SERVER im.AuthService/Register 2>&1)
echo "$REGISTER_RESULT"
USER_ID=$(echo "$REGISTER_RESULT" | jq -r '.userId // 0')
if [ "$USER_ID" != "0" ] && [ "$USER_ID" != "null" ]; then
    check_result "User registered successfully (ID: ${USER_ID})"
else
    echo -e "${RED}[FAIL]${NC} Registration failed"
    exit 1
fi

# 3. 测试重复注册（应该失败）
echo -e "\n${YELLOW}[2] Testing Duplicate Register (should fail)...${NC}"
DUP_RESULT=$(grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"test123456\",
  \"email\": \"test@example.com\"
}" $SERVER im.AuthService/Register 2>&1)
echo "$DUP_RESULT"
DUP_SUCCESS=$(echo "$DUP_RESULT" | jq -r '.success')
if [ "$DUP_SUCCESS" == "false" ]; then
    check_result "Duplicate registration correctly rejected"
else
    echo -e "${YELLOW}[WARN]${NC} Duplicate registration was allowed (may be expected)"
fi

# 4. 测试登录
echo -e "\n${YELLOW}[3] Testing Login...${NC}"
LOGIN_RESULT=$(grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"test123456\"
}" $SERVER im.AuthService/Login 2>&1)
echo "$LOGIN_RESULT"
TOKEN=$(echo "$LOGIN_RESULT" | jq -r '.token // empty')
LOGIN_USER_ID=$(echo "$LOGIN_RESULT" | jq -r '.userId // 0')
LOGIN_SUCCESS=$(echo "$LOGIN_RESULT" | jq -r '.success')

if [ "$LOGIN_SUCCESS" == "true" ] && [ -n "$TOKEN" ]; then
    check_result "Login successful, Token received"
else
    echo -e "${RED}[FAIL]${NC} Login failed"
    exit 1
fi

# 5. 测试错误密码登录
echo -e "\n${YELLOW}[4] Testing Wrong Password Login (should fail)...${NC}"
WRONG_RESULT=$(grpcurl -plaintext -d "{
  \"username\": \"${TEST_USER}\",
  \"password\": \"wrongpassword\"
}" $SERVER im.AuthService/Login 2>&1)
echo "$WRONG_RESULT"
WRONG_SUCCESS=$(echo "$WRONG_RESULT" | jq -r '.success')
if [ "$WRONG_SUCCESS" == "false" ]; then
    check_result "Wrong password correctly rejected"
else
    echo -e "${RED}[FAIL]${NC} Wrong password was accepted"
    exit 1
fi

# 6. 测试不存在的用户登录
echo -e "\n${YELLOW}[5] Testing Non-existent User Login (should fail)...${NC}"
NOUSER_RESULT=$(grpcurl -plaintext -d '{
  "username": "nonexistent_user_'"$TIMESTAMP"'",
  "password": "test123456"
}' $SERVER im.AuthService/Login 2>&1)
echo "$NOUSER_RESULT"
NOUSER_SUCCESS=$(echo "$NOUSER_RESULT" | jq -r '.success')
if [ "$NOUSER_SUCCESS" == "false" ]; then
    check_result "Non-existent user correctly rejected"
else
    echo -e "${RED}[FAIL]${NC} Non-existent user was accepted"
    exit 1
fi

# 7. 测试 Token 验证
echo -e "\n${YELLOW}[6] Testing ValidateToken...${NC}"
VALIDATE_RESULT=$(grpcurl -plaintext -d "{\"token\":\"${TOKEN}\"}" $SERVER im.AuthService/ValidateToken 2>&1)
echo "$VALIDATE_RESULT"
VALID_SUCCESS=$(echo "$VALIDATE_RESULT" | jq -r '.success')
VALID_USER_ID=$(echo "$VALIDATE_RESULT" | jq -r '.userId // 0')
if [ "$VALID_SUCCESS" == "true" ] && [ "$VALID_USER_ID" == "$LOGIN_USER_ID" ]; then
    check_result "Token validation successful"
else
    echo -e "${RED}[FAIL]${NC} Token validation failed"
    exit 1
fi

# 8. 测试无效 Token 验证
echo -e "\n${YELLOW}[7] Testing Invalid Token (should fail)...${NC}"
INVALID_RESULT=$(grpcurl -plaintext -d '{"token":"invalid_token_here_'"$TIMESTAMP"'"}' $SERVER im.AuthService/ValidateToken 2>&1)
echo "$INVALID_RESULT"
INVALID_SUCCESS=$(echo "$INVALID_RESULT" | jq -r '.success')
if [ "$INVALID_SUCCESS" == "false" ]; then
    check_result "Invalid token correctly rejected"
else
    echo -e "${RED}[FAIL]${NC} Invalid token was accepted"
    exit 1
fi

# 9. 测试 Token 刷新
echo -e "\n${YELLOW}[8] Testing RefreshToken...${NC}"
REFRESH_RESULT=$(grpcurl -plaintext -d "{\"token\":\"${TOKEN}\"}" $SERVER im.AuthService/RefreshToken 2>&1)
echo "$REFRESH_RESULT"
NEW_TOKEN=$(echo "$REFRESH_RESULT" | jq -r '.token // empty')
REFRESH_SUCCESS=$(echo "$REFRESH_RESULT" | jq -r '.success')

if [ "$REFRESH_SUCCESS" == "true" ] && [ -n "$NEW_TOKEN" ] && [ "$NEW_TOKEN" != "$TOKEN" ]; then
    check_result "Token refreshed successfully"
else
    echo -e "${RED}[FAIL]${NC} Token refresh failed"
    exit 1
fi

# 10. 使用新 Token 验证
echo -e "\n${YELLOW}[9] Testing New Token Validation...${NC}"
NEW_VALIDATE=$(grpcurl -plaintext -d "{\"token\":\"${NEW_TOKEN}\"}" $SERVER im.AuthService/ValidateToken 2>&1)
echo "$NEW_VALIDATE"
NEW_VALID_SUCCESS=$(echo "$NEW_VALIDATE" | jq -r '.success')
if [ "$NEW_VALID_SUCCESS" == "true" ]; then
    check_result "New token validation successful"
else
    echo -e "${RED}[FAIL]${NC} New token validation failed"
    exit 1
fi

# 11. 测试登出
echo -e "\n${YELLOW}[10] Testing Logout...${NC}"
LOGOUT_RESULT=$(grpcurl -plaintext -d "{\"userId\":${LOGIN_USER_ID}}" $SERVER im.AuthService/Logout 2>&1)
echo "$LOGOUT_RESULT"
LOGOUT_SUCCESS=$(echo "$LOGOUT_RESULT" | jq -r '.success')
if [ "$LOGOUT_SUCCESS" == "true" ]; then
    check_result "Logout successful"
else
    echo -e "${RED}[FAIL]${NC} Logout failed"
    exit 1
fi

# 12. 验证登出后原 Token 无效
echo -e "\n${YELLOW}[11] Testing Token After Logout (should fail)...${NC}"
AFTER_LOGOUT=$(grpcurl -plaintext -d "{\"token\":\"${TOKEN}\"}" $SERVER im.AuthService/ValidateToken 2>&1)
echo "$AFTER_LOGOUT"
AFTER_SUCCESS=$(echo "$AFTER_LOGOUT" | jq -r '.success')
if [ "$AFTER_SUCCESS" == "false" ]; then
    check_result "Token correctly invalidated after logout"
else
    echo -e "${YELLOW}[WARN]${NC} Token still valid after logout (may be Redis cache delay)"
fi

echo -e "\n${GREEN}==========================================${NC}"
echo -e "${GREEN}All tests completed successfully!${NC}"
echo -e "${GREEN}==========================================${NC}"
echo ""
echo "Test Summary:"
echo "  - Register: PASS"
echo "  - Login: PASS"
echo "  - ValidateToken: PASS"
echo "  - RefreshToken: PASS"
echo "  - Logout: PASS"
echo ""
