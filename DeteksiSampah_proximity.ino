#define SOCKETIOCLIENT_USE_SSL
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <SocketIoClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char* ssid = "F4.H4";
const char* password = "FAHAFAHA";
char host[] = "deteksisampah.site";
const char* apiEndpoint = "https://deteksisampah.site/api/sample";
int port = 443;
char path[] =  "/socket.io/?EIO=3&transport=websocket";
bool useSSL = true;
const char* sslFingerprint = "";
bool useAuth = false;
const char* serverUsername = "socketIOUsername";
const char* serverPassword = "socketIOPassword";

#define DHTPIN 4
#define DHTTYPE DHT22
#define INDUKTIF_PIN 34
#define KAPASITIF_PIN 36

DHT dht(DHTPIN, DHTTYPE);
SocketIoClient webSocket;
unsigned long lastmillisend = 0;
float temp = 0;
float hum = 0;
bool detInd = false;
bool detCap = false;
int aktual = 0;
int hasil = 0;
const char* labels[4] = { "ORGANIK", "ANORGANIK", "NONE", "CAMPURAN" };
float precisionO = 0;
float recallO = 0;
float accO = 0;
float precisionA = 0;
float recallA = 0;
float accA = 0;
float globalAcc = 0;
float confO = 0;
float confA = 0;
float confidenceGlobal = 0;
bool reset = false;
bool record = false;
// ===== EVENT SOCKET =====
void socket_Connected(const char* payload, size_t length) {
  Serial.println("Socket.IO Connected!");
}
void socket_event(const char* payload, size_t length) {
  Serial.print("got message: ");
  Serial.println(payload);
}

void selected_class(const char* payload, size_t length) {
  Serial.print("class: ");
  Serial.println(payload);
  DynamicJsonDocument doc(256);
  deserializeJson(doc, payload);
  aktual = doc["class"];
}
void reset_event(const char* payload, size_t length) {
  Serial.print("class: ");
  Serial.println(payload);
  reset = true;
}
void start_record(const char* payload, size_t length) {
  Serial.print("class: ");
  Serial.println(payload);
  DynamicJsonDocument doc(256);
  deserializeJson(doc, payload);
  record = doc["recording"].as<bool>();
}
const int OUT_RES = 101;


// === Membership Functions ===
float tri(float x, float a, float b, float c) {
  if (x <= a || x >= c) return 0;
  if (x == b) return 1;
  if (x > a && x < b) return (x - a) / (b - a);
  return (c - x) / (c - b);
}

float trap(float x, float a, float b, float c, float d) {
  if (x <= a || x >= d) return 0;
  if (x >= b && x <= c) return 1;
  if (x > a && x < b) return (x - a) / (b - a);
  return (d - x) / (d - c);
}

// === Fuzzy Input MFs ===
float H_extremelyDry(float x) {
  return trap(x, 0, 0, 65, 67);
}  // sangat kering
float H_veryDry(float x) {
  return tri(x, 66, 68, 70);
}  // kering
float H_dry(float x) {
  return tri(x, 69, 71, 73);
}  // agak kering
float H_slightlyMoist(float x) {
  return tri(x, 72, 73.5, 75);
}  // mulai lembap → transisi awal organik
float H_moderateMoist(float x) {
  return tri(x, 74, 76, 78);
}  // lembap sedang → organik ringan
float H_moist(float x) {
  return tri(x, 77, 79, 81);
}  // lembap kuat → organik jelas
float H_veryMoist(float x) {
  return tri(x, 80, 83, 86);
}  // sangat lembap
float H_wet(float x) {
  return tri(x, 85, 88, 91);
}  // basah
float H_extremelyWet(float x) {
  return trap(x, 90, 93, 100, 100);
}  // sangat basah


// === Output MFs ===
float Out_Organik(float x) {
  return trap(x, 0, 0, 45, 60);
}
float Out_Anorganik(float x) {
  return trap(x, 45, 60, 100, 100);
}

// === Aggregasi Rule ===
void applyRule(float strength, const char* label, float agg[]) {
  for (int x = 0; x < OUT_RES; x++) {
    float xm = (float)x;
    float mf = (strcmp(label, "Organik") == 0) ? Out_Organik(xm) : Out_Anorganik(xm);
    agg[x] = max(agg[x], min(mf, strength));
  }
}

