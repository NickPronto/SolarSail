import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:solar_sail/screens/ble_status_screen.dart';
import 'package:solar_sail/screens/main_screen/components/body.dart';
import 'package:provider/provider.dart';

class MainScreen extends StatelessWidget {
  const MainScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) =>
      Consumer<BleStatus?>(builder: (_, status, __) {
        if (status == BleStatus.ready) {
          return Scaffold(
            body: Body(),
          );
        } else {
          return BleStatusScreen(status: status ?? BleStatus.unknown);
        }
      });
}
