/////ライブラリ/////
#include <Arduino.h>
#include <string.h>
#include <Rotary.h>            //ロータリーエンコーダのライブラリ
#include <LovyanGFX.hpp>
#include "News_Get.hpp"
#include "time_get.hpp"
//#include "blue.hpp"         //Bluetooth設定 WiFiとBluetoothの併用は難しそう
//////////////////



////////////////LCD・定義設定//////////////////////////
#define LCD_FREQ      15000000
#define FONT          &fonts::lgfxJapanMincho_40
#define FONT_SIZE     40                // 40 or 48
#define ENC_A 18
#define ENC_B 19
#define ok 21
#define back 23
#define menu0 34
#define menu1 35
//portTickType xLastWakeWebGetTime1;   //マルチタスク設定
///////////////////////////////////////////////


//////////////////////関数のプロトタイプ宣言///////////////////

void watting(void);
void setting(void);
void Get_HTTP(void);
void rotary_encoder(void);
void check_ok(void);
void check_back(void);
void menu_plus(void);
void menu_minus(void);

//////////////////////関数のプロトタイプ宣言///////////////////


///////////タイマー割込み1//////////////////////////////////

volatile int timeCounter1;
hw_timer_t *timer1 = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile SemaphoreHandle_t timerSemaphore;

