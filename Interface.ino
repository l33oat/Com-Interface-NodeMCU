/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Sketch generator:           http://examples.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                                http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example runs directly on NodeMCU.
 *
 * Note: This requires ESP8266 support package:
 *   https://github.com/esp8266/Arduino
 *
 * Please be sure to select the right NodeMCU module
 * in the Tools -> Board menu!
 *
 * For advanced settings please follow ESP examples :
 *  - ESP8266_Standalone_Manual_IP.ino
 *  - ESP8266_Standalone_SmartConfig.ino
 *  - ESP8266_Standalone_SSL.ino
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 *
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <String.h>
#include <stdlib.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "12f00bc52eff40b089a065c0be5c14a5";

// Your WiFi credentials.
// Set password to "" for open networks.

char ssid[] = "Boat-iPhone";
char pass[] = "0971786888";
//char ssid[] = "NetworkLab";
//char pass[]="NwLabOnly";

byte sendPins[4] = {D1, D2, D3, D4};
byte statePin = D0;

char password[100];
char remainMoney[][3][100]={{"007-8-51631-3","123456","50000"},{"693-0-24366-3","654321","100000"},{"123-4-56789-0","456789","45896"},{"098-7-65432-1","987654","85693"}};
char previousRemainMoney[][3][100]={{"007-8-51631-3","123456","50000"},{"693-0-24366-3","654321","100000"},{"123-4-56789-0","456789","45896"},{"098-7-65432-1","987654","85693"}};
int  posRemainMoney=0;
int  posRemainMoneyAccount=0;
int count=0;
boolean state= 999;
boolean stateAccount=999;
int sMode=0;
char inMoney[100];
int countInMoney=0;
int statusAccount=0;
int logIn=0;
int already=0;
int forCheckAccount=100;

void checkPassLength(char password[])
{
  if(count==6)
  {
    for(int i=0;i<5;i++)
    {
      state=strcmp(password,remainMoney[i][1]);
      if(state==0) 
      {
        count=7;
        posRemainMoney=i;
        break;
      }
    }
    
    if(state==0) 
    {
      count=7;
      Blynk.virtualWrite(V16,"Correct!!");
      Blynk.virtualWrite(V16," ");
      Blynk.virtualWrite(V14,remainMoney[posRemainMoney][2]);
      Blynk.setProperty(V13, "labels","-", "Transfer", "Withdraw","Deposit");
      Blynk.virtualWrite(V17,remainMoney[posRemainMoney][0]);
      int graph=atoi(remainMoney[posRemainMoney][2]);
      int graphPrev=atoi(previousRemainMoney[posRemainMoney][2]);
      Blynk.virtualWrite(V18,graphPrev);
      sprintf(previousRemainMoney[posRemainMoney][2],"%d", graph);
      Blynk.virtualWrite(V21,graph-graphPrev);
       Blynk.virtualWrite(V19,graph);
       Blynk.virtualWrite(V20,graph);
      logIn=1;
    }
    else 
    {
      Blynk.virtualWrite(V16,"Wrong password");
      count=0;
      password="";
      Blynk.virtualWrite(V16,password);
    }
  }
}
void printAccount(char number)
{
  if(strlen(inMoney)>=13) 
  {
    Blynk.virtualWrite(V16,"Can have only 10 number");
    delay(2000);
    Blynk.virtualWrite(V16,inMoney);
    return;
  }
  if(strlen(inMoney)==3)
  {
    inMoney[countInMoney]='-';
    countInMoney++;
    inMoney[countInMoney]='\0';
  }
  if(strlen(inMoney)==5)
  {
    inMoney[countInMoney]='-';
    countInMoney++;
    inMoney[countInMoney]='\0';
  }
  if(strlen(inMoney)==11)
  {
     inMoney[countInMoney]='-';
    countInMoney++;
    inMoney[countInMoney]='\0';
  } 
  if(strlen(inMoney)<13)
  {
    inMoney[countInMoney]=number;
    countInMoney++;
    inMoney[countInMoney]='\0';
    Blynk.virtualWrite(V16,inMoney);
  }
  delay(200);
}
void printPassword(char password[])
{
  if(strlen(password)==1) Blynk.virtualWrite(V16,"*");
  if(strlen(password)==2) Blynk.virtualWrite(V16,"**");
  if(strlen(password)==3) Blynk.virtualWrite(V16,"***");
  if(strlen(password)==4) Blynk.virtualWrite(V16,"****");
  if(strlen(password)==5) Blynk.virtualWrite(V16,"*****");
  if(strlen(password)==6) Blynk.virtualWrite(V16,"******");
}
void printInMoney(char number)
{
  inMoney[countInMoney]=number;
  countInMoney++;
  inMoney[countInMoney]='\0';
  Blynk.virtualWrite(V16,inMoney);
}
boolean checkAccount()
{
  for(int i=0;i<5;i++)
  {
    stateAccount=strcmp(inMoney,remainMoney[i][0]);
    if(stateAccount==0&&remainMoney[posRemainMoney][0]==remainMoney[i][0])
    {
      Blynk.virtualWrite(V16,"Can't transfer to your account");
      delay(2000);
      Blynk.virtualWrite(V16,inMoney);
      already=1;
      forCheckAccount=i;
      break;
    }
    else if(stateAccount==0)
    {
      posRemainMoneyAccount=i;
      break;
    }  
  }
  
  if(stateAccount==0 &&already==0)  return true;
  else return false;
}
BLYNK_WRITE(V0)
{
  
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='0';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 0, 0, 0);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('0');
    }
    else printInMoney('0');
  }
  
    
}

