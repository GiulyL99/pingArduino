#include <SoftwareSerial.h>
#include <string.h>

#define LED_PIN 13
#define RX 10
#define TX 11
#define SEP ";"

// initialize the library with the numbers of the interface pins
SoftwareSerial srl(RX,TX);

void setup() {
  // set up the LCD's number of columns and rows:
  // Print a message to the LCD.
  
  //Inicializa a serial do Arduino
  srl.begin(4800);
  Serial.begin(9600);
	
  pinMode(LED_PIN,OUTPUT);
  
}

void loop() { 
  //Send message
  unsigned long send_millis=millis();
  
  char TEXT[]="TESTE 123 ALOALO";
  char buf_sum[5];
  char TOTAL_MESSAGE[128];
  
  itoa(checksum(TEXT),buf_sum,16);
  strcpy(TOTAL_MESSAGE,TEXT);
  strcat(TOTAL_MESSAGE,SEP);
  strcat(TOTAL_MESSAGE,buf_sum);

  int delays=0;
  int sentBytes=srl.print(TOTAL_MESSAGE); //send in format "TEXT;CHECKSUM(HEX)"
	//Serial.println(sentBytes);
  Serial.print("sent: ");
  Serial.println(TOTAL_MESSAGE);
  digitalWrite(LED_PIN,HIGH);
        delay(100);
        digitalWrite(LED_PIN,LOW);
        delays+=1;
  
  //Get reply
  while (!srl.available())
  {
    int i=0;
  }
  
  char received[128];
  int j=0;
  while (srl.available())
  {
    received[j]=srl.read();
    delay(1);
    j++;
  }
  
  Serial.print("recv: ");
  Serial.println(received);
  
  char* split = strtok(received,SEP); //receive in format "TEXT;CHECKSUM(HEX);REC_CHECKED(BIT)"
  
  int i=0;
  
  while (split != NULL)
  {
    //strcpy(split,fatia);
    //Serial.println(split);
    
    if (i==0)
    {
      if (split==TEXT)
      {
        digitalWrite(LED_PIN,HIGH);
        delay(100);
        digitalWrite(LED_PIN,LOW);
        delays+=1;
      }
    }
    if (i==1)
    {
      if (split==buf_sum)
      {
        digitalWrite(LED_PIN,HIGH);
        delay(100);
        digitalWrite(LED_PIN,LOW);
        delays+=1;
      }
    }
    if (i==2)
    {
      if (split=="1")
      {
        digitalWrite(LED_PIN,HIGH);
        delay(100);
        digitalWrite(LED_PIN,LOW);
        delays+=1;
      }
    }
    
    split = strtok (NULL,SEP);
    i++;
  }
  
  //End
  unsigned long elapsed_time=millis()-send_millis-(delays*100);
    Serial.print(elapsed_time);
  Serial.println(" ms");  
  
  delay(1000);
}

int checksum(char* str)
{
  int sum=0;
  unsigned int numOfChars = strlen(str);
  //Serial.println(numOfChars);
  for(byte b=0; b<numOfChars; b++)
  {
   sum += str[b];
    //Serial.println(str[b]);
  }
  
  //Serial.println(sum);
  return sum;
}
