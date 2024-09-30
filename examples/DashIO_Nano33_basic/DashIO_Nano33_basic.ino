#include "DashioNano33BLE.h"

const char configC64Str[] PROGMEM =
"lVNNb9swDP0rhQ47CUPiLM2WW2S7bhEndh3V3TAMgxursRZH8mQ5Hy3y30f5I83W7lD4QpOPpPge+Yx84qPx9x8YTecBAesZVRVP"
"0RhNh2RbrarLW5HE9zNvE4ZeFa8QRqU+5AwA8yCaTXxwLKXQSuY3jskivb7BMJEGIj8EImI5S0rAa1UxjDbJHo37vR5GRaKY0HWS"
"E9dJiqVxkleAHUFcc123mQr5AMGM8VWmo0Rzica9j9ZliwhlycEnAEmD0LTO5G7Gxcw0anoeOswpe/gJviFGa6hty1wq8wjGiouQ"
"izXUSHmSX8k8l7uybt8WMu4O/o2ZMGB3PNXZWWWM9q/6WdbIGg1geg7v7B2B7YXvRA3v1P1KG8u5aV2zSdga7vyusfzAa4xJ7DTG"
"wrdpi7f91nDi+L7WsNOIBJHjRgsjklY5cHIFWi34E8QGwPFK8RQGqjaiRGPLMuSBKo2nG1pUmxOk/7fYrW6mNJEqZaoj5z7jmnWR"
"9UqkXYAYeRv/f7FUJaKst2N5ALZMyy45WRp1us1wQI6LDxftgph8aiJE7t9o14VeVTccRCA0MDI4g74Q1f+MEYex58nGdL0r0LEW"
"a+Kfn8uXXqx+E3JNPe/GyTLuFtNf+9uzcyGTCP4KxZa8rPd18A+ZNZfvvw/Dwnvuo95tnp/49xRjAr11Jf2h+eAlkgv9Iu5p82Ez"
"Hpg6a2C7c+pGb97ge66kElyDHAidH4x9HbVnQr0ovG5MQum8s7z2ZuwruJRnlLItX7IF01UBpQRoh3f8kWO9LH4WUmmcJmWGZ7eU"
"YuK7mNphM5FT58Wc7dp9f1xFbAvm8fgH";

DashDevice dashDevice("SAMD_NINA_Type", configC64Str, 1);
DashBLE    ble_con(&dashDevice, true);

int dialValue = 0;

void processStatus(ConnectionType connectionType) {
    String message((char *)0);
    message.reserve(1024);

    message = dashDevice.getKnobMessage("KB01", dialValue);
    message += dashDevice.getDialMessage("D01", dialValue);

    ble_con.sendMessage(message);
}

void processIncomingMessage(MessageData *messageData) {
    switch (messageData->control) {
    case status:
        processStatus(messageData->connectionType);
        break;
    case knob:
        if (messageData->idStr == "KB01") {
            dialValue = messageData->payloadStr.toFloat();
            String message = dashDevice.getDialMessage("D01", dialValue);
            ble_con.sendMessage(message);
        }
        break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    ble_con.setCallback(&processIncomingMessage);
    ble_con.begin();
    dashDevice.setup(ble_con.macAddress(), "Bob Name"); // unique deviceID, and device name
}

void loop() {
    ble_con.run();
}