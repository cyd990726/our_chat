import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:ourchat_flutter/ui/navigation/app_router.dart';
import 'package:ourchat_flutter/ui/theme/app_theme.dart';

class OurChatApp extends ConsumerWidget {
  const OurChatApp({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return MaterialApp.router(
      title: 'OurChat',
      debugShowCheckedModeBanner: false,
      routerConfig: ref.watch(routerProvider),
      theme: lightTheme,
      darkTheme: darkTheme,
    );
  }
}
