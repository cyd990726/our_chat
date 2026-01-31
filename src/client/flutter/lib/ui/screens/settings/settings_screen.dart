import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:ourchat_flutter/ui/navigation/route_names.dart';

class SettingsScreen extends ConsumerWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: ListView(
        children: [
          const Padding(padding: EdgeInsets.all(16), child: Text('Account')),
          ListTile(
            leading: const Icon(Icons.lock, color: Colors.blue),
            title: const Text('Privacy'),
            subtitle: const Text('Manage privacy settings'),
            onTap: () {},
          ),
          ListTile(
            leading: const Icon(Icons.notifications, color: Colors.orange),
            title: const Text('Notifications'),
            trailing: Switch(value: true, onChanged: (value) {}),
          ),
          const Divider(),
          const Padding(padding: EdgeInsets.all(16), child: Text('Appearance')),
          ListTile(
            leading: const Icon(Icons.dark_mode, color: Colors.purple),
            title: const Text('Dark Mode'),
            trailing: Switch(value: false, onChanged: (value) {}),
          ),
          ListTile(
            leading: const Icon(Icons.language, color: Colors.green),
            title: const Text('Language'),
            subtitle: const Text('English'),
            onTap: () {},
          ),
          const Divider(),
          const Padding(padding: EdgeInsets.all(16), child: Text('About')),
          ListTile(
            leading: const Icon(Icons.info, color: Colors.blue),
            title: const Text('About OurChat'),
            subtitle: const Text('Version 1.0.0'),
            onTap: () {},
          ),
          ListTile(
            leading: const Icon(Icons.help, color: Colors.cyan),
            title: const Text('Help & Support'),
            onTap: () {},
          ),
          const Divider(),
          ListTile(
            leading: const Icon(Icons.logout, color: Colors.red),
            title: const Text('Logout'),
            onTap: () => _showLogoutDialog(context),
          ),
        ],
      ),
    );
  }

  void _showLogoutDialog(BuildContext context) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Logout'),
        content: const Text('Are you sure you want to logout?'),
        actions: [
          TextButton(onPressed: () => Navigator.of(context).pop(), child: const Text('Cancel')),
          TextButton(
            onPressed: () => context.go('/login'),
            child: const Text('Logout', style: TextStyle(color: Colors.red)),
          ),
        ],
      ),
    );
  }
}
