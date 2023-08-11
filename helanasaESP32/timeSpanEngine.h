/// Interface definition ///

class ITimeSpanEngine {
  public:
    virtual void setTimeSpan(int pos, TimeSpanData &timeSpan) = 0;
    virtual String getStartTime() = 0;
    virtual String getEndTime() = 0;
    virtual float getTempMin() = 0;
    virtual float getTempMax() = 0;
    virtual int getCurrentTimeSpanId() = 0;
    virtual String getTimeSpanName() = 0;
    virtual String getCurrentTime() = 0;
    virtual void setStartTime(int id, String timeString) = 0;
    virtual void setEndTime(int id, String timeString) = 0;
    virtual void setTempMin(int id, int temp) = 0;
    virtual void setTempMax(int id, int temp) = 0;
    virtual void setTimeSpanName(int id, String tsName) = 0;
    virtual void setEnabled(int id, bool enabled) = 0;
    virtual int forcedTs() = 0;
    virtual void resetForcedTimeSpan() = 0;
    virtual void forceTimeSpan(int id) = 0;
    virtual void update(int currentHour, int currentMinutes) = 0;
};

/// Derived class ///

class TimeSpanEngine : public ITimeSpanEngine {
  private:
    const int noForcedTs = -1;
    TimeSpanData currentTimeSpan;
    String currentRawTime;
    int forcedTimeSpan = noForcedTs;
    
  public:
    TimeSpanData periods[numPeriods];
    
    TimeSpanEngine(TimeSpanData &initialTimeSpan)
    {
      for (int i = 0; i < numPeriods; i++) {
        periods[i] = initialTimeSpan;
        periods[i].id = i;
        periods[i].enabled = false;
      }
      periods[0].enabled = true;
      
      currentTimeSpan = periods[0];
    }
    
    void setTimeSpan(int pos, TimeSpanData &timeSpan) {
      periods[pos] = timeSpan;
      periods[pos].id = pos;
    }
    
    String getStartTime() {
      return currentTimeSpan.startTime;
    }
    
    String getEndTime() {
      return currentTimeSpan.endTime;
    }
    
    float getTempMin() {
      return currentTimeSpan.tempMin;
    }
    
    float getTempMax() {
      return currentTimeSpan.tempMax;
    }
    
    int getCurrentTimeSpanId() {
      return currentTimeSpan.id;
    }
    
    String getTimeSpanName() {
      return currentTimeSpan.tsName;
    }
    
    String getCurrentTime() {
      return currentRawTime;
    }
    
    void setStartTime(int id, String timeString) {
      periods[id].startTime = timeString;
    }
    
    void setEndTime(int id, String timeString) {
      periods[id].endTime = timeString;
    }
    
    void setTempMin(int id, int temp) {
      periods[id].tempMin = temp;
    }
    
    void setTempMax(int id, int temp) {
      periods[id].tempMax = temp;
    }
    
    void setTimeSpanName(int id, String tsName) {
      periods[id].tsName = tsName;
    }
    
    void setEnabled(int id, bool enabled) {
      Serial.println(enabled);
      periods[id].enabled = enabled;
    }
    
    int forcedTs() {
      return forcedTimeSpan;
    }
    
    void resetForcedTimeSpan(){
      forcedTimeSpan = noForcedTs;
    }
    
    void forceTimeSpan(int id) {
      forcedTimeSpan = id;
    }
    
    void update(int currentHour, int currentMinutes) {
      currentRawTime = String(currentHour * 100 + currentMinutes);
      int currentTime = currentRawTime.toInt();
      
      if (forcedTimeSpan != -1) {
        currentTimeSpan = periods[forcedTimeSpan];
        return;
      }
      
//      Serial.println("---------------------");
//      Serial.println("current period: " + currentTimeSpan.tsName);
//      Serial.println("---------------------");
      for (int i = numPeriods; i >= 0; i--) {
        if (!periods[i].enabled) {
          continue;
        }
        int startTime = periods[i].startTime.toInt();
        int endTime = periods[i].endTime.toInt();

//        Serial.println(".....................");
//        Serial.println("<" + currentRawTime);
//        Serial.println("-" + String(startTime));
//        Serial.println("+" + String(endTime));

        bool result;
        if (startTime == endTime){
          currentTimeSpan = periods[i];
//          Serial.println("new period set: " + currentTimeSpan.tsName);
          break;
        }
        
        if (startTime <= endTime)
        {
          result = (currentTime >= startTime) && (currentTime < endTime);
//          Serial.println("start < end");
        }
        else
        {
          result = (currentTime >= startTime) || (currentTime < endTime);
//          Serial.println("start > end");
        }
        if (result) {
          currentTimeSpan = periods[i];
          Serial.println("new period set: " + currentTimeSpan.tsName);
          break;
        }
      }
    }

};
