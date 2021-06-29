#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <time.h>
#include <EEPROM.h>
struct Services{
    int id;
    String name;
    int servicesCount;//Yapılabilecek hizmet sayısı
    int price;
    int choosed;

};
struct Money{
    int id;
    int money;
    int moneyCount;
    int choosed;
};
bool troubleExist=false;
int lastOkLevel=0;
Services kopukleme={1,"Kopukleme",0,15,0};
Services yikama={2,"Yikama",0,10,0};
Services kurulama={3,"Kurulama",0,5,0};
Services cilalama={4,"Cilalama",0,50,0};

Money bes={1,5,0,0};
Money on={2,10,0,0};
Money yirmi={3,20,0,0};
Money elli={4,50,0,0};
Money yuz={5,100,0,0};


int okCount=0;
int servicesCount=1;
int moneyCount=1;
int randomState(){
randomSeed(analogRead(A0));
int randomNum=(int)random(4);
return randomNum;
}

void buttonIncrease(){
if (okCount==1){

    if (servicesCount<5)
    {
        servicesCount++;
    }
    
    
}
else if (okCount==2)
{   
    if (moneyCount<6)
    {
        moneyCount++;
    }
}

}
void buttonDecrease(){
if (okCount==1){
    if (servicesCount>0)
    {
        servicesCount--;
    }
    
    
}
else if (okCount==2)
{
    if (moneyCount>0)
    {
        moneyCount--;
    }
}
}
void buttonOk(){
if(okCount==0){
   okCount++;
}
else if(okCount==1)
{   
    if (servicesCount==0)
    {
    kopukleme.choosed=0;
    yikama.choosed=0;
    kurulama.choosed=0;
    cilalama.choosed=0;
    }
    else if(servicesCount==1){
        kopukleme.choosed++;
    }
    else if (servicesCount==2)
    {
        yikama.choosed++;
    }
    else if (servicesCount==3)
    {
        kurulama.choosed++;
    }
    else if (servicesCount==4)
    {
        cilalama.choosed++;
    }
    else{
        okCount++;
    }

}
else if (okCount==2){
    if (moneyCount==0)
    {
        bes.choosed=0;
        on.choosed=0;
        yirmi.choosed=0;
        elli.choosed=0;
        yuz.choosed=0;
    }
    else if(moneyCount==1){
        bes.choosed++;
    }
    else if (moneyCount==2)
    {
        on.choosed++;
    }
    else if (moneyCount==3)
    {
        yirmi.choosed++;
    }
    else if (moneyCount==4)
    {
        elli.choosed++;
    }
    else if (moneyCount==5)
    {
        yuz.choosed++;
    }
    
    
    else{
        lastOkLevel=okCount;
        int slot=(bes.choosed*bes.money)+(on.choosed*on.money)+(yirmi.choosed*yirmi.money)+(elli.choosed*elli.money)+(yuz.choosed*yuz.money);
        int sumofprice=(kopukleme.choosed*kopukleme.price)+(yikama.choosed*yikama.price)+(kurulama.choosed*kurulama.price)+(cilalama.choosed*cilalama.price);
        int paraUstu=slot-sumofprice;
        int randomNum=randomState();
        
        Serial.println("Para yukleniyor");
        delay(1000);
        Serial.print("Para ustu:");
        Serial.println(paraUstu);
        delay(1000);
        if (randomNum==2||sumofprice>slot)
        {
        
        if (sumofprice>slot)
        {   
            Serial.println("Eksik para.");
            delay(500);
            
            Serial.print("Islemlerinizi tekrar yapiniz!");
            
            delay(500);
        }
        else{
            
            Serial.println("Para sikisti");
            troubleExist=true;
              
        }
            bes.choosed=0;
            on.choosed=0;
            yirmi.choosed=0;
            elli.choosed=0;
            yuz.choosed=0;

            kopukleme.choosed=0;
            yikama.choosed=0;
            kurulama.choosed=0;
            cilalama.choosed=0;
            
            servicesCount=1;
            moneyCount=1;
            okCount=0;
            
            return;
            
        }
        else{
            
        
            troubleExist=false;
        
            bes.moneyCount+=bes.choosed;
            bes.choosed=0;
            on.moneyCount+=on.choosed;
            on.choosed=0;
            yirmi.moneyCount=yirmi.choosed;
            yirmi.choosed=0;
            elli.moneyCount+=elli.choosed;
            elli.choosed=0;
            yuz.moneyCount+=yuz.choosed;
            yuz.choosed=0;

            kopukleme.servicesCount-=kopukleme.choosed;
            kopukleme.choosed=0;
            yikama.servicesCount-=yikama.choosed;
            yikama.choosed=0;
            kurulama.servicesCount-=kurulama.choosed;
            kurulama.choosed=0;
            cilalama.servicesCount-=cilalama.choosed;
            cilalama.choosed=0;
            
            
            while (paraUstu>0)
            {
                if ((paraUstu>=100)&&(yuz.moneyCount!=0))
                {
                    yuz.moneyCount--;
                    paraUstu-=100;
                }
                else if ((paraUstu>=50)&&(elli.moneyCount!=0))
                {
                    elli.moneyCount--;
                    paraUstu-=50;
                }
                else if((paraUstu>=20)&&(yirmi.moneyCount!=0))
                {
                    yirmi.moneyCount--;
                    paraUstu-=20;
                }
                else if((paraUstu>=10)&&(on.moneyCount!=0))
                {
                    on.moneyCount--;
                    paraUstu-=10;
                }
                else if((paraUstu>=5)&&(bes.moneyCount!=0))
                {
                    bes.moneyCount--;
                    paraUstu-=5;
                }
                else
                {
                    Serial.print("Para ustu verilemedi");
                    
                    break;
                }
                
            }
            EEPROM.write(1,bes.moneyCount);
            EEPROM.write(2,on.moneyCount);
            EEPROM.write(3,yirmi.moneyCount);
            EEPROM.write(4,elli.moneyCount);
            EEPROM.write(5,yuz.moneyCount);

            EEPROM.write(6,kopukleme.servicesCount);
            EEPROM.write(7,yikama.servicesCount);
            EEPROM.write(8,kurulama.servicesCount);
            EEPROM.write(9,cilalama.servicesCount);

            servicesCount=1;
            moneyCount=1;
            okCount=0;
        
        }
        
        
    
    }
}

}


