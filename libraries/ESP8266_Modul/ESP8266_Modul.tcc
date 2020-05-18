template <class MODULE, typename FUN>
void ESP8266_Modul::parseJsonToData(String jsonData, String dataName, bool isEqual, MODULE *hardware, FUN f) {
  String key, value;

  FirebaseJson json;
  FirebaseJsonObject jsonParseResult;
  size_t count = 0;

  json.setJsonData(jsonData);
  count = json.parse().getJsonObjectIteratorCount();

  for (size_t i = 0; i < count; i++)
  {
      json.jsonObjectiterator(i, key, value);
      jsonParseResult = json.parseResult();
      
      if (jsonParseResult.success) {
          if (isEqual && key.indexOf(dataName) >= 0)
              (hardware->*f)(key, value);
          else if (!isEqual && key.indexOf(dataName) == -1)
              (hardware->*f)(key, value);
      }
  }
}