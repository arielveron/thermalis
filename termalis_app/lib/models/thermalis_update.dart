class ThermalisUpdate {
  double currentTemp = 0.0;
  bool motorOn = false;
  String backendVersion = "0.0.0";
  String monitoringMode = "Thermalis";
  String motorStartTime = "0000";
  String motorEndTime = "0000";
  int timeSpanId = 0;
  String timeSpanName = "Desconectado";
  int tempMax = 0;
  int tempMin = 0;
  int tsForced = -1;

  ThermalisUpdate();

  ThermalisUpdate.set({
    required this.currentTemp,
    required this.motorOn,
    required this.backendVersion,
    required this.monitoringMode,
    required this.motorStartTime,
    required this.motorEndTime,
    required this.timeSpanId,
    required this.timeSpanName,
    required this.tempMax,
    required this.tempMin,
    required this.tsForced,
  });

  factory ThermalisUpdate.fromJson(Map<String, dynamic> json) {
    return ThermalisUpdate.set(
        currentTemp: json['currentTemp'].toDouble(),
        motorOn: json['motorOn'],
        backendVersion: json['backendVersion'],
        monitoringMode: json['monitoringMode'],
        motorStartTime: json['motorStartTime'],
        motorEndTime: json['motorEndTime'],
        timeSpanId: json['timeSpanid'],
        timeSpanName: json['timeSpanName'],
        tempMax: json['tempMax'],
        tempMin: json['tempMin'],
        tsForced: json['tsForced']);
  }
}
