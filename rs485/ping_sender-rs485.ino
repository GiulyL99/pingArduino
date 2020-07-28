#include <SoftwareSerial.h>
#define LED_PIN 13
#define SEP ";"

#define DE_RE 3
#define RS_TX 11
#define RS_RX 10

#define RS_Transmit HIGH
#define RS_Receive LOW

SoftwareSerial rs485Serial(RS_RX,RS_TX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
  pinMode(DE_RE,OUTPUT);
  
  rs485Serial.begin(9600); //verificar baud rate
}

void loop() {
  //Send message
  unsigned long send_millis=millis();
  
  char* TEXT="TESTE 123 ALOALO";
  char* buf_sum;
  char* TOTAL_MESSAGE;
  
  itoa(checksum(TEXT),buf_sum,16);
  strcat(TOTAL_MESSAGE,TEXT);
  strcat(TOTAL_MESSAGE,SEP);
  strcat(TOTAL_MESSAGE,buf_sum);
  
  digitalWrite(DE_RE,RS_Transmit);
  rs485Serial.write(TOTAL_MESSAGE); //send in format "TEXT;CHECKSUM(HEX)"
  digitalWrite(DE_RE,RS_Receive);

  
  //Get reply
  while (!rs485Serial.available())
  {
    int i=0;
  }
  
  char* received;
  int availableBytes=rs485Serial.available();
  for (int i=0;i<availableBytes;i++)
  {
    received[i]=rs485Serial.read();
  }
  
  char* split = strtok(received,SEP); //receive in format "TEXT;CHECKSUM(HEX);REC_CHECKED(BIT)"
  int i=0;
  while (split != NULL)
  {
    if (i==0)
    {
      if (split==TEXT)
      {
        digitalWrite(LED_PIN,HIGH);
        delay(100);
        digitalWrite(LED_PIN,LOW);
      }
    }
    if (i==1)
    {
      if (split==buf_sum)
      {
        digitalWrite(LED_PIN,HIGH);
        delay(100);
        digitalWrite(LED_PIN,LOW);
      }
    }
    if (i==2)
    {
      if (split=="1")
      {
        digitalWrite(LED_PIN,HIGH);
        delay(100);
        digitalWrite(LED_PIN,LOW);
      }
    }
    
    split = strtok (NULL,SEP);
    i++;
  }
  
  //End
  unsigned long elapsed_time=millis()-send_millis;
  Serial.println(received);
  Serial.print(elapsed_time);
  Serial.println(F(" ms"));
  
  delay(1000);

}

int checksum(char* str)
{
  int sum=0;
  unsigned int numOfChars = sizeof(str)/sizeof(str[0]);
  for(byte b=0; b<numOfChars; b++)
  {
   sum += str[b];
  }

  return sum;
}

