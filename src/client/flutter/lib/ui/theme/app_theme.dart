import 'package:flutter/material.dart';

final lightTheme = ThemeData(
  useMaterial3: true,
  colorScheme: ColorScheme.fromSeed(
    seedColor: const Color(0xFF2196F3),
    primary: const Color(0xFF2196F3),
    secondary: const Color(0xFF03DAC6),
    surface: const Color(0xFFFFFFFF),
    background: const Color(0xFFF5F5F5),
  ),
  appBarTheme: const AppBarTheme(
    backgroundColor: Color(0xFF2196F3),
    foregroundColor: Colors.white,
  ),
  scaffoldBackgroundColor: const Color(0xFFF5F5F5),
);

final darkTheme = ThemeData(
  useMaterial3: true,
  colorScheme: ColorScheme.fromSeed(
    seedColor: const Color(0xFF2196F3),
    primary: const Color(0xFF90CAF9),
    secondary: const Color(0xFF03DAC6),
    surface: const Color(0xFF1E1E1E),
    background: const Color(0xFF121212),
    onPrimary: Colors.black,
  ),
  appBarTheme: const AppBarTheme(
    backgroundColor: Color(0xFF1E1E1E),
    foregroundColor: Colors.white,
  ),
  scaffoldBackgroundColor: const Color(0xFF121212),
);
