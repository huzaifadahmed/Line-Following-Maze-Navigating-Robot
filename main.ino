#include <mechbotShield.h>
#include <avr/io.h>
#include "MEC733_I_O.h"

void distancetravel()
{
 uint16_t line_1, line_2, line_3, line_4, distance, left_en_total=0, right_en_total=0, left_en_pre, right_en_pre, left_en_cur, right_en_cur, i=0;

  while(1)
  {
  line_1=analog(0);
   line_2=analog(1);
  line_3=analog(2);
    line_4=analog(3);
    motor(500,500);
    if (line_1>=800 && line_2>=800 && line_3>=800 && line_4>=800)
    {  
    while (1) 
    {
       line_1=analog(0);
       line_2=analog(1);
       line_3=analog(2);
       line_4=analog(3);
      left_en_pre=PINC&(1<<PINC2);
      right_en_pre=PINC&(1<<PINC3);

      if (line_2<=650 && line_3<=650)
      {
        i++;
      }
     
      if (left_en_cur!=left_en_pre)
       {
        left_en_total++;
        left_en_cur=left_en_pre;
      }
      if (right_en_cur!=right_en_pre)
       {
        right_en_total++;
        right_en_cur=right_en_pre;
      }

     if (i>=1 && line_1>=800 && line_2>=800 && line_3>=800 && line_4>=800)
     {
      distance=(left_en_total+right_en_total)*10.2/2;
      motor(0,0);
      clrLCD();
      lcdPrint("Distance =");
      lcdPrintDec(distance);
      return(0);
       }
    }
  }
}
}

void linefollow()
{
   uint16_t line_1, line_2, line_3, line_4;

  while(1)
  {
  line_1=analog(0);
   line_2=analog(1);
  line_3=analog(2);
    line_4=analog(3);
    
   if(line_1>800 && line_2>800 && line_3>800 && line_4>800)
     {
      lcdPrint("Done with Line");
      motor(0,0);
      return(0);
     }
   else if (line_2>700 && line_3>700)
   { 
    motor(500,500);
    delay_ms(20);
    clrLCD();
    lcdPrint("go straight");
    }
   
    else if (line_1<650)
    {
      motor(500,-500);
      delay_ms(5);
      clrLCD();
          lcdPrint("turn right");
      }
    else if(line_4<650)
    {
     motor(-500,500);
     delay_ms(5);
     clrLCD();
         lcdPrint("turn left");
     }
    else  { 
    motor(500,500);
    delay_ms(20);
    clrLCD();
        lcdPrint("go straight");
    }
}
}

