// ###########################################################
//
//      Yahoo! News取得.cpp
//
//        
//
// ###########################################################


//=======================================================================
//const char* ssid = "iPhone";            //自分のルーターのSSID
//const char* password = "0123456789";      //自分のルーターのパスワード

const char* ssid = "YAM-R1WR250X";            //自分のルーターのSSID
const char* password = "t1s2u1b1a1m2e1";      //自分のルーターのパスワード

//const char* ssid = "aterm-dd136a-g";            //自分のルーターのSSID
//const char* password = "3e55ec5f692d6";      //自分のルーターのパスワード
//=======================================================================



//=======================================================================
//  Yahoo! Japan RSSニュース、ルート証明書 
//      chrome等で取得しフォーマットを下記の様に変換
//=======================================================================
const char* yahoo= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIBADANBgkqhkiG9w0BAQsFADBdMQswCQYDVQQGEwJKUDEl\n" \
"MCMGA1UEChMcU0VDT00gVHJ1c3QgU3lzdGVtcyBDTy4sTFRELjEnMCUGA1UECxMe\n" \
"U2VjdXJpdHkgQ29tbXVuaWNhdGlvbiBSb290Q0EyMB4XDTA5MDUyOTA1MDAzOVoX\n" \
"DTI5MDUyOTA1MDAzOVowXTELMAkGA1UEBhMCSlAxJTAjBgNVBAoTHFNFQ09NIFRy\n" \
"dXN0IFN5c3RlbXMgQ08uLExURC4xJzAlBgNVBAsTHlNlY3VyaXR5IENvbW11bmlj\n" \
"YXRpb24gUm9vdENBMjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANAV\n" \
"OVKxUrO6xVmCxF1SrjpDZYBLx/KWvNs2l9amZIyoXvDjChz335c9S672XewhtUGr\n" \
"zbl+dp+++T42NKA7wfYxEUV0kz1XgMX5iZnK5atq1LXaQZAQwdbWQonCv/Q4EpVM\n" \
"VAX3NuRFg3sUZdbcDE3R3n4MqzvEFb46VqZab3ZpUql6ucjrappdUtAtCms1FgkQ\n" \
"hNBqyjoGADdH5H5XTz+L62e4iKrFvlNVspHEfbmwhRkGeC7bYRr6hfVKkaHnFtWO\n" \
"ojnflLhwHyg/i/xAXmODPIMqGplrz95Zajv8bxbXH/1KEOtOghY6rCcMU/Gt1SSw\n" \
"awNQwS08Ft1ENCcadfsCAwEAAaNCMEAwHQYDVR0OBBYEFAqFqXdlBZh8QIH4D5cs\n" \
"OPEK7DzPMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3\n" \
"DQEBCwUAA4IBAQBMOqNErLlFsceTfsgLCkLfZOoc7llsCLqJX2rKSpWeeo8HxdpF\n" \
"coJxDjrSzG+ntKEju/Ykn8sX/oymzsLS28yN/HH8AynBbF0zX2S2ZTuJbxh2ePXc\n" \
"okgfGT+Ok+vx+hfuzU7jBBJV1uXk3fs+BXziHV7Gp7yXT2g69ekuCkO2r1dcYmh8\n" \
"t/2jioSgrGK+KwmHNPBqAbubKVY8/gA3zyNs8U6qtnRGEmyR7jTV7JqR50S+kDFy\n" \
"1UkC9gLl9B/rfNmWVan/7Ir5mUf/NVoCqgTLiluHcSmRvaS0eg29mvVXIwAHIRc/\n" \
"SjnRBUkLp7Y3gaVdjKozXoEofKd9J+sAro03\n" \
"-----END CERTIFICATE----- \n";


const char* gigazine= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n" \
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n" \
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n" \
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n" \
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n" \
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n" \
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n" \
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n" \
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n" \
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n" \
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n" \
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n" \
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n" \
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n" \
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n" \
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n" \
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n" \
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n" \
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n" \
"-----END CERTIFICATE----- \n";

