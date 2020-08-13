#include <SoftwareSerial.h>
#include <string.h>

#define LED_PIN 13
#define RX 10
#define TX 11
#define DE_RE 3
#define SEP ";"

#define RS_Transmit HIGH
#define RS_Receive LOW

// initialize the library with the numbers of the interface pins
SoftwareSerial srl(RX,TX);

void setup() {
  // set up the LCD's number of columns and rows:
  // Print a message to the LCD.
  //lcd.print("hello, world!");

  //Inicializa a serial do modulo RS485
  srl.begin(4800);
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
  pinMode(DE_RE,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  //Receive
  while (!srl.available())
  {
    int i=0;
    digitalWrite(LED_PIN,HIGH);
    //delay(100);
    //lcd.setCursor(0, 0);
  	//lcd.print(srl.available());
  }
    digitalWrite(LED_PIN,LOW);
  //lcd.setCursor(0, 0);
  	//lcd.print(srl.available());
  
  char received[128];
  char TEXT[128];
  int buf_sum;
  int checked;
  
  int j=0;
  digitalWrite(DE_RE,RS_Receive);
  while (srl.available())
  {
    received[j]=srl.read();
    delay(1);
    j++;
  }
  
  Serial.print("recv: ");
  Serial.println(received);
  
  char* split = strtok(received,SEP); //receive in format "TEXT;CHECKSUM(HEX)"
  
  int i=0;
  while (split != NULL)
  {
    //strcpy(split,fatia);
    //Serial.println(split);
    
    if (i==0)
    {
      /*digitalWrite(LED_PIN,HIGH);
      delay(100);
      digitalWrite(LED_PIN,LOW);*/

      strcpy(TEXT,split);
      //Serial.println(TEXT);
      buf_sum=checksum(TEXT);
      
    }
    if (i==1)
    {
      /*digitalWrite(LED_PIN,HIGH);
      delay(100);
      digitalWrite(LED_PIN,LOW);*/

      //int rec_sum=atoi(split);
      
      int rec_sum=strtol(split,NULL,16);
      //Serial.println(split);
      //Serial.println(buf_sum);
      //Serial.println(rec_sum);
      checked= int(buf_sum==rec_sum);
    }
        
    split = strtok (NULL,SEP);
    i++;
  }

  //Reply
  char TOTAL_MESSAGE[128];
  char checked_char[2];
  itoa(checked,checked_char,10);
  char buf_sum_char[5];
  itoa(buf_sum,buf_sum_char,16);
  
  strcpy(TOTAL_MESSAGE,TEXT);
  strcat(TOTAL_MESSAGE,SEP);
  strcat(TOTAL_MESSAGE,buf_sum_char);
  strcat(TOTAL_MESSAGE,SEP);
  strcat(TOTAL_MESSAGE,checked_char);
	
  digitalWrite(DE_RE,RS_Transmit);
  srl.print(TOTAL_MESSAGE); //send in format "TEXT;CHECKSUM(HEX);REC_CHECKED(BIT)"
  digitalWrite(DE_RE,RS_Receive);
  Serial.print("sent: ");
  Serial.println(TOTAL_MESSAGE);

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
