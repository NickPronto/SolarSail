// ignore_for_file: constant_identifier_names

import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:flutter_switch/flutter_switch.dart';
import 'package:font_awesome_flutter/font_awesome_flutter.dart';
import 'package:provider/provider.dart';
import 'package:solar_sail/components/bluetooth/bluetooth.dart';

class Body extends StatefulWidget {
  const Body({Key? key}) : super(key: key);

  @override
  State<Body> createState() => _BodyState();
}

class _BodyState extends State<Body> {
  bool _manualOn = false;
  bool _isConnected = false;
  int _alpha = 255;

  static const String DEVICENAME = "SOLARSAIL";

  static const String serviceUuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
  static const String characteristicUuid =
      "beb5483e-36e1-4688-b7f5-ea07361b26a8";

  final _ble = FlutterReactiveBle();

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
          color: Color.fromARGB(_manualOn ? 50 : 255, 255, 255, 255),
          border: Border.all(
              width: 2,
              color: Color.fromARGB(_manualOn ? 50 : 255, 65, 65, 65))),
      child: IconButton(
        color: Color.fromARGB(_manualOn ? 50 : 255, 0, 0, 0),
        icon: icon,
        onPressed: () {
          print("Pressed button");
          // _ble.writeCharacteristicWithoutResponse(QualifiedCharacteristic(characteristicId: Uuid.parse(characteristicUuid), serviceId: Uuid.parse(serviceUuid), deviceId: deviceId), value: 1);
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
      decoration: BoxDecoration(
          shape: BoxShape.circle,
          color: false
              ? Color.fromARGB(255, 40, 122, 169)
              : Color.fromARGB(255, 175, 60, 60)),
      child: IconButton(
        color: Colors.white,
        icon: false ? Icon(Icons.bluetooth) : Icon(Icons.bluetooth_disabled),
        onPressed: () {
          //solarBluetooth.beginBLE();

          _ble.scanForDevices(withServices: [Uuid.parse(serviceUuid)]).listen(
              (device) {
            print(device.id);
            // _ble.connectToDevice(id: device.id).listen((connectionState) {});
          });
        },
        iconSize: 40,
      ),
    );
  }
}