const char* gizmodo= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE----- \n";


const char* akiba= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIBADANBgkqhkiG9w0BAQsFADBdMQswCQYDVQQGEwJKUDEl\n" \
"MCMGA1UEChMcU0VDT00gVHJ1c3QgU3lzdGVtcyBDTy4sTFRELjEnMCUGA1UECxMe\n" \
"U2VjdXJpdHkgQ29tbXVuaWNhdGlvbiBSb290Q0EyMB4XDTA5MDUyOTA1MDAzOVoX\n" \
"DTI5MDUyOTA1MDAzOVowXTELMAkGA1UEBhMCSlAxJTAjBgNVBAoTHFNFQ09NIFRy\n" \
"dXN0IFN5c3RlbXMgQ08uLExURC4xJzAlBgNVBAsTHlNlY3VyaXR5IENvbW11bmlj\n" \
"YXRpb24gUm9vdENBMjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANAV\n" \
"OVKxUrO6xVmCxF1SrjpDZYBLx/KWvNs2l9amZIyoXvDjChz335c9S672XewhtUGr\n" \
"zbl+dp+++T42NKA7wfYxEUV0kz1XgMX5iZnK5atq1LXaQZAQwdbWQonCv/Q4EpVM\n" \
"VAX3NuRFg3sUZdbcDE3R3n4MqzvEFb46VqZab3ZpUql6ucjrappdUtAtCms1FgkQ\n" \
"hNBqyjoGADdH5H5XTz+L62e4iKrFvlNVspHEfbmwhRkGeC7bYRr6hfVKkaHnFtWO\n" \
"ojnflLhwHyg/i/xAXmODPIMqGplrz95Zajv8bxbXH/1KEOtOghY6rCcMU/Gt1SSw\n" \
"awNQwS08Ft1ENCcadfsCAwEAAaNCMEAwHQYDVR0OBBYEFAqFqXdlBZh8QIH4D5cs\n" \
"OPEK7DzPMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3\n" \
"DQEBCwUAA4IBAQBMOqNErLlFsceTfsgLCkLfZOoc7llsCLqJX2rKSpWeeo8HxdpF\n" \
"coJxDjrSzG+ntKEju/Ykn8sX/oymzsLS28yN/HH8AynBbF0zX2S2ZTuJbxh2ePXc\n" \
"okgfGT+Ok+vx+hfuzU7jBBJV1uXk3fs+BXziHV7Gp7yXT2g69ekuCkO2r1dcYmh8\n" \
"t/2jioSgrGK+KwmHNPBqAbubKVY8/gA3zyNs8U6qtnRGEmyR7jTV7JqR50S+kDFy\n" \
"1UkC9gLl9B/rfNmWVan/7Ir5mUf/NVoCqgTLiluHcSmRvaS0eg29mvVXIwAHIRc/\n" \
"SjnRBUkLp7Y3gaVdjKozXoEofKd9J+sAro03\n" \
"-----END CERTIFICATE----- \n";


#include <Arduino.h>
#include <string.h>
#include <WiFiClientSecure.h>
#include "News_Get.hpp"