void IRAM_ATTR onTimer1() {

  portENTER_CRITICAL_ISR(&timerMux);
  timeCounter1++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

///////////タイマー割込み1//////////////////////////////////


///////////タイマー割込み2--Bluetooth用に作ったやつ//////////////////////////////////
/*
volatile int timeCounter2;
hw_timer_t *timer2 = NULL;

void IRAM_ATTR onTimer2() {

  portENTER_CRITICAL_ISR(&timerMux);
  timeCounter2++;
  portEXIT_CRITICAL_ISR(&timerMux);
}
*/
///////////タイマー割込み2--Bluetooth用に作ったやつ//////////////////////////////////





/////////グローバル変数//////////////////
Rotary r = Rotary(ENC_A, ENC_B);
uint32_t WebGet_LastTime = 0;
int32_t scroll_step = 1;
unsigned char modeinfo = 0;
String str, str2;
unsigned char kaisou0 = 0;
unsigned char kaisou1 = 0;
unsigned char kaisou2 = 0;
unsigned int time_ok;
unsigned int time_back;
unsigned int menu_0;
unsigned int menu_1;
unsigned char flag_create_buf3 = 0;
unsigned char setting_num = 0;
bool setting_bool;
//////////////////////////////////////


///////LGFX設定/////////////////////////////////////
struct LGFX_Config {
  static constexpr int gpio_wr  =  4; //to LCD WR(14)
  static constexpr int gpio_rd  =  2; //to LCD RD(13)
  static constexpr int gpio_rs  = 15; //to LCD DC(16)
  static constexpr int gpio_d0  = 12; //to LCD D0(4)
  static constexpr int gpio_d1  = 13; //to LCD D1(5)
  static constexpr int gpio_d2  = 26; //to LCD D2(6)
  static constexpr int gpio_d3  = 25; //to LCD D3(7)
  static constexpr int gpio_d4  = 17; //to LCD D4(8)
  static constexpr int gpio_d5  = 16; //to LCD D5(9)
  static constexpr int gpio_d6  = 27; //to LCD D6(10)
  static constexpr int gpio_d7  = 14; //to LCD D7(11)
};

static lgfx::LGFX_PARALLEL<LGFX_Config> lcd;
static lgfx::Panel_ILI9342 panel;
static lgfx::LGFX_Sprite buf;       // [640 * 32] Buffer
static lgfx::LGFX_Sprite buf2;       // [640 * 16] Buffer
static lgfx::LGFX_Sprite cBuf;      // ex [40 * 40] Buffer
static lgfx::LGFX_Sprite buf3;   //設定用スプライト


inline void lcd_buffer_write();
////////////////////////////////////////////////////



///////セットアップ/////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  WebGet_LastTime = 200000; //起動時に記事をGETするために、多めの数値で初期化しておく

  // 通常動作時のSPIクロックを設定します。
  // ESP32のSPIは80MHzを整数で割った値のみ使用可能です。
  // 設定した値に一番近い設定可能な値が使用されます。
  panel.freq_write = LCD_FREQ;            // WiFiと同居時は8MHzに設定

  // 単色の塗り潰し処理時のSPIクロックを設定します。
  // 基本的にはfreq_writeと同じ値を設定しますが、
  // より高い値を設定しても動作する場合があります。
  panel.freq_fill  = 27000000;

  // LCDから画素データを読取る際のSPIクロックを設定します。
  panel.freq_read  = 16000000;

  // 画素読出し時のダミービット数を設定します。
  // 画素読出しでビットずれが起きる場合に調整してください。
  panel.len_dummy_read_pixel = 8;

  // LCDのCSを接続したピン番号を設定します。
  // 使わない場合は省略するか-1を設定します。
  panel.spi_cs = 33;  //to LCD CS(15)

  // LCDのD/Cを接続したピン番号を設定します
  panel.spi_dc = -1;

  // LCDのRSTを接続したピン番号を設定します。
  // 使わない場合は省略するか-1を設定します。
  panel.gpio_rst = 32;  //to LCD RST(2)

  // LCDのバックライトを接続したピン番号を設定します。
  // 使わない場合は省略するか-1を設定します。
  panel.gpio_bl  = -1;

  // バックライト使用時、輝度制御に使用するPWMチャンネル番号を設定します。
  // PWM輝度制御を使わない場合は省略するか-1を設定します。
  panel.pwm_ch_bl = -1;

  // バックライト点灯時の出力レベルがローかハイかを設定します。
  // 省略時は true。true=HIGHで点灯 / false=LOWで点灯になります。
  panel.backlight_level = true;

  // LCDドライバチップ内のメモリサイズ（幅と高さ）を設定します。
  // 設定が合っていない場合、setRotationを使用した際の座標がずれます。
  // （例：ST7735は 132x162 / 128x160 / 132x132 の３通りが存在します）
  panel.memory_width  = 320;
  panel.memory_height = 240;

  // パネルが実際に表示可能なピクセル数（幅と高さ）を設定します。
  // 省略時はパネルクラスのデフォルト値が使用されます。
  panel.panel_width  = 320;
  panel.panel_height = 96;

  // パネルのオフセット量を設定します。
  // 省略時はパネルクラスのデフォルト値が使用されます。
  panel.offset_x = 0;
  panel.offset_y = 140;    //最初144

  // setRotationの初期化直後の値を設定します。
  panel.rotation = 0;

  // setRotationを使用した時の向きを変更したい場合、offset_rotationを設定します。
  // setRotation(0)での向きを 1の時の向きにしたい場合、 1を設定します。
  panel.offset_rotation = 0;

  // 設定を終えたら、lcdのsetPanel関数でパネルクラスのポインタを渡します。
  lcd.setPanel(&panel);

  // SPIバスとパネルの初期化を実行すると使用可能になります。
  lcd.init();

  //色は16bit指定,R(5bit)G(6bit)B(5bit)で指定
  lcd.setColorDepth(16);

  //LCDの表示回転を行うか？
  lcd.setRotation(0); // 0 or 2


  ////////スプライト設定↓↓↓↓↓

  //bufのカラー指定を16bitで行う。
  buf.setColorDepth(16);
  //buf2のカラー指定を16bitで行う。
  buf2.setColorDepth(16);
  //buf3のカラー指定を16bitで行う。
  //buf3.setColorDepth(16);

  //buf,buf2に640*32と640,16のスプライトを作製
  buf.createSprite(640, 32);     //標準(640,48)
  buf2.createSprite(640, 16);     //標準(640,48)
  cBuf.createSprite(FONT_SIZE, FONT_SIZE);
  //buf3.createSprite(640,32);

  //それぞれのフォントを指定。
  buf2.setFont(&fonts::lgfxJapanMincho_16);
  //buf3.setFont(&fonts::lgfxJapanGothic_40);

  //フォントサイズ指定
  buf2.setTextSize(1, 0.9);
  //  buf.setFont(&fonts::lgfxJapanGothic_40);        // 40ドットフォント設定
  cBuf.setFont(FONT);       // 40ドットフォント設定
  if (FONT_SIZE == 40) {    //max 48
    cBuf.setTextSize(1, 0.6);    //標準(1.2, 1.2)
  }
  //buf3.setTextSize(1, 0.6);

  //文字と背景色を選択
  cBuf.setTextColor(TFT_RED, TFT_WHITE);
  //buf3.setTextColor(TFT_GREEN, TFT_WHITE);
  cBuf.setTextWrap(false);
  buf.setScrollRect(0, (48 - FONT_SIZE) / 2, 640, FONT_SIZE);               // スクロール幅設定


///////////////////////////ロータリーエンコーダ設定/////////////////

  r.begin();
  attachInterrupt(ENC_A, rotary_encoder, CHANGE);                   //ロータリーエンコーダの設定
  attachInterrupt(ENC_B, rotary_encoder, CHANGE);

///////////////////////////ロータリーエンコーダ設定/////////////////

///////////////////////////タイマー割込み1設定/////////////////

  timer1 = timerBegin(0, 80, true);

  timerAttachInterrupt(timer1, &onTimer1, true);

  timerAlarmWrite(timer1, 1000000, true);    //1000ms

  timerAlarmEnable(timer1);

///////////////////////////タイマー割込み1設定/////////////////


///////////////////////////タイマー割込み2設定--Bluetooth用に作ったやつ/////////////////
/*
  timer2 = timerBegin(1, 80, true);

  timerAttachInterrupt(timer2, &onTimer2, true);

  timerAlarmWrite(timer2, 10000000, true);    //10000ms

  timerAlarmEnable(timer2);
*/
///////////////////////////タイマー割込み2設定--Bluetooth用に作ったやつ/////////////////

/////////////////////////////マルチコア設定/////////////////////////
/*
  TaskHandle_t th; //マルチタスクハンドル定義
  xTaskCreatePinnedToCore(Task1, "Task1", 4096, NULL, 5, &th, 0); //マルチタスク実行
 
  xLastWakeWebGetTime1 = xTaskGetTickCount(); //マルチタスク時間カウント代入
*/
/////////////////////////////マルチコア設定/////////////////////////

/////////////////////////////ボタン割込み設定/////////////////////////

pinMode(ok, INPUT);
pinMode(back, INPUT);

attachInterrupt(ok, check_ok, FALLING);
attachInterrupt(back, check_back, FALLING);

pinMode(menu0, INPUT);
pinMode(menu1, INPUT);

attachInterrupt(menu0, menu_plus, FALLING);
attachInterrupt(menu1, menu_minus, FALLING);

 time_back = millis()/1000;
 time_ok = millis()/1000;

 menu_0 = millis()/1000;
 menu_1 = millis()/1000;

/////////////////////////////ボタン割込み設定/////////////////////////




}

