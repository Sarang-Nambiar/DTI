#include <FastLED.h>
#define sensorPin 13
#define LED_PIN     25
#define NUM_LEDS    30
#define BRIGHTNESS  80
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 60


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
    Serial.begin(115200);
    delay( 3000 );  power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}




void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 12;
    static uint8_t lastSecond = 99;
    
    int x = analogRead(sensorPin);
    if (x > 2500){
      if( lastSecond != secondHand) {
          lastSecond = secondHand;
          if( secondHand > 0 && secondHand <= 2 )  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
          if( secondHand > 2 && secondHand <= 4)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
          if( secondHand > 4 && secondHand <= 6)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
          if( secondHand > 6 && secondHand <= 8)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
          if( secondHand > 8 && secondHand <= 10)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
          if( secondHand > 10 && secondHand <= 12)  { SetupTotallyRandomPalette();           currentBlending = NOBLEND; }
      } 
    }
}


void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}


void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}


void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   black,  black,  green,  green,  
                                   black,  black,  purple, purple,
                                   black,  black,  green,  green,
                                   black,  black,  purple, purple );
}