BLYNK_WRITE(V1)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='1';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 0, 0, 1);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('1');
    }
    else printInMoney('1');
  }
}

BLYNK_WRITE(V2)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='2';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 0, 1, 0);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('2');
    }
    else printInMoney('2');
  }
}

BLYNK_WRITE(V3)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='3';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 0, 1, 1);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
     else if(sMode==1&&statusAccount==0)
    {
      printAccount('3');
    }
    else printInMoney('3');
  }
    
}

BLYNK_WRITE(V4)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='4';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 1, 0, 0);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
     else if(sMode==1&&statusAccount==0)
    {
      printAccount('4');
    }
    else
    {
      printInMoney('4');
    }
  }
    
}

BLYNK_WRITE(V5)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='5';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 1, 0, 1);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('5');
    }
    else
    {
      printInMoney('5');
    }
  }
    
}

BLYNK_WRITE(V6)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='6';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 1, 1, 0);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('6');
    }
    else
    {
      printInMoney('6');
    }
  }
    
}

BLYNK_WRITE(V7)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='7';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(0, 1, 1, 1);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('7');
    }
    else
    {
      printInMoney('7');
    }
  }
    
}

BLYNK_WRITE(V8)
{
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='8';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
    ////sendBin(1, 0, 0, 0);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('8');
    }
    else 
    {
      printInMoney('8');
    }
  }
    
}

BLYNK_WRITE(V9)
{
  
  if (param.asInt() == 1&&state!=0)
  {
    password[count]='9';
    count++;
    password[count]='\0';
    printPassword(password);
    checkPassLength(password);
   ////sendBin(1, 0, 0, 1);
  }
  if(param.asInt() ==1 &&state==0)
  {
    if(sMode==0) Blynk.virtualWrite(V16,"Select Mode first");
    else if(sMode==1&&statusAccount==0)
    {
      printAccount('9');
    }
    else
    {
      printInMoney('9');
    }
  }
    
}

