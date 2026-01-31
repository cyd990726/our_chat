import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:ourchat_flutter/ui/navigation/route_names.dart';

class ProfileScreen extends ConsumerWidget {
  const ProfileScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return Scaffold(
      appBar: AppBar(title: const Text('Profile')),
      body: SingleChildScrollView(
        child: Column(
          children: [
            const SizedBox(height: 20),
            const Center(
              child: CircleAvatar(
                radius: 60,
                child: Icon(Icons.person, size: 60),
              ),
            ),
            const SizedBox(height: 16),
            const Text('Current User', style: TextStyle(fontSize: 24, fontWeight: FontWeight.bold)),
            const Text('user@ourchat.com', style: TextStyle(color: Colors.grey)),
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: () {},
              style: ElevatedButton.styleFrom(
                minimumSize: const Size(double.infinity, 50),
              ),
              child: const Text('Edit Profile'),
            ),
            const SizedBox(height: 20),
            const Divider(),
            _buildProfileItem('Phone', '+1 234 567 8900', Icons.phone),
            _buildProfileItem('Bio', 'Hey there! I am using OurChat', Icons.info),
            _buildProfileItem('Joined', 'January 2024', Icons.calendar_today),
          ],
        ),
      ),
    );
  }

  Widget _buildProfileItem(String label, String value, IconData icon) {
    return ListTile(
      leading: Icon(icon, color: Colors.blue),
      title: Text(label),
      subtitle: Text(value),
      onTap: () {},
    );
  }
}
