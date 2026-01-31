import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:ourchat_flutter/ui/screens/auth/login_screen.dart';
import 'package:ourchat_flutter/ui/screens/auth/register_screen.dart';
import 'package:ourchat_flutter/ui/screens/auth/splash_screen.dart';
import 'package:ourchat_flutter/ui/screens/chat/chat_detail_screen.dart';
import 'package:ourchat_flutter/ui/screens/chat/chat_list_screen.dart';
import 'package:ourchat_flutter/ui/screens/contacts/contacts_screen.dart';
import 'package:ourchat_flutter/ui/screens/profile/profile_screen.dart';
import 'package:ourchat_flutter/ui/screens/settings/settings_screen.dart';
import 'route_names.dart';

final routerProvider = Provider<GoRouter>((ref) {
  return GoRouter(
    initialLocation: RouteNames.splash,
    routes: [
      GoRoute(
        path: RouteNames.splash,
        builder: (context, state) => const SplashScreen(),
      ),
      GoRoute(
        path: RouteNames.login,
        builder: (context, state) => const LoginScreen(),
      ),
      GoRoute(
        path: RouteNames.register,
        builder: (context, state) => const RegisterScreen(),
      ),
      GoRoute(
        path: RouteNames.chatList,
        builder: (context, state) => const ChatListScreen(),
      ),
      GoRoute(
        path: RouteNames.chatDetail,
        builder: (context, state) {
          final peerId = state.uri.queryParameters['peer_id'] ?? '';
          final peerName = state.uri.queryParameters['peer_name'] ?? '';
          return ChatDetailScreen(peerId: peerId, peerName: peerName);
        },
      ),
      GoRoute(
        path: RouteNames.contacts,
        builder: (context, state) => const ContactsScreen(),
      ),
      GoRoute(
        path: RouteNames.profile,
        builder: (context, state) => const ProfileScreen(),
      ),
      GoRoute(
        path: RouteNames.settings,
        builder: (context, state) => const SettingsScreen(),
      ),
    ],
  );
});