//=======================================================================
//  https_Web_Get() : news取得
//=======================================================================
String https_Web_Get(const char* host1, String target_page, char char_tag, String Final_tag, String Begin_tag, String End_tag, String Begin_tag2, String End_tag2, String Paragraph, unsigned char sa){


  String ret_str;

  WiFiClientSecure https_client;

  switch (sa){

    case 0:
           https_client.setCACert(akiba); //akibaサイトのルート証明書をセットする
           break;
           
    case 1:
           https_client.setCACert(yahoo); //Yahooサイトのルート証明書をセットする
           break;

    case 2:
           https_client.setCACert(gigazine); //GIGAZINEサイトのルート証明書をセットする
           break;
           
    case 3:
           https_client.setCACert(gizmodo); //GIZMODOサイトのルート証明書をセットする
           break;
           
    default:
            break;
 
  }
   
  if (https_client.connect(host1, 443)){
    Serial.print(host1); Serial.print(F("-------------"));
    Serial.println(F("connected"));
    Serial.println(F("-------WEB HTTPS GET Request Send"));
 
    String str1 = String("GET https://") + String( host1 ) + target_page + " HTTP/1.1\r\n";
           str1 += "Host: " + String( host1 ) + "\r\n";
           str1 += "User-Agent: BuildFailureDetectorESP32\r\n";
           str1 += "Connection: close\r\n\r\n"; //closeを使うと、サーバーの応答後に切断される。最後に空行必要
           str1 += "\0";
 
    https_client.print(str1); //client.println にしないこと。最後に改行コードをプラスして送ってしまう為
    https_client.flush(); //client出力が終わるまで待つ
    Serial.print(str1);
    Serial.flush(); //シリアル出力が終わるまで待つ
     
  }else{
    Serial.println(F("------connection failed"));
  }
 
  if(https_client){
    String dummy_str;
    uint16_t from, to;
    Serial.println(F("-------WEB HTTPS Response Receive"));
 
    while(https_client.connected()){
      while(https_client.available()) {
        if(dummy_str.indexOf(Final_tag) == -1){          
          dummy_str = https_client.readStringUntil(char_tag);
 
          if(dummy_str.indexOf(Begin_tag) >= 0){
            from = dummy_str.indexOf(Begin_tag) + Begin_tag.length();
            to = dummy_str.indexOf(End_tag);
            ret_str += Paragraph;
            ret_str += dummy_str.substring(from,to);
//            ret_str += "  ";
          }
          if(dummy_str.indexOf(Begin_tag2) >= 0){
            from = dummy_str.indexOf(Begin_tag2) + Begin_tag2.length();
            to = dummy_str.indexOf(End_tag2);
//            ret_str += "：　";
            ret_str += "：";
            ret_str += dummy_str.substring(from,to);
            ret_str += "  ";
          }
        }else{
          while(https_client.available()){
            https_client.read(); //サーバーから送られてきた文字を１文字も余さず受信し切ることが大事
            //delay(1);
          }
          delay(10);
          https_client.stop(); //特に重要。コネクションが終わったら必ず stop() しておかないとヒープメモリを食い尽くしてしまう。
          delay(10);
          Serial.println(F("-------Client Stop"));
 
          break;
        }
        //delay(1);
      }
      //delay(1);
    }
  }
   
  ret_str += "\0";
  ret_str.replace("&amp;","&"); //XMLソースの場合、半角&が正しく表示されないので、全角に置き換える
  ret_str.replace("&#039;","\'"); //XMLソースの場合、半角アポストロフィーが正しく表示されないので置き換える
  ret_str.replace("&#39;","\'"); //XMLソースの場合、半角アポストロフィーが正しく表示されないので置き換える
  ret_str.replace("&apos;","\'"); //XMLソースの場合、半角アポストロフィーが正しく表示されないので置き換える
  ret_str.replace("&quot;","\""); //XMLソースの場合、ダブルクォーテーションが正しく表示されないので置き換える
   
  if(ret_str.length() < 20) ret_str = "※ニュース記事を取得できませんでした";
   
  if(https_client){
    delay(10);
    https_client.stop(); //特に重要。コネクションが終わったら必ず stop() しておかないとヒープメモリを食い尽くしてしまう。
    delay(10);
    Serial.println(F("-------Client Stop"));
  }
  Serial.flush(); //シリアル出力が終わるまで待つ
 
  return ret_str;

}

//=======================================================================
//   WiFi control
//=======================================================================
void wifiConnect(void) {
  delay(100);                   // wait for Wifi module

  Serial.println();
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  WiFi.disconnect(true, true);  // おまじない
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());
  delay(100);  
}

void wifiDisconnect(void) {

  delay(100);                   // wait for Wifi module
  WiFi.disconnect(true, true);  // おまじない
  delay(100);  
}
