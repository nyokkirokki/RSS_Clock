// ###########################################################
//
//      NTPサーバーから時刻取得 time_get.cpp
//
//        
//
// ###########################################################



#include <Arduino.h>
#include <string.h>
#include <time.h>
#include "time_get.hpp"
#include "News_Get.hpp"


int nYear = 0;
byte nMon = 0;
int nDay = 0;
byte nHour = 0;
byte nMin = 0;
int  nSec = 0;

unsigned int gettime = 0;
unsigned char flag = 0;


String time_now() { 

  
  String tt = "";

again:

  if((millis()/1000) - gettime > 1000 || flag == 0){
    wifiConnect();
    configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");//NTPの設定
    wifiDisconnect();
    gettime = (millis()/1000);
    flag = 1;
  }

  struct tm timeInfo;

  getLocalTime(&timeInfo);

  if(timeInfo.tm_year == 70){
    flag = 0;
    goto again;
  }

    nYear = timeInfo.tm_year + 1900;
    nMon  = timeInfo.tm_mon + 1;
    nDay  = timeInfo.tm_mday;
    nHour = timeInfo.tm_hour;
    nMin  = timeInfo.tm_min;
    nSec  = timeInfo.tm_sec;

    String NYear = String(nYear);
    String NMon  = String(nMon);
    String NDay = String(nDay);
    String NHour = String(nHour);
    String NMin = String(nMin);
    String NSec = String(nSec);


    
    tt = "現在時間:　";
    tt = tt + NYear + "年" + NMon + "月" + NDay + "日";
    tt = tt + "　";
    tt = tt + NHour + "時" + NMin + "分" + NSec + "秒";
    tt = tt + "　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　";
    
  return tt;

  
}