///////セットアップ/////////////////////////////////////////////////////////



//////ロータリーエンコーダ・ピン変化割込み設定/////////////
    void rotary_encoder(){
      unsigned char result = r.process();
      if(result){
      if(result == DIR_CW){
        
        if(kaisou0 == 0){
        modeinfo = modeinfo-1;
        str = "";
        }

        if(kaisou0 == 2){
          setting_num = setting_num - 1;
        }
        
      }else{
        
        if(kaisou0 == 0){
        modeinfo = modeinfo+1;
        str = "";
        }

        if(kaisou0 == 2){
          setting_num = setting_num + 1;
        }
    }
  }
}
////////ロータリーエンコーダ・ピン変化割込み設定////////////




///////カーソル位置取得・移動関数///////////////////////
 void lcd_write() {

  Serial.println("lcd_write()開始");


  int32_t text_position = 0;
  int32_t cursor_position = 0;
  cBuf.setCursor(cursor_position, 0);                      // カーソル位置を初期化
  while (str[text_position] != '\0') {
    // マルチバイト文字1文字を取り出し1文字バッファに表示
    cBuf.print(str[text_position]);                          // 1バイトずつ出力 (マルチバイト文字でもこの処理で動作します)
    cursor_position = cBuf.getCursorX();                   // 出力後のカーソル位置を取得
    text_position++;
    // 細長LCDの右端に1文字分表示
    if (cursor_position != 0) {                            // 表示する文字があるか？
      for (int i = 0; i < cursor_position; i++) {          // 文字幅分スクロール表示
        buf.scroll( -scroll_step, 0);                         // 1ドットスクロール

         if (timeCounter1 > 0) {
          portENTER_CRITICAL(&timerMux);
          timeCounter1--;
          portEXIT_CRITICAL(&timerMux);
           //Serial.println("time_now実行開始");
           str2 = time_now();
           buf2.setCursor(0,0);
           buf2.print(str2);
           buf2.pushSprite(&buf, 0, 32);
           lcd_buffer_write();
           Serial.println(str2);
            }

            

/* Bluetoothの割込み　WiFiとの併用は難しそう
           if (timeCounter2 > 0){
              wifiDisconnect();
              delay(200);
              bluetooth();
           }
*/
        
        cBuf.pushSprite(&buf, 639 - i, (48 - FONT_SIZE) / 2); // 1文字バッファをbuf右端に転送
       lcd_buffer_write();                           // LCDへデータ転送
      }
    }
    // 次の文字表示の準備
    cursor_position = 0;
    cBuf.setCursor(cursor_position, 0);                    // カーソル位置を初期化
  }
  Serial.println("lcd_write() 終了");
}