BLYNK_WRITE(V10)
{
//  if (param.asInt() == 1&&count==7&&sMode==0)
//  {
//    Blynk.setProperty(V13, "labels","-", "Deposit", "Withdraw");
//    Blynk.virtualWrite(V16," ");
//    ////sendBin(1, 0, 1, 0);
//  }
  if(param.asInt()==1&&count<=6&&sMode==0)
  {
    Blynk.virtualWrite(V16,"Password must have lease 6 number");
    count=0;
  }
  //--------------Transfer------------//
  if(param.asInt()==1&&sMode==1)
  {
    boolean transferMode=checkAccount();
    already=0;
    if(countInMoney>=13&&statusAccount==0)  
    {
      if(transferMode== true) 
      { 
        Blynk.virtualWrite(V16,"Enter money to transfer");
        inMoney[0]='\0';
        countInMoney=0;
        statusAccount=1;
      }
      else if(transferMode!=true&&remainMoney[posRemainMoney][0]!=remainMoney[forCheckAccount][0])
      {
        Blynk.virtualWrite(V16,"Don't have account in system");
        delay(2000);
        Blynk.virtualWrite(V16,inMoney);
      }
      forCheckAccount=100;
    }
    else if(countInMoney<13&&statusAccount==0) 
    {
      Blynk.virtualWrite(V16,"Account must have 10 number");
      delay(2000);
      Blynk.virtualWrite(V16,inMoney);
    }
    else if(param.asInt()==1&&statusAccount==1)
    {
      int value=atoi(inMoney);
      int tempValue=atoi(remainMoney[posRemainMoney][2]);
      int tempValueTransfer=atoi(remainMoney[posRemainMoneyAccount][2]);
      sprintf(previousRemainMoney[posRemainMoneyAccount][2],"%d", tempValueTransfer);
      sprintf(remainMoney[posRemainMoney][2],"%d", tempValue);
      int graphPrev=atoi(remainMoney[posRemainMoney][2]);
      Blynk.virtualWrite(V18,graphPrev);
      
      //Blynk.virtualWrite(V18, tempValueTransfer);
      if((tempValue-value)>=0) 
      {
        tempValue-=value;
        tempValueTransfer+=value;
        Serial.println('T');
        Serial.println(remainMoney[posRemainMoney][0]);
        Serial.println(inMoney);
        Serial.println(remainMoney[posRemainMoneyAccount][0]);
        Blynk.virtualWrite(V16,"Transfer complete");
        Blynk.virtualWrite(V21,tempValue-graphPrev);
        statusAccount=0;
      }
      else Blynk.virtualWrite(V16,"Not enought money");
      inMoney[0]='\0';
      countInMoney=0;
      sprintf(remainMoney[posRemainMoney][2],"%d", tempValue);
      sprintf(remainMoney[posRemainMoneyAccount][2],"%d", tempValueTransfer);
      Serial.println(remainMoney[posRemainMoney][2]);
      //Blynk.virtualWrite(V18, tempValueTransfer);
      Blynk.virtualWrite(V14,remainMoney[posRemainMoney][2]);
      
         int graph=atoi(remainMoney[posRemainMoney][2]);
         Blynk.virtualWrite(V19,graph);
         Blynk.virtualWrite(V20,graph);
    }
  }
  //-----------------WithDraw---------------//
  if(param.asInt()==1&&sMode==2)
  {
    int value=atoi(inMoney);
    int tempValue=atoi(remainMoney[posRemainMoney][2]);
    sprintf(previousRemainMoney[posRemainMoney][2],"%d", tempValue);
    int graphPrev=atoi(remainMoney[posRemainMoney][2]);
    Blynk.virtualWrite(V18,graphPrev);
    //Blynk.virtualWrite(V18, tempValue);
    
    if((tempValue-value)>=0) 
    {
      tempValue-=value;
      Serial.println('W');
      Serial.println(remainMoney[posRemainMoney][0]);
      Serial.println(inMoney);
      Blynk.virtualWrite(V16,"Success !!");
      Blynk.virtualWrite(V21,tempValue-graphPrev);
    }
    else Blynk.virtualWrite(V16,"Not enought money");
    inMoney[0]='\0';
    countInMoney=0;
    sprintf(remainMoney[posRemainMoney][2],"%d", tempValue);
    Serial.println(remainMoney[posRemainMoney][2]);
    //Blynk.virtualWrite(V18, tempValue);
    Blynk.virtualWrite(V14,remainMoney[posRemainMoney][2]);
      if(logIn==1)
      {
         int graph=atoi(remainMoney[posRemainMoney][2]);
         Blynk.virtualWrite(V19,graph);
         Blynk.virtualWrite(V20,graph);
      }
  }
  //-----------------Deposit--------------------//
  if(param.asInt()==1&&sMode==3)
  {
    int value=atoi(inMoney);
    int tempValue=atoi(remainMoney[posRemainMoney][2]);
    sprintf(previousRemainMoney[posRemainMoney][2],"%d", tempValue);
    int graphPrev=atoi(remainMoney[posRemainMoney][2]);
    Blynk.virtualWrite(V18,graphPrev);
    //Blynk.virtualWrite(V18, tempValue);
    
      tempValue+=value;
      Serial.println('D');
      Serial.println(remainMoney[posRemainMoney][0]);
      Serial.println(inMoney);
      Blynk.virtualWrite(V16,"Success !!");
      Blynk.virtualWrite(V21,tempValue-graphPrev);
    
    inMoney[0]='\0';
    countInMoney=0;
    sprintf(remainMoney[posRemainMoney][2],"%d", tempValue);
    Serial.println(remainMoney[posRemainMoney][2]);
    //Blynk.virtualWrite(V18, tempValue);
    Blynk.virtualWrite(V14,remainMoney[posRemainMoney][2]);
    if(logIn==1)
    {
       int graph=atoi(remainMoney[posRemainMoney][2]);
       Blynk.virtualWrite(V19,graph);
       Blynk.virtualWrite(V20,graph);
    }
  }
}