void showServices(){
    Serial.print(kopukleme.id);
    Serial.print(" ");
    Serial.print(kopukleme.name);
    Serial.print(" ");
    Serial.print(kopukleme.price);
    Serial.println(" TL");
    delay(1000);
    

    Serial.print(yikama.id);
    Serial.print(" ");
    Serial.print(yikama.name);
    Serial.print(" ");
    Serial.print(yikama.price);
    Serial.println(" TL");
    delay(1000);
    

    Serial.print(kurulama.id);
    Serial.print(" ");
    Serial.print(kurulama.name);
    Serial.print(" ");
    Serial.print(kurulama.price);
    Serial.println(" TL");
    delay(1000);
    

    Serial.print(cilalama.id);
    Serial.print(" ");
    Serial.print(cilalama.name);
    Serial.print(" ");
    Serial.print(cilalama.price);
    Serial.println(" TL");
    delay(1000);
    
}

void setup() {
    
    Serial.begin(9600);
    bes.moneyCount=EEPROM.read(1);
    on.moneyCount=EEPROM.read(2);
    yirmi.moneyCount=EEPROM.read(3);
    elli.moneyCount=EEPROM.read(4);
    yuz.moneyCount=EEPROM.read(5);
    kopukleme.servicesCount=EEPROM.read(6);
    yikama.servicesCount=EEPROM.read(7);
    kurulama.servicesCount=EEPROM.read(8);
    cilalama.servicesCount=EEPROM.read(9);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
attachInterrupt(digitalPinToInterrupt(2),buttonIncrease,RISING);
attachInterrupt(digitalPinToInterrupt(3),buttonDecrease,RISING);
attachInterrupt(digitalPinToInterrupt(18),buttonOk,RISING);

}

void loop() {
     if(lastOkLevel==2){
        if (troubleExist)
        {
            for (int i = 0; i < 5; i++)
            {   
                digitalWrite(4,HIGH);
                delay(500);
                digitalWrite(4,LOW);
                delay(500);
            }
            lastOkLevel=okCount;
            
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {   
                digitalWrite(5,HIGH);
                delay(500);
                digitalWrite(5,LOW);
                delay(500);
                lastOkLevel=okCount;
            }
        }
        
        
    }
    if(okCount==0){
       showServices();
    }
    else if (okCount==1)
    {
       switch (servicesCount)
       {

       case 0:
           Serial.println("Secimleri sifirla");
           break;

       case 1:
           Serial.print(kopukleme.name);
           Serial.print(" ");
           Serial.print(kopukleme.price);
           Serial.print(" secilen say:");
           Serial.println(kopukleme.choosed);
           break;
        case 2:
           Serial.print(yikama.name);
           Serial.print(" ");
           Serial.print(yikama.price);
           Serial.print(" secilen say:");
           Serial.println(yikama.choosed);
           break;
        case 3:
           Serial.print(kurulama.name);
           Serial.print(" ");
           Serial.print(kurulama.price);
           Serial.print(" secilen say:");
           Serial.println(kurulama.choosed);
           break;
        case 4:
           Serial.print(cilalama.name);
           Serial.print(" ");
           Serial.print(cilalama.price);
           Serial.print(" secilen say:");
           Serial.println(cilalama.choosed);
           break;
        case 5:
           Serial.println("Onayla");
           break;
       default:
           break;
       }
    }
    else if (okCount==2)
    {
        switch (moneyCount)
        {
        case 0:
            Serial.println("Secimleri sifirla");
        case 1:
            Serial.print(5);
            Serial.print(" Secilme mik:");
            Serial.println(bes.choosed);
            break;
        case 2:
            Serial.print(10);
            Serial.print(" Secilme mik:");
            Serial.println(on.choosed);
            break;    
        case 3:
            Serial.print(20);
            Serial.print(" Secilme mik:");
            Serial.println(yirmi.choosed);
            break;
        case 4:
            Serial.print(50);
            Serial.print(" Secilme mik:");
            Serial.println(elli.choosed);
            break;
        case 5:
            Serial.print(100);
            Serial.print(" Secilme mik:");
            Serial.println(yuz.choosed);
            break;
        case 6:
            Serial.println("Bitir");
            break;
        default:
            break;
        }
    }
   
    
    delay(1000);
    
    
   
    
}