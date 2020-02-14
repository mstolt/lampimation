
#include <FastLED.h>

FASTLED_USING_NAMESPACE

// ***************************************************************************************
// Hier definierst du die LEDs und die Anschlüsse
// ***************************************************************************************
 
#define NUM_LEDS     48           // Anzahl der LEDs auf dem Strang
#define LED_PIN      7            // Der Pin für die Steuerleitung
#define LED_TYPE     WS2812       // Der LED Typ
#define BRIGHTNESS   2            // Wert von 0 bis 255 um die Helligkeit einzustellen

// ***************************************************************************************

CRGBArray<NUM_LEDS> leds;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Variables to control the sequence of animations
int gPatternNumber; // Index number of which pattern is current
typedef void (*SimplePatternList[])();
unsigned long lastPatternChange; // millis of the last change

void setup() {
  delay(3000); // 3 second delay for recovery

  FastLED.addLeds<LED_TYPE,LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);  
  gPatternNumber = -1; // Start Pattern is the last pattern, just be be immediatly set to the first pattern, Index number of which pattern is current
  lastPatternChange = 0; // initialize with a long time to trigger an immediate naxtAnimation
  nextAnimation();
}

// ##################################################################################
//
//  In diesen Arrays werden die Reihenfolgen der Anumationen definiert. Animationen können auch mehrfach aufgerufen werden.
//  Es sind jeweils eine Funktion für das Setup der Variablen der Animation und eine Funktion für die Animations Loop anzugeben.
//  Im dritten Array steht wie lange die einzelnen Animationen gezeigt werden sollen (Angabe erfolgt in Millisekunden).
//
//  Die eigentlichen Funktionen (animation1Setup, animation1Loop und so weiter) sind am Ende der Datei definiert.
// ##################################################################################

SimplePatternList gPatternAnimationsSetup = { animation1Setup, animation3Setup, animation2Setup, animation3Setup };
SimplePatternList gPatternAnimationsLoop  = { animation1Loop,  animation3Loop,  animation2Loop,  animation3Loop };
unsigned long gPatternTiming[] =            { 3*1000,          2*1000,          3*1000,          3*1000 };

// ##################################################################################

void loop() {  
  unsigned long patternRuntime = millis() - lastPatternChange; // This is good for approx. 50 days. After 50 Days millis() will overrun 
  if( (patternRuntime >= gPatternTiming[gPatternNumber]) ){
    nextAnimation();
  }
  gPatternAnimationsLoop[gPatternNumber]();
  FastLED.show();
}

void nextAnimation(){
   gPatternNumber = (gPatternNumber + 1) % ARRAY_SIZE(gPatternAnimationsLoop);   
   gPatternAnimationsSetup[gPatternNumber]();      
   lastPatternChange = millis();
}

// ##########################################################################
// Ab hier folgen die Animationen und deren Hilfsfunktionen
// ##########################################################################

// ----------------------------
// Hilfsfunktionen
// ----------------------------

void fadeall() { 
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i].nscale8(250); 
  } 
}

void allOff() { 
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

// ----------------------------
// Animation 1
// ----------------------------

// Variables used in the Animations
uint8_t rhue;
uint8_t rdiff;

void animation1Setup(){
  rhue=0;
}

void animation1Loop(){
  for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out
    leds.fadeToBlackBy(40);

    // let's set an led value
    leds[i] = CHSV(rhue++,255,255);

    // now, let's first 20 leds to the top 20 leds, 
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(33);
  }  
}

// ----------------------------
// Animation 2
// ----------------------------

void animation2Setup(){
  rhue=0;
  rdiff=1;
}

void animation2Loop(){
  rhue++;
  if(rhue == 255){
    rhue=0;
  }
  rdiff = 255 / NUM_LEDS;
  fill_rainbow( leds, NUM_LEDS, rhue, rdiff);    
  FastLED.delay(33);
}

// ----------------------------
// Animation 3
// ----------------------------

uint8_t currentLed;

void animation3Setup(){
  currentLed=0;
}

void animation3Loop(){
  allOff();
  currentLed++;
  if(currentLed == NUM_LEDS){
    currentLed=0;
  }  
  leds[currentLed] = CRGB::Blue;
  FastLED.delay(33);
}
