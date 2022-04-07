import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:flutter_switch/flutter_switch.dart';
import 'package:solar_sail/components/bluetooth/ble_device_connector.dart';
import 'package:solar_sail/components/bluetooth/ble_device_interactor.dart';
import 'package:solar_sail/components/bluetooth/ble_scanner.dart';
import 'package:solar_sail/components/bluetooth_char/bluetooth_characteristics.dart';
import 'package:solar_sail/constants.dart';
import 'package:solar_sail/screens/ble_status_screen.dart';
import 'package:provider/provider.dart';
import 'package:solar_sail/screens/main_screen/main_screen.dart';

class ConnectScreen extends StatelessWidget {
  const ConnectScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) => Consumer5<BleScanner, BleScannerState?,
          BleDeviceConnector, ConnectionStateUpdate, BleDeviceInteractor>(
        builder: (_, bleScanner, bleScannerState, bleConnectorState,
                bleConnectionUpdate, bleDeviceInteractor, __) =>
            Body(
          scannerState: bleScannerState ??
              const BleScannerState(
                  discoveredDevices: [], scanIsInProgress: false),
          startScan: bleScanner.startScan,
          stopScan: bleScanner.stopScan,
          bleDeviceConnector: bleConnectorState,
          bleConnectorUpdate: bleConnectionUpdate,
          bleDeviceInteractor: bleDeviceInteractor,
        ),
      );
}

@immutable
class Body extends StatefulWidget {
  Body(
      {Key? key,
      required this.scannerState,
      required this.startScan,
      required this.stopScan,
      required this.bleDeviceConnector,
      required this.bleConnectorUpdate,
      required this.bleDeviceInteractor})
      : super(key: key);

  final BleScannerState scannerState;
  final void Function(List<Uuid>) startScan;
  final VoidCallback stopScan;
  final BleDeviceConnector bleDeviceConnector;
  final ConnectionStateUpdate bleConnectorUpdate;
  final BleDeviceInteractor bleDeviceInteractor;
  String connectedDeviceId = "";

  bool _manualOn = false;
  bool _isConnecting = false;
  int _alpha = 255;

  bool get deviceConnected =>
      bleConnectorUpdate.connectionState == DeviceConnectionState.connected;

  @override
  State<Body> createState() => _BodyState();
}

class _BodyState extends State<Body> {
  @override
  void initState() {
    super.initState();
  }

  @override
  void dispose() {
    widget.stopScan();
    super.dispose();
  }

  void _stopScanning() {
    print("Stop scanning");
    widget.stopScan();
  }

  void _startScanning() {
    print("Start scanning");
    widget.startScan([
      //Uuid.parse(serviceUuid)
    ]); // Start empty scan for any device...will do matching for device name later
  }

  // TODO: Add feature
  void _scanAndConnect() {}

  void _connect(String deviceId) {
    print("Connecting to device");
    _stopScanning();
    setState(() {
      widget._isConnecting = true;
    });

    widget.bleDeviceConnector.connect(deviceId);
    widget.connectedDeviceId = deviceId;
  }

  void _disconnect() {
    print("Disconnect from device");
    widget.bleDeviceConnector.disconnect(widget.connectedDeviceId);
  }

  Icon _getIconState(bool scanInProgress, bool connected) {
    if ((scanInProgress && !connected) || widget._isConnecting) {
      return Icon(Icons.bluetooth_searching);
    } else if (connected) {
      setState(() {
        widget._isConnecting = false;
      });
      return Icon(Icons.bluetooth_connected);
    } else {
      return Icon(Icons.bluetooth_disabled);
    }
  }

  Function? _getBluetoothButtonFunction() {
    if (!widget.deviceConnected && !widget.scannerState.scanIsInProgress) {
      _startScanning();
    } else if (!widget.deviceConnected &&
        widget.scannerState.scanIsInProgress) {
      _stopScanning();
    } else if (widget.bleConnectorUpdate.connectionState ==
            DeviceConnectionState.connected &&
        !widget.scannerState.scanIsInProgress) {
      _disconnect();
    }
  }

  Color _getColorState(bool scanInProgress, bool connected) {
    if ((scanInProgress && !connected) || widget._isConnecting) {
      return greenColor;
    } else if (connected) {
      return blueColor;
    } else {
      return redColor;
    }
  }

  QualifiedCharacteristic convertToCharacteristic(String characteristic) {
    return QualifiedCharacteristic(
        characteristicId: Uuid.parse(characteristic),
        serviceId: Uuid.parse(serviceUuid),
        deviceId: widget.bleConnectorUpdate.deviceId);
  }

