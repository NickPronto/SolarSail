// ignore_for_file: constant_identifier_names

import 'package:flutter/material.dart';
import 'package:flutter_switch/flutter_switch.dart';
import 'package:font_awesome_flutter/font_awesome_flutter.dart';
import 'package:flutter_ble_lib/flutter_ble_lib.dart';

class Body extends StatefulWidget {
  const Body({Key? key}) : super(key: key);

  @override
  State<Body> createState() => _BodyState();
}

class _BodyState extends State<Body> {
  bool _manualOn = false;

  BleManager bleManager = BleManager();

  void startBLE() async {
    await bleManager.createClient();
    bleManager.enableRadio();
    BluetoothState currentState = await bleManager.bluetoothState();
    bleManager.observeBluetoothState().listen((event) {
      print(event);

      if (event == BluetoothState.POWERED_ON) {
        bleManager.startPeripheralScan().listen((scanResult) {
          //Scan one peripheral and stop scanning
          print(
              "Scanned Peripheral ${scanResult.peripheral.name}, RSSI ${scanResult.rssi}");
          bleManager.stopPeripheralScan();
        });
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        SizedBox(
          height: MediaQuery.of(context).size.height * 0.05,
        ),
        Center(
          child: bluetoothButton(),
        ),
        SizedBox(
          height: MediaQuery.of(context).size.height * 0.05,
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            lock_container(context, "Left"),
            SizedBox(
              width: MediaQuery.of(context).size.width * 0.2,
            ),
            lock_container(context, "Right")
          ],
        ),
        SizedBox(
          height: MediaQuery.of(context).size.height * 0.05,
        ),
        Center(
          child: autoManualSwitch(context),
        ),
        SizedBox(
          height: MediaQuery.of(context).size.height * 0.1,
        ),
        Center(child: dPadButton(Icon(Icons.pause))),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            dPadButton(Icon(Icons.arrow_back)),
            dPadButton(Icon(Icons.arrow_forward))
          ],
        ),
        Center(child: dPadButton(Icon(Icons.arrow_downward)))
      ],
    );
  }

  Container dPadButton(Widget icon) {
    return Container(
      alignment: Alignment.center,
      padding: const EdgeInsets.all(10),
      decoration: BoxDecoration(
          shape: BoxShape.circle,
          color: Colors.white,
          border: Border.all(width: 2, color: Colors.grey)),
      child: IconButton(
        color: Colors.black,
        icon: icon,
        onPressed: () {
          print("Pressed button");
        },
        iconSize: 40,
      ),
    );
  }

  FlutterSwitch autoManualSwitch(BuildContext context) {
    return FlutterSwitch(
        value: _manualOn,
        showOnOff: true,
        inactiveText: "Manual",
        activeText: "Auto",
        padding: 8,
        inactiveColor: Color.fromARGB(255, 116, 139, 255),
        inactiveTextColor: Colors.white,
        activeColor: Color.fromARGB(255, 70, 154, 78),
        activeTextColor: Colors.white,
        toggleSize: MediaQuery.of(context).size.height * 0.05,
        width: MediaQuery.of(context).size.width * 0.4,
        height: MediaQuery.of(context).size.height * 0.075,
        onToggle: (val) {
          setState(() {
            _manualOn = val;
          });
        });
  }

  Container lock_container(BuildContext context, String text) {
    return Container(
      alignment: Alignment.center,
      padding: const EdgeInsets.fromLTRB(0, 10, 0, 10),
      height: MediaQuery.of(context).size.height * 0.12,
      width: MediaQuery.of(context).size.width * 0.17,
      decoration: BoxDecoration(
          color: Color.fromARGB(255, 75, 75, 75),
          borderRadius: BorderRadius.circular(20)),
      child: Column(
        children: [
          Text(text, style: TextStyle(color: Colors.white, fontSize: 17)),
          SizedBox(
            height: 15,
          ),
          Icon(
            Icons.lock,
            size: 25,
            color: Color.fromARGB(255, 175, 60, 60),
          )
        ],
      ),
    );
  }

  Container bluetoothButton() {
    return Container(
      alignment: Alignment.center,
      padding: const EdgeInsets.all(30),
      decoration: const BoxDecoration(
        shape: BoxShape.circle,
        color: Color.fromARGB(255, 40, 122, 169),
      ),
      child: IconButton(
        color: Colors.white,
        icon: const Icon(Icons.bluetooth),
        onPressed: () {
          startBLE();
        },
        iconSize: 40,
      ),
    );
  }
}