// === Defuzzifikasi Centroid ===
float defuzz(float agg[]) {
  float num = 0, den = 0;
  for (int x = 0; x < OUT_RES; x++) {
    num += x * agg[x];
    den += agg[x];
  }
  return (den == 0) ? 0 : num / den;
}

// === Fuzzy Evaluator ===
// int evaluateFuzzy(bool detInd, bool detCap, float hum) {
//   if (!detInd && !detCap) {
//     return 2;
//   }

//   // === Derajat keanggotaan kelembaban ===
//   float hExtDry = H_extremelyDry(hum);
//   float hVeryDry = H_veryDry(hum);
//   float hDry = H_dry(hum);
//   float hSlightMoist = H_slightlyMoist(hum);
//   float hModerate = H_moderateMoist(hum);
//   float hMoist = H_moist(hum);
//   float hVeryMoist = H_veryMoist(hum);
//   float hWet = H_wet(hum);
//   float hExtWet = H_extremelyWet(hum);

//   // === Bobot kepercayaan sensor ===
//   float wInd = detInd ? 0.9f : 0.0f;
//   float wCap = detCap ? 0.8f : 0.0f;

//   float outOrganik[OUT_RES] = { 0 };
//   float outAnorganik[OUT_RES] = { 0 };

//   // === RULE BASE YANG DITATA ULANG ===
//   // R1: Induktif aktif → kemungkinan besar anorganik (logam)
//   applyRule(wInd, "Anorganik", outAnorganik);

//   // R2: Kapasitif aktif dan kelembaban tinggi → organik (daun, sisa makanan)
//   applyRule(min(wCap, hHigh), "Organik", outOrganik);

//   // R3: Kapasitif aktif dan kelembaban sedang → bisa organik (daun kering, kertas)
//   applyRule(min(wCap, hMed), "Organik", outOrganik);

//   // R4: Kapasitif aktif dan kelembaban rendah → anorganik (plastik kering)
//   applyRule(min(wCap, hLow), "Anorganik", outAnorganik);

//   // R5: Induktif aktif dan kelembaban rendah → anorganik kuat (logam, kering)
//   applyRule(min(wInd, hLow), "Anorganik", outAnorganik);

//   // R6: Induktif aktif tapi kelembaban tinggi → kemungkinan salah, kurangi kepastian
//   applyRule(min(wInd * 0.5f, hHigh), "Anorganik", outAnorganik);

//   // R7: Hanya kapasitif yang aktif → bisa organik (non-logam, lembab)
//   applyRule(wCap, "Organik", outOrganik);

//   // R8: Kelembaban sangat tinggi tapi sensor lemah → organik (bahan basah alami)
//   applyRule(min(hHigh, max(wCap, wInd * 0.3f)), "Organik", outOrganik);
//   // R2a: Kapasitif aktif dan kelembaban sangat tinggi → bahan basah alami → organik kuat
//   applyRule(min(wCap, hVeryHigh), "Organik", outOrganik);

//   // R3a: Kapasitif aktif dan kelembaban tinggi → organik biasa (daun basah)
//   applyRule(min(wCap, hHigh), "Organik", outOrganik);

//   // R3b: Kapasitif aktif dan kelembaban sedang → bahan semi organik (kertas, daun kering)
//   applyRule(min(wCap, hMed), "Organik", outOrganik);

//   // R4a: Kapasitif aktif dan kelembaban rendah → anorganik (plastik lembap)
//   applyRule(min(wCap, hLow), "Anorganik", outAnorganik);

//   // R4b: Kapasitif aktif dan kelembaban sangat rendah → anorganik kering (botol, logam)
//   applyRule(min(wCap, hVeryLow), "Anorganik", outAnorganik);

//   // R5a: Induktif aktif + kelembaban rendah → logam kering → anorganik kuat
//   applyRule(min(wInd, hLow), "Anorganik", outAnorganik);

//   // R5b: Induktif aktif + kelembaban sangat rendah → logam murni → anorganik kuat
//   applyRule(min(wInd, hVeryLow), "Anorganik", outAnorganik);

//   // R6a: Induktif aktif + kelembaban tinggi → bisa salah deteksi → kurangi bobot
//   applyRule(min(wInd * 0.4f, hHigh), "Anorganik", outAnorganik);

