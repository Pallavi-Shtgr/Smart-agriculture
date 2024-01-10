//Modern agriculture - water pump control through phone calls ,GSM moudle code

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
SoftwareSerial gsm(10, 11); // RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);

int address = 0; 
int wr_call_add = 1; 
int f_address = 2; 
int eeprom_dead = 3; 
int wr_call_val = 0;
int f_value = 70;
int value = 0;
int ring = 0;
int i = 0;
int var = 0;
int eeprom_tst = 0;
int eeprom_tst_ack = 0;
int tst_var = 100;
int wr_call_tst = 100;
int eeprom_dead_val = 0;
String number = "";
String string = "";
const int output = 5;
const int red = 7;
const int buzzer = 4;
const int green = 6;
boolean wait = true;
boolean at_flag = 1;
boolean net_flag = 1;

void setup()
{
Serial.begin(9600);
lcd.init();
lcd.backlight();
gsm.begin(9600);
pinMode(output, OUTPUT);
pinMode(red, OUTPUT);
pinMode(buzzer, OUTPUT);
pinMode(green, OUTPUT);
if (EEPROM.read(f_address) != f_value)
{
EEPROM.write(f_address, f_value);
EEPROM.write(address, value);
EEPROM.write(wr_call_add, wr_call_val);
eeprom_dead_val = 0;
EEPROM.write(eeprom_dead, eeprom_dead_val);
}
if (EEPROM.read(eeprom_dead) == 1)
{
while (true)
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("EEPROM Error.");
lcd.setCursor(0, 1);
lcd.print("System Disabled.");
delay(1500);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Reload the code");
lcd.setCursor(0, 1);
lcd.print("with new address");
delay(1500);
}
}
if (EEPROM.read(address) == 0)
{
digitalWrite(output, LOW);
digitalWrite(red, HIGH);
digitalWrite(buzzer, LOW);
delay(2000);
digitalWrite(green, LOW);
gsm_init();
lcd.clear();
if (EEPROM.read(wr_call_add) == 0)
{
lcd.setCursor(0, 0);
lcd.print("Sending SMS");
lcd.setCursor(0, 1);
lcd.print("Acknowledgement.");
delay(1000);
gsm.println("AT+CMGF=1");
delay(500);
gsm.print("AT+CMGS=");
gsm.print("\"");
gsm.print(str);
gsm.println("\"");
delay(1000);
gsm.println("Motor is OFF / System is Ready.");
delay(100);
gsm.println((char)26);
}
if (EEPROM.read(wr_call_add) == 1)
{
wr_call_val = 0;
EEPROM.write(wr_call_add, wr_call_val);
}
}
if (EEPROM.read(address) == 1)
{
eeprom_test();
output_begin();
}
}
void(* resetFunc) (void) = 0;
void loop()
{
serialEvent();
if (ring == 1)
{
number = "+91xxxxxxxxxx";
var = string.indexOf("+CLIP: \"");
if (var > 0)
{
number += string.substring(var + 8, var + 13 + 7);
}
if (number[0] == str[0] && number[1] == str[1] && number[2] == str[2] && number[3] == str[3]
&& number[4] == str[4] && number[5] == str[5] && number[6] == str[6] && number[7] == str[7]
&& number[8] == str[8] && number[9] == str[9] && number[10] == str[10] && number[11] == str[11])
{
gsm.println("ATH");
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Incomming call");
lcd.setCursor(0, 1);
lcd.print("Number Verified.");
delay(2000);
if (EEPROM.read(address) == 0)
{
EEPROM.write(address, 1);
}
else if (EEPROM.read(address) == 1)
{
EEPROM.write(address, 0);
}
resetFunc();
}
if (!(number[0] == str[0] && number[1] == str[1] && number[2] == str[2] && number[3] == str[3]
&& number[4] == str[4] && number[5] == str[5] && number[6] == str[6] && number[7] == str[7]
&& number[8] == str[8] && number[9] == str[9] && number[10] == str[10] && number[11] == str[11]))
{
gsm.println("ATH");
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Unknown number");
lcd.setCursor(0, 1);
lcd.print("Call Rejected.");
wr_call_val = 1;
EEPROM.write(wr_call_add, wr_call_val);
delay(2000);
resetFunc();
}
}
if (EEPROM.read(address) == 0)
{
serialEvent();
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("System Standby");
lcd.setCursor(0, 1);
lcd.print("Status: OFF");
delay(1000);
}
else if (EEPROM.read(address) == 1)
{
lcd.setCursor(0, 0);
lcd.print("STATUS: ON");
lcd.setCursor(0, 1);
lcd.print("----------------");
}
}
void gsm_init()
{
lcd.print("System booting....");
lcd.setCursor(0, 1);
lcd.print("initiating......");
delay(1500);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Checking Module");
lcd.setCursor(0, 1);
lcd.print("Connectivity....");
while (at_flag)
{
gsm.println("AT");
while (gsm.available() > 0)
{
if (gsm.find("OK"))
at_flag = 0;
}
delay(1000);
}
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Module");
lcd.setCursor(0, 1);
lcd.print("Connection: OK");
delay(1500);
eeprom_test();
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("System is Ready");
lcd.setCursor(0, 1);
lcd.print("----------------");
delay(1500);
}
void serialEvent()
{
while (gsm.available())
{
char read_char = gsm.read();
string += read_char;
i++;

if (string[i - 4] == 'R' && string[i - 3] == 'I' && string[i - 2] == 'N' && string[i - 1] == 'G' )
{
ring = 1;
}
}
}
void output_begin()
{
digitalWrite(red, LOW);
digitalWrite(buzzer, HIGH);
delay(2000);
digitalWrite(buzzer, LOW);
digitalWrite(green, HIGH);
if (EEPROM.read(wr_call_add) == 0)
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Sending SMS");
lcd.setCursor(0, 1);
lcd.print("Acknowledgement.");
delay(1000);
gsm.println("AT+CMGF=1");
delay(500);
gsm.print("AT+CMGS=");
gsm.print("\"");
gsm.print(str);
gsm.println("\"");
delay(1000);
gsm.println("Motor is ON.");
delay(100);
gsm.println((char)26);
}
if (EEPROM.read(wr_call_add) == 1)
{
wr_call_val = 0;
EEPROM.write(wr_call_add, wr_call_val);
}
lcd.clear();
}
void eeprom_test()
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Testing EEPROM");
lcd.setCursor(0, 1);
lcd.print("Memory.");
delay(1500);
eeprom_tst = EEPROM.read(address);
eeprom_tst_ack = EEPROM.read(wr_call_add);
EEPROM.write(address, tst_var);
EEPROM.write(wr_call_add, wr_call_tst);
if (EEPROM.read(address) == tst_var && EEPROM.read(wr_call_add) == wr_call_tst)
{
EEPROM.write(address, eeprom_tst);
EEPROM.write(wr_call_add, eeprom_tst_ack);
if (EEPROM.read(address) != eeprom_tst || EEPROM.read(wr_call_add) != eeprom_tst_ack)
{
digitalWrite(output, LOW);
digitalWrite(red, HIGH);
digitalWrite(buzzer, LOW);
delay(2000);
digitalWrite(green, LOW);
eeprom_dead_val = 1;
EEPROM.write(eeprom_dead, eeprom_dead_val);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("EEPROM Test is");
lcd.setCursor(0, 1);
lcd.print("Unsuccessful.");
delay(1500);
}
else if (EEPROM.read(address) == eeprom_tst && EEPROM.read(wr_call_add) == eeprom_tst_ack)
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("EEPROM Test is");
lcd.setCursor(0, 1);
lcd.print("Successful.");
delay(1500);
}
}
else if (EEPROM.read(address) != tst_var || EEPROM.read(wr_call_add) != wr_call_tst)
{
digitalWrite(output, LOW);
digitalWrite(red, HIGH);
digitalWrite(buzzer, LOW);
delay(2000);
digitalWrite(green, LOW);
eeprom_dead_val = 1;
EEPROM.write(eeprom_dead, eeprom_dead_val);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("EEPROM Test is");
lcd.setCursor(0, 1);
lcd.print("Unsuccessful.");
delay(1500);
}
}
void eeprom_sms()
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Sending SMS");
lcd.setCursor(0, 1);
lcd.print("Acknowledgement");
delay(1000);
gsm.println("AT+CMGF=1");
delay(500);
gsm.print("AT+CMGS=");
gsm.print("\"");
gsm.print(str);
gsm.println("\"");
delay(1000);
gsm.println("EEPROM error. System disabled. Please reload the code with new address.");
delay(100);
gsm.println((char)26);

{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("EEPROM Error.");
lcd.setCursor(0, 1);
lcd.print("System Disabled.");
delay(1500);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Reload the code");
lcd.setCursor(0, 1);
lcd.print("with new address");
delay(1500);
}
}