///////カーソル位置取得・移動関数///////////////////////




///////////////////////////メイン関数//////////////////////////////////////////

void loop() {

/*
    Get_HTTP();
    lcd_write();    //この処理が時間の大半を占める。
*/


///*
     switch(kaisou0){

      case 0:{
              buf3.deleteSprite();
              Get_HTTP();
              lcd_write();    //この処理が時間の大半を占める。
              break;
              }

      case 1:{
              watting();
              break;
              }

      case 2:{
              setting();
              break;
              }

      case 255:{
                kaisou0 = 2;
                break;
               }

      default:{
              kaisou0 = 0;
              break;
              }

     }
//*/

}
///////////////////////////メイン関数//////////////////////////////////////////


////////LCD描写関数1///////////////////////////
inline void lcd_buffer_write() {
  lcd.startWrite();
  lcd.setAddrWindow(0, 0, 320, 96);      //標準(0,0,320,96) これまでは(320,65)
  lcd.writePixels((lgfx::swap565_t*)buf.getBuffer(), 640 * 48);   //標準(640,48)これまでは(320,65)
  lcd.endWrite();
}
////////LCD描写関数1///////////////////////////


///////////////////////////マルチコア関数//////////////////////////////////////////
/*
void Task1(void *pvParameters){

}
*/
///////////////////////////マルチコア関数//////////////////////////////////////////



//////記事取得/////////////////////////////////////
void Get_HTTP(){

           str = "現在記事取得中・・・";
           wifiConnect();
    switch(modeinfo){
      case 0:
           str = https_Web_Get("akiba-pc.watch.impress.co.jp", "/data/rss/1.0/ah/feed.rdf", '\n', "</rdf:RDF>", "<title>", "</title>", "<description>", "</description>", "◇", 0);    //AKIBA記事取得
           break;
           
      case 1:
           str = https_Web_Get("news.yahoo.co.jp", "/rss/topics/top-picks.xml", '\n', "</rss >", "<title>", "</title>", "<description>", "</description>", "◇", 1);   //Yahoo記事取得
           break;

      case 2:
           str = https_Web_Get("gigazine.net", "/news/rss_atom/", '\n', "</feed>", "<title>", "</title>", "<br />", "<p>", "◇", 2);   //GIGAZINE記事取得
           break;

      case 3:
           str = https_Web_Get("www.gizmodo.jp", "/index.xml", '\n', "</rss>", "<title>", "</title>", "<description>", "</description>", "◇", 3);   //GIZMODO記事取得
           break;

      case 4:
           modeinfo = 0;
           break;

      case 255:
           modeinfo = 3;
           break;

      default:
           modeinfo = 0;
           break;
    }
           str = str + "　　　　　　　　　　　　　　　　　　　";
           wifiDisconnect();
}
//////記事取得/////////////////////////////////////