//   // R6b: Induktif aktif + kelembaban sangat tinggi → sinyal error → netral
//   applyRule(min(wInd * 0.2f, hVeryHigh), "Organik", outOrganik);

//   // R9: Tidak ada sensor aktif tapi kelembaban rendah → kemungkinan anorganik
//   if (!detCap && !detInd && hLow > 0.5f) {
//     applyRule(hLow, "Anorganik", outAnorganik);
//   }

//   // === DEFUZZIFIKASI ===
//   float valOrg = defuzz(outOrganik);
//   float valAnorg = defuzz(outAnorganik);

//   // === Normalisasi hasil ===
//   valOrg = constrain(valOrg, 0, OUT_RES);
//   valAnorg = constrain(valAnorg, 0, OUT_RES);

//   // === Keputusan ===
//   float diff = fabs(valOrg - valAnorg);
//   float maxVal = max(valOrg, valAnorg);
//   const float MIN_CONFIDENCE = 10.0f;

//   if (maxVal < MIN_CONFIDENCE) {
//     return 2;
//   }

//   if (diff < 8.0f) {
//     return 3;
//   }

//   return (valOrg > valAnorg) ? 0 : 1;
// }
int evaluateFuzzy(bool detInd, bool detCap, float hum) {
  if (!detInd && !detCap) {
    return 2;  // Tidak ada sensor aktif
  }
  // === Derajat keanggotaan kelembaban ===
  float hExtDry = H_extremelyDry(hum);
  float hVeryDry = H_veryDry(hum);
  float hDry = H_dry(hum);
  float hSlightMoist = H_slightlyMoist(hum);
  float hModerate = H_moderateMoist(hum);
  float hMoist = H_moist(hum);
  float hVeryMoist = H_veryMoist(hum);
  float hWet = H_wet(hum);
  float hExtWet = H_extremelyWet(hum);
  // === Bobot kepercayaan sensor ===
  float wInd = detInd ? 0.9f : 0.0f;
  float wCap = detCap ? 0.8f : 0.0f;
  float outOrganik[OUT_RES] = { 0 };
  float outAnorganik[OUT_RES] = { 0 };
  // ==============================================================
  //  RULE BASE UTAMA (lebih dari 20 aturan)
  // ==============================================================
  //  Kapasitif dominan → cenderung organik
  applyRule(wCap, "Organik", outOrganik);
  // 1️Kelembaban sangat rendah = bahan kering → anorganik
  applyRule(min(wCap, hExtDry), "Anorganik", outAnorganik);
  applyRule(min(wInd, hExtDry), "Anorganik", outAnorganik);
  //  Kelembaban rendah = bahan kering ringan (plastik, logam kering)
  applyRule(min(wCap, hVeryDry), "Anorganik", outAnorganik);
  applyRule(min(wInd, hVeryDry), "Anorganik", outAnorganik);
  //  Kelembaban agak kering tapi kapasitif aktif → bahan semi isolator
  applyRule(min(wCap, hDry), "Anorganik", outAnorganik);
  //  Kelembaban agak lembap + kapasitif aktif → bahan organik ringan
  applyRule(min(wCap, hSlightMoist), "Organik", outOrganik);
  //  Kelembaban sedang + kapasitif aktif → bahan organik (kertas, daun kering)
  applyRule(min(wCap, hModerate), "Organik", outOrganik);
  //  Kelembaban tinggi + kapasitif aktif → bahan organik kuat (daun, makanan)
  applyRule(min(wCap, hMoist), "Organik", outOrganik);
  //  Kelembaban sangat tinggi + kapasitif aktif → organik basah alami
  applyRule(min(wCap, hVeryMoist), "Organik", outOrganik);
  //  Kelembaban ekstrem tinggi → bahan basah alami (organik kuat)
  applyRule(min(wCap, hWet), "Organik", outOrganik);
  applyRule(min(wCap, hExtWet), "Organik", outOrganik);
  // Induktif dominan → cenderung anorganik (logam, bahan konduktor)
  applyRule(wInd, "Anorganik", outAnorganik);
  //  Induktif aktif + kelembaban rendah → logam kering
  applyRule(min(wInd, hDry), "Anorganik", outAnorganik);
  // Induktif aktif + kelembaban sangat rendah → logam murni
  applyRule(min(wInd, hVeryDry), "Anorganik", outAnorganik);
  //  Induktif aktif + kelembaban sedang → bahan semi logam
  applyRule(min(wInd, hModerate), "Anorganik", outAnorganik);
  //  Induktif aktif + kelembaban tinggi → konduktor basah → noise → kurangi bobot
  applyRule(min(wInd * 0.4f, hMoist), "Anorganik", outAnorganik);
  //  Induktif aktif + kelembaban sangat tinggi → bisa error → organik lemah
  applyRule(min(wInd * 0.2f, hVeryMoist), "Organik", outOrganik);
  // Kapasitif aktif dan kelembaban mulai lembap → bahan organik ringan
  applyRule(min(wCap, hSlightMoist), "Organik", outOrganik);
  // Kapasitif aktif dan kelembaban sedang → bahan organik (kertas, daun lembap)
  applyRule(min(wCap, hModerate), "Organik", outOrganik);
  //  Kombinasi Sensor (detInd + detCap aktif bersamaan)
  if (detInd && detCap) {
    //  Kedua sensor aktif + kelembaban sedang → campuran bahan (semi organik)
    applyRule(min(min(wInd, wCap), hModerate), "Organik", outOrganik);
    //  Kedua sensor aktif + kelembaban tinggi → organik dominan
    applyRule(min(min(wInd, wCap), hMoist), "Organik", outOrganik);
    //  Kedua sensor aktif + kelembaban rendah → logam atau plastik
    applyRule(min(min(wInd, wCap), hDry), "Anorganik", outAnorganik);

    //  Kedua sensor aktif + kelembaban ekstrem tinggi → organik basah alami
    applyRule(min(min(wInd, wCap), hExtWet), "Organik", outOrganik);
  }
  //  Ketika hanya satu sensor aktif
  if (detCap && !detInd) {
    //  Kapasitif saja + kelembaban menengah → bahan lembap alami
    applyRule(min(wCap, hModerate), "Organik", outOrganik);
  }
  if (detInd && !detCap) {
    //  Induktif saja + kelembaban rendah → logam murni
    applyRule(min(wInd, hVeryDry), "Anorganik", outAnorganik);
  }
  //  Kelembaban ekstrem tanpa sinyal kuat → organik alami
  // (misal sensor kurang respons tapi udara sangat lembap)
  applyRule(min(hExtWet, max(wCap, wInd * 0.3f)), "Organik", outOrganik);
  //  Jika kelembaban antara 75–80 (transisi lembap ringan)
  float trans = max(hSlightMoist, hModerate);
  applyRule(min(wCap * 0.9f, trans), "Organik", outOrganik);
  //  Jika kelembaban turun cepat → anorganik (kering cepat)
  if (hum < 72 && wCap > 0.2f) {
    applyRule(hDry, "Anorganik", outAnorganik);
  }
  //  Jika kelembaban naik cepat → organik (menyimpan air)
  if (hum > 85 && wCap > 0.2f) {
    applyRule(hVeryMoist, "Organik", outOrganik);
  }
  //  Tidak ada sensor aktif tapi kelembaban tinggi → lingkungan organik
  if (!detCap && !detInd && hum > 80) {
    applyRule(hVeryMoist, "Organik", outOrganik);
  }
  // ==============================================================
  //  DEFUZZIFIKASI
  // ==============================================================
  float valOrg = defuzz(outOrganik);
  float valAnorg = defuzz(outAnorganik);

  valOrg = constrain(valOrg, 0, OUT_RES);
  valAnorg = constrain(valAnorg, 0, OUT_RES);

  //  KEPUTUSAN AKHIR
  float diff = fabs(valOrg - valAnorg);
  float maxVal = max(valOrg, valAnorg);
  const float MIN_CONFIDENCE = 10.0f;

  if (maxVal < MIN_CONFIDENCE) {
    return 2;  // netral / tidak yakin
  }

  if (diff < 8.0f) {
    return 3;  // ambigu / transisi
  }

  return (valOrg > valAnorg) ? 0 : 1;  // 0 = organik, 1 = anorganik
}

