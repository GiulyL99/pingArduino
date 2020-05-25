#include <SoftwareSerial.h>
#define LED_PIN 13
#define SEP ";"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  //Send message
  unsigned long send_millis=millis();
  
  char* TEXT="MY TEXT HERE";
  char* buf_sum;
  char* TOTAL_MESSAGE;
  
  itoa(checksum(TEXT),buf_sum,16);
  strcpy(TOTAL_MESSAGE,TEXT);
  strcpy(TOTAL_MESSAGE,SEP);
  strcpy(TOTAL_MESSAGE,buf_sum);

  Serial.write(TOTAL_MESSAGE); //send in format "TEXT;CHECKSUM(HEX)"

  
  //Get reply
  while (!Serial.available())
  {
    int i=0;
  }
  
  char* received;
  int availableBytes=Serial.available();
  for (int i=0;i<availableBytes;i++)
  {
    received[i]=Serial.read();
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

