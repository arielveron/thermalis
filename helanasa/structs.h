enum monitoringMode {
  TERMONASA,
  HELANASA
};

struct TimeSpanData {
  int id;
  String tsName;
  String startTime;
  String endTime;
  float tempMin;
  float tempMax;
  bool enabled;
};
