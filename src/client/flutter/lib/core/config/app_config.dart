import 'package:flutter_riverpod/flutter_riverpod.dart';
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

  static final appConfigProvider = FutureProvider<AppConfig>((ref) async {
    final config = AppConfig.getInstance();
    await config.initialize();
    return config;
  });
}