  void _writeToCharacteristic(
      QualifiedCharacteristic characteristic, String value) {
    List<int> bytes = utf8.encode(value);
    widget.bleDeviceInteractor
        .writeCharacterisiticWithResponse(characteristic, bytes);
  }

  void _writeIntToCharacteristic(
      QualifiedCharacteristic characteristic, int value) {
    List<int> bytes = utf8.encode(value.toString());
    widget.bleDeviceInteractor
        .writeCharacterisiticWithResponse(characteristic, bytes);
  }

  @override
  Widget build(BuildContext context) => Scaffold(
        backgroundColor: backgroundColor,
        body: Column(
          // mainAxisAlignment: MainAxisAlignment.center,
          children: [
            SizedBox(
              height: MediaQuery.of(context).size.height * 0.05,
            ),
            Center(
              child: Container(
                alignment: Alignment.center,
                padding: const EdgeInsets.all(30),
                width: MediaQuery.of(context).size.width * 0.4,
                decoration: BoxDecoration(
                    shape: BoxShape.circle,
                    color: _getColorState(
                      widget.scannerState.scanIsInProgress,
                      widget.deviceConnected,
                    )),
                child: IconButton(
                  color: Colors.white,
                  icon: _getIconState(
                    widget.scannerState.scanIsInProgress,
                    widget.deviceConnected,
                  ),
                  onPressed: () => _getBluetoothButtonFunction(),
                  iconSize: 40,
                ),
              ),
            ),
            const SizedBox(height: 8),
            if (!widget.scannerState.scanIsInProgress &&
                !widget.deviceConnected)
              Center(
                child: Text(
                  "No device connected",
                  style: TextStyle(
                      fontSize: MediaQuery.of(context).size.width * 0.05),
                ),
              ),
            if (widget.scannerState.scanIsInProgress)
              Expanded(
                child: Padding(
                  padding: const EdgeInsets.all(8.0),
                  child: ListView(
                    children: widget.scannerState.discoveredDevices
                        .map(
                          (device) => Card(
                              shape: RoundedRectangleBorder(
                                  borderRadius: BorderRadius.circular(15)),
                              child: Padding(
                                padding: const EdgeInsets.all(15.0),
                                child: GestureDetector(
                                    onTap: () => widget.bleConnectorUpdate
                                                .connectionState ==
                                            ConnectionState.done
                                        ? _disconnect()
                                        : _connect(device.id),
                                    child: Container(
                                        child: Text("${device.name}"))),
                              )
                              /* tileColor: Colors.white,
                          subtitle: Text("${device.id}\nRSSI: ${device.rssi}"),
                          onTap: () async {
                            widget.stopScan();
                          }, */
                              ),
                        )
                        .toList(),
                  ),
                ),
              ),
            if (widget.deviceConnected)
              Column(
                children: [
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
                  Center(child: dPadButton(Icon(Icons.pause), 0)),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceAround,
                    children: [
                      dPadButton(Icon(Icons.arrow_back), -1),
                      dPadButton(Icon(Icons.arrow_forward), 1)
                    ],
                  ),
                  Center(child: dPadButton(Icon(Icons.arrow_downward), 0))
                ],
              )
          ],
        ),
      );

  Container dPadButton(Widget icon, int index) {
    return Container(
      alignment: Alignment.center,
      padding: const EdgeInsets.all(10),
      decoration: BoxDecoration(
          shape: BoxShape.circle,
          color: Color.fromARGB(widget._manualOn ? 50 : 255, 255, 255, 255),
          border: Border.all(
              width: 2,
              color: Color.fromARGB(widget._manualOn ? 50 : 255, 65, 65, 65))),
      child: IconButton(
        color: Color.fromARGB(widget._manualOn ? 50 : 255, 0, 0, 0),
        icon: icon,
        onPressed: () {
          print("Pressed button");
          !widget._manualOn
              ? _writeToCharacteristic(
                  convertToCharacteristic(characteristicUuid), index.toString())
              : null;
          // _ble.writeCharacteristicWithoutResponse(QualifiedCharacteristic(characteristicId: Uuid.parse(characteristicUuid), serviceId: Uuid.parse(serviceUuid), deviceId: deviceId), value: 1);
        },
        iconSize: 40,
      ),
    );
  }

  FlutterSwitch autoManualSwitch(BuildContext context) {
    return FlutterSwitch(
        value: widget._manualOn,
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
          widget._manualOn = val;
          _writeIntToCharacteristic(
              convertToCharacteristic(characteristicUuid), val ? 1 : 0);

          setState(() {
            widget._manualOn = val;
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
}
