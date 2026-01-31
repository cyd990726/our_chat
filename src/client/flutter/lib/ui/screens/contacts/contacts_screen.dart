import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:ourchat_flutter/ui/navigation/route_names.dart';

class ContactsScreen extends ConsumerWidget {
  const ContactsScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final contacts = [
      {'id': '1', 'name': 'John Doe', 'status': 'Online'},
      {'id': '2', 'name': 'Alice Smith', 'status': 'Away'},
      {'id': '3', 'name': 'Bob Johnson', 'status': 'Offline'},
      {'id': '4', 'name': 'Carol Williams', 'status': 'Online'},
    ];

    return Scaffold(
      appBar: AppBar(
        title: const Text('Contacts'),
        actions: [
          IconButton(icon: const Icon(Icons.search), onPressed: () {}),
          IconButton(icon: const Icon(Icons.person_add), onPressed: () {}),
        ],
      ),
      body: ListView.builder(
        itemCount: contacts.length,
        itemBuilder: (context, index) {
          final contact = contacts[index];
          return ListTile(
            leading: Stack(
              children: [
                const CircleAvatar(child: Icon(Icons.person)),
                Positioned(
                  bottom: 0,
                  right: 0,
                  child: Container(
                    width: 12,
                    height: 12,
                    decoration: BoxDecoration(
                      color: contact['status'] == 'Online' ? Colors.green : 
                             contact['status'] == 'Away' ? Colors.orange : Colors.grey,
                      shape: BoxShape.circle,
                    ),
                  ),
                ),
              ],
            ),
            title: Text(contact['name'] as String, style: const TextStyle(fontWeight: FontWeight.bold)),
            subtitle: Text(contact['status'] as String),
            trailing: Row(mainAxisSize: MainAxisSize.min, children: [
              IconButton(icon: const Icon(Icons.chat, color: Colors.blue), onPressed: () {}),
              IconButton(icon: const Icon(Icons.call, color: Colors.green), onPressed: () {}),
            ]),
            onTap: () {},
          );
        },
      ),
    );
  }
}