/////////ok・backボタン判定関数////////////////////////

void check_ok(){
  if(millis()/1000 - time_ok > 1){
  setting_bool = true;
  Serial.println("true");
  time_ok = millis()/1000;
  }
}
void check_back(){
  if(millis()/1000 - time_back > 1){
  setting_bool = false;
  Serial.println("false");
  time_back = millis()/1000;
  }
}

/////////ok・backボタン判定関数////////////////////////


/////////menuボタン判定関数////////////////////////

void menu_plus(){
  if(millis()/1000 - menu_0 > 1){
  kaisou0++;
  flag_create_buf3 = 0;
  Serial.println("kaisou++");
  str = "";
  menu_0 = millis()/1000;
  }
}
void menu_minus(){
  if(millis()/1000 - menu_1 > 1){
  kaisou0--;
  flag_create_buf3 = 0;
  Serial.println("kaisou--");
  str = "";
  menu_1 = millis()/1000;
  }
}

/////////menuボタン判定関数////////////////////////


//////////設定画面関数/////////////////////////////////

void setting(){


   if(flag_create_buf3 == 0){

   buf3.deleteSprite();
   
   buf3.setColorDepth(16);
   buf3.createSprite(640,32);
   buf3.setFont(&fonts::lgfxJapanGothic_40);
   buf3.setTextSize(1, 0.6);
   buf3.setTextColor(TFT_GREEN, TFT_WHITE);

   flag_create_buf3 = 1;
   }

   char *setting[] = {"1.画面の明るさ　　　　　　", "2.時間設定　　　　　　　", "3.WiFi設定　　　　　　", "4.こんにちは　　　　　　"};

/*
  if (timeCounter1 > 0) {
  portENTER_CRITICAL(&timerMux);
  timeCounter1--;
  portEXIT_CRITICAL(&timerMux);
*/

    str = "設定項目:";
      switch(setting_num){
        case 0:{
                str = str + setting[setting_num];
                str2 = setting[setting_num+1];
                break;
               }
        case 1:{
                str = str + setting[setting_num];
                str2 = setting[setting_num+1];
                break;
               }
        case 2:{
                str = str + setting[setting_num];
                str2 = setting[setting_num+1];
                break;
               }
        case 3:{
                setting_num = 0;
                break;
               }
        case 255:{
                  setting_num = 2;
                  break;
                 }
      }
    str2 = str2 + "　　　　　　　　　　　　　　　　　　　";
    buf3.setCursor(0, 0);
    buf3.println(str);
    buf3.pushSprite(&buf, 0, (48 - FONT_SIZE) / 2);
    lcd_buffer_write();
    buf2.setCursor(0,0);
    buf2.println(str2);
    buf2.pushSprite(&buf, 0, 32);
    lcd_buffer_write();
//  }
}

//////////設定画面関数/////////////////////////////////

///////////時計画面関数///////////////////////////////

void watting(){

   if(flag_create_buf3 == 0){
      buf3.deleteSprite();
      
      buf3.setColorDepth(16);
      buf3.createSprite(640,32);
      buf3.setFont(&fonts::lgfxJapanGothic_40);
      buf3.setTextSize(0.8, 0.8);
      buf3.setTextColor(TFT_GREEN, TFT_BLACK);

      flag_create_buf3 = 1;
   }

   str = time_now();
   str2 = "　　　　　　↑RSS　↓設定　　　　　　";
   buf3.setCursor(0, 0);
   buf3.println(str);
   buf3.pushSprite(&buf, 0, (48 - FONT_SIZE) / 2);
   lcd_buffer_write();
   buf2.setCursor(0,0);
   buf2.println(str2);
   buf2.pushSprite(&buf, 0, 32);
   lcd_buffer_write();

}

///////////時計画面関数///////////////////////////////