struct Metric {
  unsigned long TP = 0, FP = 0, FN = 0, TN = 0, DETEK = 0;

  float precision() const {
    return (TP + FP == 0) ? 0 : (float)TP / (TP + FP);
  }

  float recall() const {
    return (TP + FN == 0) ? 0 : (float)TP / (TP + FN);
  }

  float accuracy() const {
    return (TP + TN + FP + FN == 0) ? 0 : (float)(TP + TN) / (TP + TN + FP + FN);
  }

  void reset() {
    TP = FP = FN = TN = DETEK = 0;
  }
};

Metric organik, anorganik;
unsigned long totalPred = 0;
void updateMetrics(int actual, int pred) {
  if (pred == 2 || pred == 3) {
    return;
  }

  totalPred++;
  if (actual == 0) {
    if (pred == 0) {
      organik.TP++;
      anorganik.TN++;
      organik.DETEK++;
    } else if (pred == 1) {
      organik.FN++;
      anorganik.FP++;
    }
  } else if (actual == 1) {
    if (pred == 1) {
      anorganik.TP++;
      anorganik.DETEK++;
      organik.TN++;
    } else if (pred == 0) {
      anorganik.FN++;
      organik.FP++;
    }
  }
  precisionO = organik.precision();
  recallO = organik.recall();
  accO = organik.accuracy();

  precisionA = anorganik.precision();
  recallA = anorganik.recall();
  accA = anorganik.accuracy();
  confO = ((precisionO + recallO) == 0.0f) ? 0.0f : (2.0f * precisionO * recallO) / (precisionO + recallO);
  confA = ((precisionA + recallA) == 0.0f) ? 0.0f : (2.0f * precisionA * recallA) / (precisionA + recallA);
  unsigned long totalAll =
    organik.TP + organik.FP + organik.FN + organik.TN + anorganik.TP + anorganik.FP + anorganik.FN + anorganik.TN;
  globalAcc = (float)(organik.TP + anorganik.TP) / max(1UL, totalAll);
  confidenceGlobal = (confO + confA) / 2.0f;
}



