import 'package:flutter/material.dart';
import 'package:flutter_ble_lib/flutter_ble_lib.dart';

class BluetoothProvider extends ChangeNotifier {
  final BleManager bleManager = BleManager();

  BluetoothState _bluetoothState = BluetoothState.UNKNOWN;
  BluetoothState get getBluetoothState => _bluetoothState;

  bool isPeripheralConnected = false;
  bool get getPeripheralConnected => isPeripheralConnected;

  Future<bool> isBluetoothOn() async {
    print("Bluetooth state...");
    _bluetoothState = await bleManager.bluetoothState();

    if (getBluetoothState == BluetoothState.POWERED_ON) {
      return true;
    }

    return false;
  }

  Future<bool> connectToPeripheral() async {
    bleManager.startPeripheralScan().listen((scanResult) async {
      print(
          "Scanned Peripheral ${scanResult.peripheral.name}, RSSI ${scanResult.rssi}");
      if (scanResult.peripheral.name == "SOLARSAIL") {
        bleManager.stopPeripheralScan();
        Future.delayed(Duration(seconds: 2));

        // Create peripheral
        Peripheral peripheral = scanResult.peripheral;

        isPeripheralConnected = await peripheral.isConnected();

        print("Peripheral connected");
      }
    });

    return false;
  }
}
