class BleLogger {
  final List<String> _logMessages = [];

  List<String> get messages => _logMessages;

  void addToLog(String message) {
    final now = DateTime.now();
    _logMessages.add('$message');
  }

  void clearLogs() => _logMessages.clear();
}
