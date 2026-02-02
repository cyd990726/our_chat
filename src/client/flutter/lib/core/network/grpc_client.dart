import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:grpc/grpc.dart';
import 'package:ourchat_flutter/core/config/app_config.dart';

abstract class AuthServiceClient {
  Future<LoginResponse> login(LoginRequest request);
  Future<RegisterResponse> register(RegisterRequest request);
  Future<LogoutResponse> logout(LogoutRequest request);
}

class LoginRequest {
  final String username;
  final String password;
  final String deviceId;
  final String deviceType;
  final String clientVersion;

  LoginRequest({
    required this.username,
    required this.password,
    required this.deviceId,
    required this.deviceType,
    required this.clientVersion,
  });
}

class LoginResponse {
  bool success = false;
  Error? error;
  int userId = 0;
  String token = '';
  int expireAt = 0;
}

class RegisterRequest {
  final String username;
  final String password;
  final String phone;
  final String email;
  final String nickname;

  RegisterRequest({
    required this.username,
    required this.password,
    required this.phone,
    required this.email,
    required this.nickname,
  });
}

class RegisterResponse {
  bool success = false;
  Error? error;
  int userId = 0;
}

class LogoutRequest {
  final int userId;
  final String deviceId;

  LogoutRequest({required this.userId, required this.deviceId});
}

class LogoutResponse {
  bool success = false;
  Error? error;
}

class Error {
  int code = 0;
  String message = '';
}

class GrpcClient {
  static GrpcClient? _instance;
  ClientChannel? _channel;
  AuthServiceClient? _authClient;

  GrpcClient._internal() {
    _initializeClient();
  }

  void _initializeClient() {
    final config = AppConfig.getInstance();
    final host = config.grpcHost;
    final port = config.grpcPort;

    _channel = ClientChannel(
      host,
      port: port,
      options: ChannelOptions(
        credentials: _getChannelCredentials(port),
        idleTimeout: const Duration(minutes: 5),
        connectionTimeout: const Duration(seconds: 10),
      ),
    );
  }

  ChannelCredentials _getChannelCredentials(int port) {
    if (port == 443) {
      return ChannelCredentials.secure();
    }
    return ChannelCredentials.insecure();
  }

  factory GrpcClient.getInstance() {
    _instance ??= GrpcClient._internal();
    return _instance!;
  }

  AuthServiceClient get authClient {
    return _authClient ?? _MockAuthServiceClient();
  }

  Future<void> shutdown() async {
    await _channel?.shutdown();
    _channel = null;
    _authClient = null;
    _instance = null;
  }

  ClientChannel? get channel => _channel;
}

class _MockAuthServiceClient implements AuthServiceClient {
  @override
  Future<LoginResponse> login(LoginRequest request) async {
    await Future.delayed(const Duration(milliseconds: 500));

    if (request.username == 'test' && request.password == '123456') {
      return LoginResponse()
        ..success = true
        ..userId = 1
        ..token = 'mock_jwt_token_${DateTime.now().millisecondsSinceEpoch}'
        ..expireAt =
            DateTime.now().add(const Duration(days: 7)).millisecondsSinceEpoch;
    }

    return LoginResponse()
      ..success = false
      ..error = (Error()
        ..code = 401
        ..message = 'Invalid username or password');
  }

  @override
  Future<RegisterResponse> register(RegisterRequest request) async {
    await Future.delayed(const Duration(milliseconds: 500));

    if (request.username.isEmpty || request.password.isEmpty) {
      return RegisterResponse()
        ..success = false
        ..error = (Error()
          ..code = 400
          ..message = 'Username and password are required');
    }

    if (request.password.length < 6) {
      return RegisterResponse()
        ..success = false
        ..error = (Error()
          ..code = 400
          ..message = 'Password must be at least 6 characters');
    }

    return RegisterResponse()
      ..success = true
      ..userId = DateTime.now().millisecondsSinceEpoch % 10000;
  }

  @override
  Future<LogoutResponse> logout(LogoutRequest request) async {
    await Future.delayed(const Duration(milliseconds: 200));
    return LogoutResponse()..success = true;
  }
}

class AuthGrpcResult<T> {
  final bool success;
  final T? data;
  final String? errorMessage;
  final int? errorCode;

  AuthGrpcResult({
    required this.success,
    this.data,
    this.errorMessage,
    this.errorCode,
  });

  factory AuthGrpcResult.success(T data) {
    return AuthGrpcResult(success: true, data: data);
  }

  factory AuthGrpcResult.error(String message, {int? code}) {
    return AuthGrpcResult(
      success: false,
      errorMessage: message,
      errorCode: code,
    );
  }

  bool get hasError => !success;

  T getOrThrow() {
    if (hasError) {
      throw AuthException(errorMessage ?? 'Unknown error', errorCode);
    }
    return data!;
  }
}

class AuthException implements Exception {
  final String message;
  final int? code;

  AuthException(this.message, [this.code]);

  @override
  String toString() =>
      'AuthException: $message${code != null ? ' (code: $code)' : ''}';
}

final grpcClientProvider = Provider<GrpcClient>((ref) {
  return GrpcClient.getInstance();
});
