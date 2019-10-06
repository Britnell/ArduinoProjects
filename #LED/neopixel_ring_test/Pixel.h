#ifndef PIXEL_H
#define PIXEL_H

#include <Arduino.h>

class Pixel
{

  public:
    // Funcs
    Pixel(uint8_t ID);
    void off();
    void printout();
    void set( float R, float G, float B, float W);
    uint8_t R();
    uint8_t G();
    uint8_t B();
    uint8_t W();
    uint8_t ID();
    
    void add( float R, float G, float B, float W);
    void fadeTo( float R, float G, float B, float W, float interval);
    
    uint8_t is_fading();
    void setFade( uint8_t x);
    uint8_t fade();
    
  private:
    // Variables
    uint8_t id;
    float r, g, b, w;
    float Tr, Tg, Tb, Tw;
    float Sr, Sg, Sb, Sw;
    uint8_t Fr, Fg, Fb, Fw;

    
};

#endif