BLYNK_WRITE(V11)
{
  if (param.asInt() == 1)
   {
    ////sendBin(1, 0, 1, 1);
    
   
    if(count==7)
    {
      inMoney[0]='\0'; 
      countInMoney=0;
      Blynk.virtualWrite(V16," ");
    }
    else 
    { 
      password[0]='\0';
      count=0;
      Blynk.virtualWrite(V16," ");
    }
    
//    password[0]='\0';
//    count=0;
//    state= 999;
//    sMode=0;i
//    Blynk.virtualWrite(V16," ");
//    Blynk.setProperty(V13, "labels", " ");  
   }
   
  
}


BLYNK_WRITE(V13)
{
  if (param.asInt() ==1)
   {
     Blynk.virtualWrite(V16," ");
     sMode=0;
   }
  else if (param.asInt() == 2&&count==7)
   { 
    Blynk.virtualWrite(V16,"Enter Account");
    statusAccount=0;
    sMode=1;
   }    
  else if (param.asInt() == 3&&count==7)
  {
    Blynk.virtualWrite(V16,"Enter money");
    statusAccount=0;
    sMode=2;
  }
  else if(param.asInt() == 4 && count==7)
  {
    Blynk.virtualWrite(V16,"Enter money");
    statusAccount=0;
    sMode=3;
  }
  else
  {
    Blynk.virtualWrite(V16,"Enter password first");
  }
     
}
BLYNK_WRITE(V15)
{
  if(param.asInt()==1) 
  { 
    if(logIn==1)
    {
      int graph=atoi(remainMoney[posRemainMoney][2]);
      Blynk.virtualWrite(V19,graph);
      Blynk.virtualWrite(V20,graph);
      int graphPrev=atoi(previousRemainMoney[posRemainMoney][2]);
      Blynk.virtualWrite(V18,graphPrev);
    }
    Blynk.virtualWrite(V14," ");
    Blynk.virtualWrite(V16,"Enter password");
    password[0]='\0';
    sMode=0;
    count=0;
    state=999;
    inMoney[0]='\0';
    countInMoney=0;
    statusAccount=0;
    posRemainMoneyAccount=0;
    logIn=0;
    Blynk.virtualWrite(V17," ");
    Blynk.virtualWrite(V18,0);
    Blynk.virtualWrite(V19,0);
    Blynk.virtualWrite(V20,0);
    Blynk.virtualWrite(V21,0);
//    for(int i=0;i<4;i++)
//    {
//      Serial.println(i);
//      Serial.print("remainMoney : ");
//      Serial.println(remainMoney[i][2]);
//      Serial.print("previousRemainMoney : ");
//      Serial.println(previousRemainMoney[i][2]);
//      
//    }
  }
}
int dex=0;
char input[100];
char tempInput;
int checkInput=0;
void receiveData()
{
  while(Serial.available()>0)
  {
    if(dex<99)
    {
      tempInput=Serial.read();
      input[dex]=tempInput;
       dex++;
     input[dex]='\0';
    }    
  }
   if(input[strlen(input)-1]=='T')
   {
      
   }
   else if(input[0]=='W')
   {
    
   }
  
   input[0]='\0';
   dex=0;
}

void setup()
{
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  ////sendBin(1, 1, 1, 1);
  
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V16,"Enter password");
  Blynk.virtualWrite(V14," ");
}
int graph=0;
int graphPrev=0;
void loop()
{
  Blynk.run();
//  while(Serial.available()>0)
//  {
//    if(dex<99)
//    {
//      tempInput=Serial.read();
//      input[dex]=tempInput;
//       dex++;
//     input[dex]='\0';
//     Serial.println(input);
//    }    
//  }
//  
//  input[0]='\0';
//  dex=0;
//  if(logIn==1)
//  {
//     graph=atoi(remainMoney[posRemainMoney][2]);
//     Blynk.virtualWrite(V19,graph);
//     Blynk.virtualWrite(V20,graph);
//     graphPrev=atoi(previousRemainMoney[posRemainMoney][2]);
//     Blynk.virtualWrite(V18,graphPrev);
//  }
 
//delay(1000);
}
char number[4];
void sendBin(int a, int b, int c, int d)
{
  char val[5];
  val[0] = a+'0';
  val[1] = b+'0';
  val[2] = c+'0';
  val[3] = d+'0';
  val[4]='\0';
}
  
  
  //for (int i=0; i<4; i++)
   // digitalWrite(sendPins[i], val[i]);
//  for (int i=0; i<4; i++)
//    digitalWrite(sendPins[i], HIGH);

