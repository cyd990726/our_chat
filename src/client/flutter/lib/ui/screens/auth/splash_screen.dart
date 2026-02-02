import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:ourchat_flutter/core/auth/auth_manager.dart';
import 'package:ourchat_flutter/ui/navigation/route_names.dart';

class SplashScreen extends ConsumerWidget {
  const SplashScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final authManager = ref.read(authManagerProvider);

    Future.delayed(const Duration(seconds: 2), () async {
      final token = await authManager.getAccessToken();
      final isLoggedIn =
          token != null && token.isNotEmpty && authManager.isTokenValid(token);

      if (context.mounted) {
        if (isLoggedIn) {
          context.go(RouteNames.chatList);
        } else {
          context.go(RouteNames.login);
        }
      }
    });

    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Icon(Icons.chat_bubble, size: 80, color: Colors.blue),
            const SizedBox(height: 20),
            const Text(
              'OurChat',
              style: TextStyle(fontSize: 32, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 10),
            const Text('Connecting you with the world'),
            const SizedBox(height: 40),
            const CircularProgressIndicator(),
          ],
        ),
      ),
    );
  }
}
