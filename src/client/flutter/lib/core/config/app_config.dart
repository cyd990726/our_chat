import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:grpc/grpc.dart';
import 'package:shared_preferences/shared_preferences.dart';

class AppConfig {
  static AppConfig? _instance;
  late SharedPreferences _prefs;

  String get grpcHost => _prefs.getString('grpc_host') ?? 'localhost';
  set grpcHost(String value) => _prefs.setString('grpc_host', value);

  int get grpcPort => _prefs.getInt('grpc_port') ?? 50001;
  set grpcPort(int value) => _prefs.setInt('grpc_port', value);

  String get wsUrl => _prefs.getString('ws_url') ?? 'ws://localhost:50003';
  set wsUrl(String value) => _prefs.setString('ws_url', value);

  bool get isDarkMode => _prefs.getBool('dark_mode') ?? false;
  set isDarkMode(bool value) => _prefs.setBool('dark_mode', value);

  String get currentUserId => _prefs.getString('current_user_id') ?? '';
  set currentUserId(String value) => _prefs.setString('current_user_id', value);

  String get languageCode => _prefs.getString('language_code') ?? 'zh';
  set languageCode(String value) => _prefs.setString('language_code', value);

  factory AppConfig.getInstance() {
    return _instance ??= AppConfig._();
  }

  AppConfig._();

  Future<void> initialize() async {
    _prefs = await SharedPreferences.getInstance();
  }

  String getGrpcAddress() {
    final isSecure = grpcPort == 443;
    return '${isSecure ? 'https' : 'http'}://$grpcHost:$grpcPort';
  }

  static final appConfigProvider = FutureProvider<AppConfig>((ref) async {
    final config = AppConfig.getInstance();
    await config.initialize();
    return config;
  });
}

class GrpcConfig {
  static const int defaultAuthPort = 50001;
  static const int defaultMessagePort = 50004;
  static const int defaultGroupPort = 50005;
  static const int defaultSessionPort = 50002;
  static const int defaultPresencePort = 50006;

  static String getAuthHost() {
    final config = AppConfig.getInstance();
    return config.grpcHost;
  }

  static int getAuthPort() {
    final config = AppConfig.getInstance();
    return config.grpcPort;
  }

  static ChannelCredentials getChannelCredentials() {
    final port = getAuthPort();
    if (port == 443) {
      return ChannelCredentials.secure();
    }
    return ChannelCredentials.insecure();
  }

  static Duration getConnectionTimeout() {
    return const Duration(seconds: 10);
  }

  static Duration getIdleTimeout() {
    return const Duration(minutes: 5);
  }
}

final appConfigProvider = FutureProvider<AppConfig>((ref) async {
  final config = AppConfig.getInstance();
  await config.initialize();
  return config;
});