void maze()
{
  
  DDRD &= ~(1 << PD3) | (1 << PD4) | (1 << PD5);
  DDRC &= ~(1<<PC1);
  //Setting High
  PORTD |= (1 << PD3) | (1 << PD4) | (1 << PD5);
  PORTC |= (1 << PC1);
 uint16_t inputAnalog = 0;
 
  //Port Outputs
  DDRB |= 0b00111111;
  DDRD |= (1 << PD6) | (1 << PD7);
  PORTB |= 0b11111111;
  PORTD &= ~((1 << PD6) | (1 << PD7));
 // end of line stuff 
 
 //motor direction output 
  DDRD=DDRD|((1<<DDRD7));
  DDRB=DDRB|((1<<DDRB0));
  // set PWM 
  DDRB=DDRB|((1<<DDRB1));
  DDRB=DDRB|((1<<DDRB2));
 
 //set the port direction for encoders, 0 for input
  DDRC=DDRC&(~((1<<DDRC2)|(1<<DDRC3)));

  PORTC=PORTC|((1<<PORTC2)|(1<<PORTC3));
  
  uint16_t Dis_left, Dis_right, Dis_center;
  int base_speed = 500, turning_speed = base_speed, Rturning_speed=-1*base_speed;
  int left_en_total=0, right_en_total=0;
  int left_en_cur, left_en_pre, right_en_cur, right_en_pre, error, left_speed, right_speed, P=25, i=20;
  int turns=12.2, j, bumper_right, bumper_left, line_1, line_2, line_3, line_4, y=0;
  left_en_pre=PINC&(1<<PINC2);
  right_en_pre=PINC&(1<<PINC3);

while(1)
  {
   line_1=analog(0);
   line_2=analog(1);
   line_3=analog(2);
   line_4=analog(3);
  
  clrLCD();
  Dis_left=analog(4);
  Dis_center=analog(5);
  Dis_right=analog(6);
  lcdPrintDec(Dis_left);
  lcdPrintDec(Dis_center);
  lcdPrintDec(Dis_right);
  left_en_cur=PINC&(1<<PINC2);
  right_en_cur=PINC&(1<<PINC3);

  bumper_right=PIND&(1<<PIND3); 
//  bumper_2=PIND&(1<<PIND4);
//  bumper_3=PIND&(1<<PIND5);
  bumper_left=PINC&(1<<PINC1);

if (Dis_center >= 200 && Dis_left >= 200 && Dis_right >= 200) 
{

  moveLCDCursor(16);
  lcdPrint("U-Turn");
  for (j=0;j<turns;j++){
    motor (turning_speed,Rturning_speed);// more power to left wheel    
    delay_ms(200);
    if (bumper_left==0) {
      motor (Rturning_speed,Rturning_speed);
      delay_ms(300);
    }
    left_en_total=0;
    right_en_total=0;
    y++;
  }
//  delay_ms(2200);
  clrLCD();
}// first if

else if (Dis_center >180)// obstacle in front 
{
  if (Dis_left > (Dis_right +50))// turn right 
  {
  moveLCDCursor(16);
  lcdPrint("Turning Right");
  for (j=0;j<turns;j++){
    motor (turning_speed,Rturning_speed);// more power to left wheel  
        delay_ms(100);  
    if (bumper_left==0) {   
      motor (Rturning_speed,Rturning_speed);
      delay_ms(300);
      motor (turning_speed*1.5,Rturning_speed*1.5);// more power to left wheel  
      delay_ms(300);
    } 
    left_en_total=0;
    right_en_total=0;
    y++;
  }
//  delay_ms(1100);
  clrLCD();
  }

  else if (Dis_left < Dis_right +50)// Turn left
  {
  moveLCDCursor(16);
  lcdPrint("Turning Left");
  for (j=0;j<turns;j++){
    motor (Rturning_speed,turning_speed); // more power to right wheel
        delay_ms(100); 
    if (bumper_right==0) {
      motor (Rturning_speed,Rturning_speed);
      delay_ms(300);
      motor (Rturning_speed*1.5,turning_speed*1.5);// more power to left wheel  
      delay_ms(300);
    } 
    left_en_total=0;
    right_en_total=0;
    y++;
  }
//  delay_ms(1100);
  clrLCD();   
  }  
}// else if

else if (Dis_center <= 50 && Dis_left <= 50 && Dis_right <= 50 && y>=1)
{
  motor (0,0);
  clrLCD();
  lcdPrint("Mission Accomplished");
  return(0);
}


else 
  {

if (bumper_right==0) {
      motor (Rturning_speed,Rturning_speed);
      delay_ms(300);
      motor (turning_speed*1.5,Rturning_speed*1.5);
      delay_ms(300);
    } 
else if (bumper_left==0) {
      motor (Rturning_speed,Rturning_speed);
      delay_ms(300);
      motor (Rturning_speed*1.5,turning_speed*1.5);
      delay_ms(300);
    }   
    error=left_en_total-right_en_total;
    left_speed=base_speed-error*P;
    right_speed=base_speed-error*P*13;
    motor(base_speed,base_speed);
  }
} // while end
}
  
int main(void)
{
  initMotor();
  initSoftSerial();
  setLCDBackLight(255);
  clrLCD();
  moveLCDCursor(0);
  initADC();
  
 // below is for line stuff 
  //Port Inputs
  DDRD &= ~(1 << PD3) | (1 << PD4) | (1 << PD5);
  DDRC &= ~(1<<PC1);
  //Setting High
  PORTD |= (1 << PD3) | (1 << PD4) | (1 << PD5);
  PORTC |= (1 << PC1);
 uint16_t inputAnalog = 0;
 
  //Port Outputs
  DDRB |= 0b00111111;
  DDRD |= (1 << PD6) | (1 << PD7);
  PORTB |= 0b11111111;
  PORTD &= ~((1 << PD6) | (1 << PD7));
 // end of line stuff 
 
 //motor direction output 
  DDRD=DDRD|((1<<DDRD7));
  DDRB=DDRB|((1<<DDRB0));
  // set PWM 
  DDRB=DDRB|((1<<DDRB1));
  DDRB=DDRB|((1<<DDRB2));
 
 //set the port direction for encoders, 0 for input
  DDRC=DDRC&(~((1<<DDRC2)|(1<<DDRC3)));
  PORTC=PORTC|((1<<PORTC2)|(1<<PORTC3));
  
  // variables
  uint16_t Dis_left, Dis_right, Dis_center;
  int turning_speed = 500,Rturning_speed=-500;
  int left_en_total=0, right_en_total=0;
  int left_en_cur, left_en_pre, right_en_cur, right_en_pre, error, left_speed, right_speed, base_speed=500, P=25, i=20;
  int turns=12.2, j, bumper_right, bumper_left;

  while(1)
  {
     clrLCD();
     left_en_pre=PINC&(1<<PINC2);
     right_en_pre=PINC&(1<<PINC3);
     bumper_left=PINC&(1<<PINC1); 
    if (bumper_left==0)
    {
    distancetravel();
    delay_ms(5000);
    linefollow();
    delay_ms(3000);
    maze();
    return(0);
    }
  }
}
