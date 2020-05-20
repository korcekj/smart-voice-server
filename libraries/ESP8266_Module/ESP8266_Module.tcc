// Metoda sluziaca na parsovanie json dat a nasledne zavolanie prislusnej funkcie, ktora sa o preparsovane data postara
template <class MODULE, typename FUN>
void ESP8266_Module::parseJsonToData(String jsonData, String dataName, bool isEqual, MODULE *hardware, FUN f) {
  String key, value;
  // FirebaseJson objekt
  FirebaseJson json;
  // FirebaseJsonObject objekt
  FirebaseJsonObject jsonParseResult;
  // Pocet vnorenych dat v json objekte
  size_t count = 0;
  
  // Nastavenie json dat
  json.setJsonData(jsonData);
  // Ziskanie poctu vnorenych dat
  count = json.parse().getJsonObjectIteratorCount();

  // Prechadzanie a parsovanie prislusnych dat v json objekte
  for (size_t i = 0; i < count; i++)
  {
      json.jsonObjectiterator(i, key, value);
      jsonParseResult = json.parseResult();
      // V pripade ze sa parsovanie podarilo vykonaj prislusnu funkciu     
      if (jsonParseResult.success) {
          if (isEqual && key.indexOf(dataName) >= 0)
              (hardware->*f)(key, value);
          else if (!isEqual && key.indexOf(dataName) == -1)
              (hardware->*f)(key, value);
      }
  }
}