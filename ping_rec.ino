#include <SoftwareSerial.h>
#define LED_PIN 13
#define SEP ";"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Receive
  while (!Serial.available())
  {
    int i=0;
  }
  
  char* received;
  char* TEXT;
  char* buf_sum;
  int checked;
  
  int availableBytes=Serial.available();
  for (int i=0;i<availableBytes;i++)
  {
    received[i]=Serial.read();
  }
  
  char* split = strtok(received,SEP); //receive in format "TEXT;CHECKSUM(HEX)"
  int i=0;
  while (split != NULL)
  {
    if (i==0)
    {
      digitalWrite(LED_PIN,HIGH);
      delay(100);
      digitalWrite(LED_PIN,LOW);

      TEXT=split;
      itoa(checksum(TEXT),buf_sum,16);
      
    }
    if (i==1)
    {
      digitalWrite(LED_PIN,HIGH);
      delay(100);
      digitalWrite(LED_PIN,LOW);
      
      checked= buf_sum==split;
    }
        
    split = strtok (NULL,SEP);
    i++;
  }

  //Reply
  char* TOTAL_MESSAGE;
  char* checked_char;
  itoa(checked,checked_char,10);
  
  strcpy(TOTAL_MESSAGE,TEXT);
  strcpy(TOTAL_MESSAGE,SEP);
  strcpy(TOTAL_MESSAGE,buf_sum);
  strcpy(TOTAL_MESSAGE,SEP);
  strcpy(TOTAL_MESSAGE,checked_char);

  Serial.write(TOTAL_MESSAGE); //send in format "TEXT;CHECKSUM(HEX);REC_CHECKED(BIT)"
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
