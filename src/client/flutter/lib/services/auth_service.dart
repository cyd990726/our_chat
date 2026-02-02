import 'dart:async';
import 'dart:io';

import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:ourchat_flutter/core/auth/auth_manager.dart';
import 'package:ourchat_flutter/core/network/grpc_client.dart';
import 'package:uuid/uuid.dart';

class AuthService {
  final GrpcClient _grpcClient;
  final AuthManager _authManager;
  final String _deviceId;

  AuthService(
      {required GrpcClient grpcClient, required AuthManager authManager})
      : _grpcClient = grpcClient,
        _authManager = authManager,
        _deviceId = '${Platform.operatingSystem}-${const Uuid().v4()}';

  Future<AuthGrpcResult<LoginResult>> login({
    required String username,
    required String password,
  }) async {
    if (username.isEmpty) {
      return AuthGrpcResult.error('Username is required');
    }
    if (password.isEmpty) {
      return AuthGrpcResult.error('Password is required');
    }

    try {
      final request = LoginRequest(
        username: username,
        password: password,
        deviceId: _deviceId,
        deviceType: Platform.operatingSystem,
        clientVersion: '1.0.0',
      );

      final response = await _grpcClient.authClient.login(request);

      if (response.success) {
        final result = LoginResult(
          userId: response.userId,
          token: response.token,
          expireAt: response.expireAt,
        );
        await _authManager.saveTokens(response.token, '');
        await _authManager.saveUserId(response.userId.toString());
        return AuthGrpcResult.success(result);
      } else {
        return AuthGrpcResult.error(
          response.error?.message ?? 'Login failed',
          code: response.error?.code,
        );
      }
    } catch (e) {
      return AuthGrpcResult.error('Network error: ${e.toString()}');
    }
  }

  Future<AuthGrpcResult<RegisterResult>> register({
    required String username,
    required String password,
    required String email,
    required String phone,
    String nickname = '',
  }) async {
    final validationError = _validateRegistrationInput(
      username: username,
      password: password,
      email: email,
      phone: phone,
    );
    if (validationError != null) {
      return AuthGrpcResult.error(validationError);
    }

    try {
      final request = RegisterRequest(
        username: username,
        password: password,
        email: email,
        phone: phone,
        nickname: nickname.isNotEmpty ? nickname : username,
      );

      final response = await _grpcClient.authClient.register(request);

      if (response.success) {
        final result = RegisterResult(userId: response.userId);
        return AuthGrpcResult.success(result);
      } else {
        return AuthGrpcResult.error(
          response.error?.message ?? 'Registration failed',
          code: response.error?.code,
        );
      }
    } catch (e) {
      return AuthGrpcResult.error('Network error: ${e.toString()}');
    }
  }

  Future<AuthGrpcResult<void>> logout() async {
    try {
      final userIdStr = await _authManager.getUserId();
      if (userIdStr == null || userIdStr.isEmpty) {
        await _authManager.clearAuthData();
        return AuthGrpcResult.success(null);
      }

      final userId = int.tryParse(userIdStr) ?? 0;
      final request = LogoutRequest(
        userId: userId,
        deviceId: _deviceId,
      );

      await _grpcClient.authClient.logout(request);
      await _authManager.clearAuthData();
      return AuthGrpcResult.success(null);
    } catch (e) {
      await _authManager.clearAuthData();
      return AuthGrpcResult.success(null);
    }
  }

  Future<bool> isLoggedIn() async {
    final token = await _authManager.getAccessToken();
    if (token == null || token.isEmpty) {
      return false;
    }
    return _authManager.isTokenValid(token);
  }

  Future<String?> getCurrentToken() async {
    return await _authManager.getAccessToken();
  }

  Future<int?> getCurrentUserId() async {
    final userIdStr = await _authManager.getUserId();
    if (userIdStr == null || userIdStr.isEmpty) {
      return null;
    }
    return int.tryParse(userIdStr);
  }

  String? _validateRegistrationInput({
    required String username,
    required String password,
    required String email,
    required String phone,
  }) {
    if (username.isEmpty || username.length < 3) {
      return 'Username must be at least 3 characters';
    }
    if (!RegExp(r'^[a-zA-Z0-9_]+$').hasMatch(username)) {
      return 'Username can only contain letters, numbers, and underscores';
    }
    if (password.isEmpty || password.length < 6) {
      return 'Password must be at least 6 characters';
    }
    if (email.isNotEmpty &&
        !RegExp(r'^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$').hasMatch(email)) {
      return 'Invalid email format';
    }
    if (phone.isNotEmpty && !RegExp(r'^\+?[0-9]{10,15}$').hasMatch(phone)) {
      return 'Invalid phone number format';
    }
    return null;
  }
}

class LoginResult {
  final int userId;
  final String token;
  final int expireAt;

  LoginResult({
    required this.userId,
    required this.token,
    required this.expireAt,
  });
}

class RegisterResult {
  final int userId;

  RegisterResult({required this.userId});
}

final authServiceProvider = Provider<AuthService>((ref) {
  final grpcClient = ref.read(grpcClientProvider);
  final authManager = ref.read(authManagerProvider);
  return AuthService(
    grpcClient: grpcClient,
    authManager: authManager,
  );
});
