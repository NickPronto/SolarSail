import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:provider/provider.dart';
import 'package:solar_sail/components/bluetooth/ble_device_connector.dart';
import 'package:solar_sail/components/bluetooth/ble_device_interactor.dart';
import 'package:solar_sail/components/bluetooth/ble_logger.dart';
import 'package:solar_sail/components/bluetooth/ble_scanner.dart';
import 'package:solar_sail/components/bluetooth/ble_status_monitor.dart';
import 'package:solar_sail/screens/ble_status_screen.dart';
import 'package:solar_sail/screens/connect_screen/connect_screen.dart';
import 'package:solar_sail/screens/main_screen/main_screen.dart';
import 'package:solar_sail/screens/test_screen/test_screen.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  final _ble = FlutterReactiveBle();
  final _bleLogger = BleLogger();

  final _scanner = BleScanner(ble: _ble, logMessage: _bleLogger.addToLog);

  final _monitor = BleStatusMonitor(_ble);
  final _connector = BleDeviceConnector(
    ble: _ble,
    logMessage: _bleLogger.addToLog,
  );
  final _serviceDiscoverer = BleDeviceInteractor(
    bleDiscoverServices: _ble.discoverServices,
    readCharacteristic: _ble.readCharacteristic,
    writeWithResponse: _ble.writeCharacteristicWithResponse,
    writeWithOutResponse: _ble.writeCharacteristicWithoutResponse,
    subscribeToCharacteristic: _ble.subscribeToCharacteristic,
    logMessage: _bleLogger.addToLog,
  );

  runApp(MultiProvider(
    providers: [
      Provider.value(value: _scanner),
      Provider.value(value: _monitor),
      Provider.value(value: _connector),
      Provider.value(value: _serviceDiscoverer),
      Provider.value(value: _bleLogger),
      StreamProvider<BleScannerState?>(
          create: (_) => _scanner.state,
          initialData: const BleScannerState(
              discoveredDevices: [], scanIsInProgress: false)),
      StreamProvider<BleStatus?>(
          create: (_) => _monitor.state, initialData: BleStatus.unknown),
      StreamProvider<ConnectionStateUpdate>(
          create: (_) => _connector.state,
          initialData: const ConnectionStateUpdate(
              deviceId: 'Unknown device',
              connectionState: DeviceConnectionState.disconnected,
              failure: null))
    ],
    child: MaterialApp(
      title: "ESP32 controller",
      theme: ThemeData(primaryColor: Colors.white),
      debugShowCheckedModeBanner: false,
      home: const MyApp(),
    ),
  ));
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) =>
      Consumer<BleStatus?>(builder: (_, status, __) {
        if (status == BleStatus.ready) {
          return ConnectScreen();
        } else {
          return BleStatusScreen(status: BleStatus.unknown);
        }
      });
}
