import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:ourchat_flutter/ui/navigation/route_names.dart';

class ChatListScreen extends ConsumerWidget {
  const ChatListScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final conversations = [
      {'id': '1', 'name': 'John Doe', 'lastMessage': 'Hey, how are you?', 'time': '10:30 AM', 'unread': 2},
      {'id': '2', 'name': 'Development Team', 'lastMessage': 'Sprint planning at 2pm', 'time': '9:15 AM', 'unread': 5},
      {'id': '3', 'name': 'Alice Smith', 'lastMessage': 'Thanks for the help!', 'time': 'Yesterday', 'unread': 0},
    ];

    return Scaffold(
      appBar: AppBar(
        title: const Text('Chats'),
        actions: [
          IconButton(icon: const Icon(Icons.search), onPressed: () {}),
          IconButton(icon: const Icon(Icons.more_vert), onPressed: () {}),
        ],
      ),
      drawer: _buildDrawer(context),
      body: ListView.separated(
        itemCount: conversations.length,
        separatorBuilder: (_, __) => const Divider(height: 1),
        itemBuilder: (context, index) {
          final chat = conversations[index];
          return ListTile(
            leading: CircleAvatar(
              child: Text((chat['name'] as String)[0]),
            ),
            title: Row(
              children: [
                Expanded(child: Text(chat['name'] as String, style: const TextStyle(fontWeight: FontWeight.bold))),
                Text(chat['time'] as String, style: const TextStyle(fontSize: 12, color: Colors.grey)),
              ],
            ),
            subtitle: Row(
              children: [
                Expanded(
                  child: Text(
                    chat['lastMessage'] as String,
                    maxLines: 1,
                    overflow: TextOverflow.ellipsis,
                  ),
                ),
                if ((chat['unread'] as int) > 0)
                  Container(
                    padding: const EdgeInsets.all(6),
                    decoration: const BoxDecoration(color: Colors.blue, shape: BoxShape.circle),
                    child: Text(
                      '${chat['unread']}',
                      style: const TextStyle(color: Colors.white, fontSize: 10),
                    ),
                  ),
              ],
            ),
            onTap: () => context.go('${RouteNames.chatDetail}?peer_id=${chat['id']}&peer_name=${chat['name']}'),
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {},
        child: const Icon(Icons.edit),
      ),
    );
  }

  Widget _buildDrawer(BuildContext context) {
    return Drawer(
      child: ListView(
        children: [
          const UserAccountsDrawerHeader(
            decoration: BoxDecoration(color: Colors.blue),
            accountName: Text('Current User'),
            accountEmail: Text('user@ourchat.com'),
            currentAccountPicture: CircleAvatar(child: Icon(Icons.person)),
          ),
          ListTile(
            leading: const Icon(Icons.chat),
            title: const Text('Chats'),
            selected: true,
            onTap: () => Navigator.of(context).pop(),
          ),
          ListTile(
            leading: const Icon(Icons.people),
            title: const Text('Contacts'),
            onTap: () => context.go(RouteNames.contacts),
          ),
          ListTile(
            leading: const Icon(Icons.group),
            title: const Text('Groups'),
            onTap: () => context.go(RouteNames.groupList),
          ),
          const Divider(),
          ListTile(
            leading: const Icon(Icons.person),
            title: const Text('Profile'),
            onTap: () => context.go(RouteNames.profile),
          ),
          ListTile(
            leading: const Icon(Icons.settings),
            title: const Text('Settings'),
            onTap: () => context.go(RouteNames.settings),
          ),
        ],
      ),
    );
  }
}