void printMetrics() {
  Serial.println("\n=====  METRIK KLASIFIKASI =====");
  Serial.print("Total sampel: ");
  Serial.println(totalPred);
  Serial.print("Akurasi Global: ");
  Serial.print(globalAcc * 100, 2);
  Serial.println("%");

  Serial.println("\n--- ORGANIK ---");
  Serial.print("Precision: ");
  Serial.print(precisionO * 100, 2);
  Serial.println("%");
  Serial.print("Recall: ");
  Serial.print(recallO * 100, 2);
  Serial.println("%");
  Serial.print("Accuracy: ");
  Serial.print(accO * 100, 2);
  Serial.println("%");

  Serial.println("\n--- ANORGANIK ---");
  Serial.print("Precision: ");
  Serial.print(precisionA * 100, 2);
  Serial.println("%");
  Serial.print("Recall: ");
  Serial.print(recallA * 100, 2);
  Serial.println("%");
  Serial.print("Accuracy: ");
  Serial.print(accA * 100, 2);
  Serial.println("%");
  Serial.println("=================================\n");
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  delay(100);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  webSocket.on("connect", socket_Connected);
  webSocket.on("event", socket_event);
  webSocket.on("selected_class_BE", selected_class);
  webSocket.on("reset_system", reset_event);
  webSocket.on("record_status", start_record);


  // webSocket.on("record_status", stop_record);

  if (useSSL) {
    webSocket.beginSSL(host, port, path, sslFingerprint);
  } else {
    webSocket.begin(host, port, path);
  }
  if (useAuth) { webSocket.setAuthorization(serverUsername, serverPassword); }
  dht.begin();
  pinMode(INDUKTIF_PIN, INPUT);
  pinMode(KAPASITIF_PIN, INPUT);
  Serial.println("=== Fuzzy Klasifikasi Sampah Canggih ===");
}
void resetAll(void) {
  organik.reset();
  anorganik.reset();
  totalPred = 0;
  precisionO = recallO = accO = 0.0f;
  precisionA = recallA = accA = 0.0f;
  confO = confA = 0.0f;
  globalAcc = confidenceGlobal = 0.0f;
  Serial.println("⚙️ Semua metrik dan counter telah direset ke nilai awal!");
}

