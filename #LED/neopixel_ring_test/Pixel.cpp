
#include <Arduino.h>

#include "Pixel.h"

uint8_t id;
float r, g, b, w;
float Tr, Tg, Tb, Tw;
float Sr, Sg, Sb, Sw;
uint8_t Fr, Fg, Fb, Fw;

char print_buff[40];


Pixel::Pixel(uint8_t ID){
  id = ID;
  //off();
}


void Pixel::off(){
  r = 0;
  g = 0;
  b = 0;
  w = 0;
}

void Pixel::setFade( uint8_t x){
  Fr = x;
  Fg = x;
  Fb = x;
  Fw = x;
}

void Pixel::set( float R, float G, float B, float W){
  r = R;
  g = G;
  b = B;
  w = W;
  
  Tr = r;
  Tg = g;
  Tb = b;
  Tw = w;

  setFade(0);
}

void Pixel::fadeTo( float R, float G, float B, float W, float interval){
  Tr = R;
  Tg = G;
  Tb = B;
  Tw = W;

  Sr = ( Tr -r)/interval;
  Sg = ( Tg -g)/interval;
  Sb = ( Tb -b)/interval;
  Sw = ( Tw -w)/interval;
  
  if( r != Tr )
    Fr = 1;
  else 
    Fr = 0;

  if( g != Tg )
    Fg = 1;
  else 
    Fg = 0;

  if( b != Tb )
    Fb = 1;
  else 
    Fb = 0;

  if( w != Tw )
    Fw = 1;
  else 
    Fw = 0;

//  Serial.print(" current is: ");  Serial.print(r);
//  Serial.print(" target is: ");  Serial.print(Tr);
//  Serial.print(" Step is: ");  Serial.print(Sr);
//  Serial.print(" fading is: ");  Serial.print(Fr);
  
}


uint8_t Pixel::ID(){
  return id;
}

uint8_t Pixel::R(){
  return int(r);
}

uint8_t Pixel::G(){
  return int(g);
}

uint8_t Pixel::B(){
  return int(b);
}

uint8_t Pixel::W(){
  return int(w);
}
uint8_t Pixel::fade(){
  // fade each

  //printout();
  
  if( Fr ){
    // catch end of fade
    if( abs(Tr-r) < abs(Sr) ) {
      r = Tr;
      Fr = 0;
    }
    else  // step fade
      r += Sr;
  }

  if( Fg ){
    // catch end of fade
    if( abs(Tg -g) < abs(Sg) ) {
      g = Tg;
      Fg = 0;
    }
    else  // step fade
      g += Sg;
  }

  if( Fb ){
    // catch end of fade
    if( abs(Tb-b) < abs(Sb) ) {
      b = Tb;
      Fb = 0;
    }
    else  // step fade
      b += Sb;
  }

  if( Fw ){
    // catch end of fade
    if( abs(Tw-w) < abs(Sw) ) {
      w = Tw;
      Fw = 0;
    }
    else  // step fade
      w += Sw;
  }

  return ( Fr || Fg || Fb || Fw );
}


void Pixel::add( float R, float G, float B, float W){
  r += R;
  g += G;
  b += B;
  w += W;
}


uint8_t Pixel::is_fading(){
  return ( Fr || Fg || Fb || Fw );
}


void Pixel::printout(){
  
  Serial.print("\t LED vals: ");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.print(b);
  Serial.print(", ");
  Serial.print(w);
  Serial.print("\t");  
  
  Serial.print(" target : ");
  Serial.print(Tr);
  Serial.print(", ");
  Serial.print(Tg);
  Serial.print(", ");
  Serial.print(Tb);
  Serial.print(", ");
  Serial.print(Tw);
  Serial.print("\t");  
  
  Serial.print(" in steps : ");
  Serial.print(Sr);
  Serial.print(", ");
  Serial.print(Sg);
  Serial.print(", ");
  Serial.print(Sb);
  Serial.print(", ");
  Serial.print(Sw);
  Serial.print("\t");  
  
  sprintf(print_buff, " status: %d, %d, %d; %d \n" ,Fr, Fg, Fb, Fw );
  Serial.print(print_buff);
}


