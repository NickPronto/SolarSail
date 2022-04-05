import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:solar_sail/components/bluetooth/bluetooth.dart';
import 'package:solar_sail/screens/main_screen/main_screen.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (context) => BluetoothProvider(),
      child: MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: ThemeData(
          primarySwatch: Colors.grey,
        ),
        home: const MainScreen(),
      ),
    );
  }
}