void loop() {
  webSocket.loop();
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  detInd = (digitalRead(INDUKTIF_PIN) == LOW);
  detCap = (!digitalRead(KAPASITIF_PIN) == LOW);
  hasil = evaluateFuzzy(detInd, detCap, hum);
  if (reset) {
    resetAll();
    reset = false;
  }
  updateMetrics(aktual, hasil);
  if (millis() - lastmillisend > 500) {
    lastmillisend = millis();
    // printMetrics();
    sendSensorData();
    if (record) {
      sendHttpPost();
    }
  }
  // Serial.println("=================================");
  // Serial.print("Kelembaban: ");
  // Serial.print(hum, 2);
  // Serial.println(" %");
  // Serial.print("Induktif: ");
  // Serial.println(detInd ? "DETEKSI" : "TIDAK");
  // Serial.print("Kapasitif: ");
  // Serial.println(detCap ? "DETEKSI" : "TIDAK");
  // Serial.print("Prediksi: ");
  // Serial.println(labels[hasil]);
  // Serial.print("Aktual: ");
  // Serial.println(labels[aktual]);
  // Serial.println("=================================");
}
void sendSensorData() {
  StaticJsonDocument<512> doc;

  // --- Data Sensor ---
  doc["humidity"] = hum;
  doc["induktif"] = detInd;
  doc["kapasitif"] = detCap;
  doc["temperature"] = temp;

  // --- Prediksi dan Aktual ---
  doc["prediksi"] = labels[hasil];
  doc["aktual"] = labels[aktual];

  // --- Metrik Organik ---
  doc["precision_organik"] = precisionO;
  doc["recall_organik"] = recallO;
  doc["accuracy_organik"] = accO;
  doc["organik_detect"] = organik.DETEK;
  // --- Metrik Anorganik ---
  doc["precision_anorganik"] = precisionA;
  doc["recall_anorganik"] = recallA;
  doc["accuracy_anorganik"] = accA;
  doc["accuracy_anorganik"] = accA;
  doc["anorganik_detect"] = anorganik.DETEK;
  // --- Akurasi Total ---
  doc["accuracy_total"] = globalAcc;
  doc["confidence_organik"] = confO;
  doc["confidence_anorganik"] = confA;
  doc["confidence_total"] = confidenceGlobal;
  doc["total_prediction"] = totalPred;

  // --- Kirim JSON via Socket.IO ---
  String jsonStr;
  serializeJson(doc, jsonStr);
  webSocket.emit("sensor_data", jsonStr.c_str());

  // Debug opsional
  // Serial.println("📤 Data dikirim ke server:");
  // Serial.println(jsonStr);
}
void sendHttpPost() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi tidak terhubung, POST dibatalkan");
    return;
  }

  HTTPClient http;
  http.begin(apiEndpoint);
  http.addHeader("Content-Type", "application/json");
  StaticJsonDocument<512> doc;
  // === Data utama ===
  doc["humidity"] = hum;
  doc["induktif"] = detInd ? 1 : 0;
  doc["kapasitif"] = detCap ? 1 : 0;
  doc["prediksi"] = labels[hasil];
  doc["aktual"] = labels[aktual];
  // === Metrik Organik ===
  doc["tp_organik"] = organik.TP;
  doc["tn_organik"] = organik.TN;
  doc["fp_organik"] = organik.FP;
  doc["fn_organik"] = organik.FN;
  doc["precision_organik"] = precisionO;
  doc["recall_organik"] = recallO;
  doc["accuracy_organik"] = accO;
  // === Metrik Anorganik ===
  doc["tp_anorganik"] = anorganik.TP;
  doc["tn_anorganik"] = anorganik.TN;
  doc["fp_anorganik"] = anorganik.FP;
  doc["fn_anorganik"] = anorganik.FN;
  doc["precision_anorganik"] = precisionA;
  doc["recall_anorganik"] = recallA;
  doc["accuracy_anorganik"] = accA;
  doc["accuracy_total"] = globalAcc;
  String jsonStr;
  serializeJson(doc, jsonStr);
  int httpResponseCode = http.POST(jsonStr);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.printf("HTTP Response [%d]: %s\n", httpResponseCode, response.c_str());
  } else {
    Serial.printf(" HTTP POST gagal, kode error: %d\n", httpResponseCode);
  }
  http.end();
}
