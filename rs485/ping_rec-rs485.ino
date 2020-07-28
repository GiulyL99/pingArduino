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
  // put your main code here, to run repeatedly:

  //Receive
  while (!rs485Serial.available())
  {
    int i=0;
  }
  
  char* received;
  char* TEXT;
  char* buf_sum;
  int checked;
  
  digitalWrite(DE_RE,RS_Receive);
  int availableBytes=rs485Serial.available();
  for (int i=0;i<availableBytes;i++)
  {
    received[i]=rs485Serial.read();
  }
  
  Serial.println(received);
  
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
  
  strcat(TOTAL_MESSAGE,TEXT);
  strcat(TOTAL_MESSAGE,SEP);
  strcat(TOTAL_MESSAGE,buf_sum);
  strcat(TOTAL_MESSAGE,SEP);
  strcat(TOTAL_MESSAGE,checked_char);

  digitalWrite(DE_RE,RS_Transmit);
  rs485Serial.write(TOTAL_MESSAGE); //send in format "TEXT;CHECKSUM(HEX);REC_CHECKED(BIT)"
  digitalWrite(DE_RE,RS_Receive);
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
