#include "rm67162.h"
#include <TFT_eSPI.h>

#include <RotaryEncoder.h>

#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#include "NotoSansMonoSCB20.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite sprite2 = TFT_eSprite(&tft);

#define PIN_IN1 45
#define PIN_IN2 46

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

#define bck TFT_BLACK
#define up 21
#define down 0
#define led 38
#define led2 10
bool deb = 0;
bool deb2 = 0;
byte bright = 5;
byte brightness[7] = {100, 120, 140, 180, 200, 230, 254 };
unsigned short c1[4]={0xBD44,0xB88C,0x036F,0xF79D};

float n[4]={0,359,20,20};
float changeN[4]={1,2,1.2,1.2};
int dir[4]={1,-1,1,1};
int sx[4]={122,122,60,60};
int sy[4]={280,172,108,108};
int center[3]={14,8,10};

float speed[4]={1,2,4,6};
int chosenSpeed=1;
int blinks=0;
 
int r[4]={80,40,60,24};
int d[4]={30,60,36,36};

double rad=0.01745;
float x[4][360]; //outer point
float y[4][360];
float px[4][360]; //ineer point
float py[4][360];

int a,b,c,e;
bool Auto=1;
bool ledOn=0;
int angle=0;

void setup()
{  
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  rm67162_init();  // amoled lcd initialization
  lcd_setRotation(0);
  sprite.createSprite(230, 422);
  sprite2.createSprite(220,110);
  sprite2.fillRect(0,0,130,42,c1[0]);
  sprite2.setTextColor(bck,c1[0]);
  sprite2.loadFont(NotoSansBold36);
  sprite2.drawString("BLINK",4,4);
  sprite2.setTextColor(c1[3],bck);
  sprite2.drawString("MONSTER",0,46);
  sprite2.unloadFont();
  sprite2.loadFont(NotoSansBold15);
  sprite2.setTextColor(0x94B2,bck);
  sprite2.drawString("VOLOS",140,4);
  sprite2.drawString("PROJECTS",140,20);
  sprite2.unloadFont();
  
  int z=random(30,150);
  sprite2.fillRect(0,86,z,6,c1[1]);
  sprite2.fillRect(z,86,220-z,6,c1[2]);

  lcd_PushColors(10, 6, 220, 120, (uint16_t*)sprite2.getPointer());

  sprite.setSwapBytes(1);
  lcd_brightness(220);

  for(int j=0;j<4;j++)
  for(int i=0;i<360;i++)
    {
       x[j][i]=(r[j]*cos(rad*i))+sx[j];
       y[j][i]=(r[j]*sin(rad*i))+sy[j];
       px[j][i]=((r[j]-20)*cos(rad*i))+sx[j];
       py[j][i]=((r[j]-20)*sin(rad*i))+sy[j];
    }
}

