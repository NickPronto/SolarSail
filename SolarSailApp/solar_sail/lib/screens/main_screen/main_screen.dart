import 'package:flutter/material.dart';
import 'package:solar_sail/screens/main_screen/components/body.dart';

class MainScreen extends StatelessWidget {
  const MainScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Body(),
      backgroundColor: Color.fromARGB(255, 229, 229, 229),
    );
  }
}
