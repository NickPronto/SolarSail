import 'package:flutter/material.dart';
import 'package:flutter_ble_lib/flutter_ble_lib.dart';

class SolarBluetooth {
  final BleManager bleManager = BleManager();

  late final ScanResult scanResult;

  late final Peripheral peripheral;

  String DEVICENAME = "SOLARSAIL";
  static const String serviceUuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
  static const String characteristicUuid =
      "beb5483e-36e1-4688-b7f5-ea07361b26a8";

  // constructor

  void beginBLE() async {
    print("Start BLE");
    BluetoothState currentState = await bleManager.bluetoothState();

    if (currentState != BluetoothState.POWERED_ON) {
      await bleManager.createClient();
      bleManager.enableRadio();
    }

    findDevice();
  }

  void findDevice() async {
    bleManager.observeBluetoothState().listen((event) {
      print(event);
      bleManager.startPeripheralScan().listen((scanResult) async {
        // Scan for peripherals
        if (scanResult.peripheral.name != null) {
          print(
              "Scanned Peripheral ${scanResult.peripheral.name}, RSSI ${scanResult.rssi}");
        }

        if (scanResult.peripheral.name == DEVICENAME) {
          print("Solar Sail found!");
          bleManager.stopPeripheralScan();
          peripheral = scanResult.peripheral;

          connectToDevice();
        }
      });
    });
  }

  void connectToDevice() async {
    await peripheral.connect();
    peripheral
        .observeConnectionState(
            emitCurrentValue: true, completeOnDisconnect: true)
        .listen((connectionState) {
      print(
          "Peripheral ${peripheral.name} connection state is $connectionState");
    });
    print("Connecting to device");
    await peripheral.isConnected();
    print("Connected to device");
    /* setState(() {
                _isConnected = true;
              }); */
    await peripheral.discoverAllServicesAndCharacteristics();

    findCharacteristics();
    readCharacteristic(serviceUuid, characteristicUuid);
  }

  void writeToCharacteristic() async {}

  void readCharacteristic(String service, String characteristic) async {
    CharacteristicWithValue text =
        await peripheral.readCharacteristic(service, characteristic);
    print(text.toString());
  }

  void findCharacteristics() async {
    print("searching characteristics at $serviceUuid");
    List<Characteristic> characteristic =
        await peripheral.characteristics(serviceUuid);
    print(characteristic);
  }
}

class BluetoothProvider extends ChangeNotifier {
  final BleManager bleManager = BleManager();

  Future<BluetoothState> _getBluetoothState() async {
    return await bleManager.bluetoothState();
  }

  void beginBLE() async {
    print("Start BLE");

    if (_getBluetoothState() != BluetoothState.POWERED_ON) {
      await bleManager.createClient();
      bleManager.enableRadio();
    }

    notifyListeners();
  }
}