void draw()
{
 sprite.fillSprite(bck);
 
 
 for(int j=0;j<4;j++)
 {
 if(j<3){  
 for(int i=0; i<360/d[j];i++){
   
   a=n[j]+(i*d[j]);
   if(a>=360)
   a=(n[j]+(i*d[j]))-360;

   b=n[j]+(d[j]/2)+(i*d[j]);
   if(b>=360)
   b=(n[j]+(d[j]/2)+(i*d[j]))-360;

   c=n[j]+d[j]+(i*d[j]);
   if(c>=360)
   c=(n[j]+d[j]+(i*d[j]))-360;


 sprite.fillTriangle(x[j][b],y[j][b], px[j][a],py[j][a], px[j][c],py[j][c], c1[j]);
 sprite.fillCircle(sx[j],sy[j],r[j]-20,c1[j]);
 if(j==0 || j==2)
 sprite.drawSmoothArc(sx[j],sy[j], r[j]/2, r[j]/2-4, 0, 360, bck, c1[j]);

 sprite.fillSmoothCircle(sx[j],sy[j],center[j],bck,c1[j]);
 sprite.drawWedgeLine(x[j][b],y[j][b],px[j][a],py[j][a],2,2,c1[j]);
 sprite.drawWedgeLine(x[j][b],y[j][b],px[j][c],py[j][c],2,2,c1[j]);
 }}else{
    
    if(y[j][(int)n[j]]<sy[j])
     a=sy[j]-y[j][(int)n[j]];
    else
   
    a=y[j][(int)n[j]]-sy[j];

   b=(90*90)-(a*a);
   float er=sqrt(b);

   if((x[j][(int)n[j]]+er+36)>204)
   {ledOn=1;}
   if((x[j][(int)n[j]]+er+36)<204 && ledOn==1)
   {blinks++; ledOn=0;}
   sprite.drawWedgeLine(x[j][(int)n[j]]+er,sy[j],x[j][(int)n[j]]+er+36,sy[j],5,5,c1[0]);
   sprite.drawWedgeLine(x[j][(int)n[j]],y[j][(int)n[j]],x[j][(int)n[j]]+er,sy[j],8,6,c1[j]);
   sprite.fillSmoothCircle(x[j][(int)n[j]],y[j][(int)n[j]],4,bck,c1[j]);
   sprite.fillSmoothCircle(x[j][(int)n[j]]+er,sy[j],4,bck,c1[j]);
  
   }
}

 sprite.drawRect(0,392,220,30,c1[0]);
 sprite.fillRect(4,396,100,22,c1[0]);
 sprite.loadFont(NotoSansMonoSCB20);
 sprite.setTextColor(bck,c1[0]);
 sprite.drawString("BLINKS:",10,398); 
 sprite.setTextColor(c1[3],bck);
 sprite.drawString(String(blinks),160,398); 
 sprite.fillRect(130,0+(Auto*30),6,26,c1[0]);

 sprite.drawRect(140,0,80,26,c1[0]);
 sprite.drawString("MANUAL",146,4); 

 sprite.drawRect(140,30,80,26,c1[0]);
 sprite.drawString("AUTO",146,34); 


 sprite.setTextColor(c1[3],bck);
 sprite.drawString("SPEED:",0,370); 

 for(int i=0;i<4;i++)
 {
 sprite.drawRect(0,246+i*30,26,26,c1[0]);
 if(i==chosenSpeed)
 {
  sprite.fillRect(0,246+i*30,26,26,c1[0]);
  sprite.setTextColor(bck,c1[0]);  
  sprite.drawString(String((int)speed[i]),6,(246+i*30)+4); 
}
 else{
 sprite.drawRect(0,246+i*30,26,26,c1[0]);
 sprite.setTextColor(0x94B2,bck);  
 sprite.drawString(String((int)speed[i]),6,(246+i*30)+4); }
 }

 sprite.fillRect(210,70,10,80,c1[0]);
 sprite.unloadFont(); 
 lcd_PushColors(10, 110, 230, 422, (uint16_t*)sprite.getPointer());
}


void readButtons(){
  if (digitalRead(down) == 0) {
    if (deb == 0) {
      deb = 1;
      chosenSpeed++;
      if(chosenSpeed==4)
      chosenSpeed=0;
      draw();
 
    }
  } else deb = 0;

  if (digitalRead(up) == 0) {
    if (deb2 == 0) {
      deb2 = 1;
      Auto = !Auto;
      draw();
    }
  } else deb2 = 0;
}

void readEncoder() {

  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    
    if(newPos>pos)
    {
   //dir[4]={1,-1,1,1};
   dir[0]=1; dir[2]=1; dir[3]=1; dir[1]=-1;
   for(int i=0;i<4;i++){ 
   n[i]=n[i]+((changeN[i]*speed[chosenSpeed])*dir[i]);
   if(dir[i]==1)
   {if(n[i]>=360)
   n[i]=n[i]-360;}
   else
   {if(n[i]<=0)
   n[i]=359;}
   }draw();
    }

      if(newPos<pos)
    {
   //dir[4]={1,-1,1,1};
   dir[0]=-1; dir[2]=-1; dir[3]=-1; dir[1]=1;
   for(int i=0;i<4;i++){ 
   n[i]=n[i]+((changeN[i]*speed[chosenSpeed])*dir[i]);
   if(dir[i]==1)
   {if(n[i]>=360)
   n[i]=n[i]-360;}
   else
   {if(n[i]<=0)
   n[i]=359;}
   }draw();
    }
    
    
    
    pos = newPos;
  } 

}


void loop()
{
   digitalWrite(led,ledOn);
   digitalWrite(led2,ledOn);
   readButtons();
   
   
   
   if(Auto==1){
   for(int i=0;i<4;i++){
   n[i]=n[i]+((changeN[i]*speed[chosenSpeed])*dir[i]);
   if(dir[i]==1)
   {if(n[i]>=360)
   n[i]=n[i]-360;}
   else
   {if(n[i]<=0)
   n[i]=359;}
   }draw();
   }
   else
   readEncoder();

}