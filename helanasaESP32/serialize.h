/// Interface definition ///

class ISerializeAPI {
  public:
    virtual String SerializeMonitoring() = 0;
};

/// Derived class ///

class SerializeAPI: public ISerializeAPI {
  private:
    DallasTemperature* sensors;
    ITimeSpanEngine* tsEngine;
    INASABrain* brain;
    
  public:
  SerializeAPI(DallasTemperature *pSensors, ITimeSpanEngine *pTsEngine, INASABrain *pBrain) : 
    sensors(pSensors), brain(pBrain), tsEngine(pTsEngine) { }
    
  String SerializeMonitoring()
  {
    String json;
    StaticJsonDocument<300> doc;
    doc["currentTemp"] = sensors->getTempCByIndex(0);
    
    doc["motorOn"] = brain->isMotorOn();
    doc["backendVersion"] = brain->getBackendVersion();
    doc["monitoringMode"] = brain->getmonitoringMode();
    doc["motorStartTime"] = brain->getMotorLastStartTime();
    doc["motorEndTime"] = brain->getMotorLastEndTime();
    doc["timeSpanid"] = tsEngine->getCurrentTimeSpanId();
    doc["timeSpanName"] = tsEngine->getTimeSpanName();
    doc["tempMax"] = tsEngine->getTempMax();
    doc["tempMin"] = tsEngine->getTempMin();
    doc["tsForced"] = tsEngine->forcedTs();
    
    serializeJson(doc, json);
    return json;
  }  
};
