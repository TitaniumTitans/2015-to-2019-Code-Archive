/* TODO:
	·	Make all setLeds() settings able to be applied at once (battle the Binary Problem (perhaps do something with falling-through with switch()?) ).
	·	Make stripes look right: thick bars with big gaps.
	·	Clean up code/get rid of some commented-out code.
	·	I don't like how setLeds() sets vars that the following code will sometimes just overwrite 🤨.
*/


#include <FastLED.h>


#define LED_PIN		 11
#define NUM_LEDS		8*24
#define BRIGHTNESS	64
#define LED_TYPE		WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
#define FADE_SPEED 0.001 // setting this to 0.001 would make the RSL2 fade out in one second.
#define NORMAL_FLASH_SPEED 250
#define SLOW_FLASH_SPEED 750

CRGBArray<NUM_LEDS> leds;

// handler controls:
uint8_t RControl = 0;
uint8_t GControl = 0;
uint8_t BControl = 0;
int flashRateControl = 0; //	0 means no flashing, btw.
int flashCountControl = 5;
bool fadeOutControl = false;
bool stripeControl = false;

unsigned long flashingTimer;
bool flashingLedsOn = false;
int flashCounter = 0;
float fadeBrightness = 0.0;

int frameRate;
unsigned long frameStartTime;

void setup() {
		millidel( 2000 ); //	power-up safety millidel
		FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
		FastLED.setBrightness(	BRIGHTNESS );
		Serial.begin(9600);
		
		setLeds( 255 , 255 , 255 , 0 );
		fadeOutControl = true;
}

void loop () {
	frameStartTime = millis();

	if ( Serial.available() != 0 ) {
		handleRequest( Serial.read() );
	}
	
	/*if ( flashRateControl != 0 ) { // flash:
		if ( millis() > flashingTimer ) {
			flashingTimer = millis() + flashRateControl;

			if ( flashingLedsOn ) {
				fillLeds( 0 , 0 , 0 );
			} else {
				if ( flashCounter++ == flashCountControl ) {
					setLeds( 0 , 0 , 0 , 0 );
				} else {
					//fillLeds( RControl , GControl , BControl );
				}
			}
			flashingLedsOn = !flashingLedsOn;
		}
	} else if ( fadeOutControl ) {
		//fillLeds( (int)( RControl * fadeBrightness ) , (int)(GControl * fadeBrightness) , (int)(BControl * fadeBrightness) );
		fadeBrightness -= FADE_SPEED * frameRate;
		if ( fadeBrightness <= 0.0 ) {
			setLeds( 0 , 0 , 0 , 0 );
		}
	} else { // solid:
		//fillLeds( RControl , GControl , BControl );
	}*/
	applySetLeds();
	FastLED.delay(1000 / UPDATES_PER_SECOND);
	
	frameRate = millis() - frameStartTime;
}

void handleRequest ( int x ) {
	switch ( x ) { // the handlers:
		case (int)'-': // All off
			setLeds( 0 , 0 , 0 , 0 );
			break;
		
		// what is this case for, if the LEDs just fade white right when the Arduino powers on?:
		case (int)'0': // Power On
			setLeds( 255 , 255 , 255 , 0 );
			fadeOutControl = true;
			break;
		case (int)'1': // Disabled
			setLeds( 255 , 255 , 0 , 0 );
			stripeControl = true;
			break;
		case (int)'2': // Auto Enabled
			setLeds( 0 , 255 , 0 , 0 );
			break;
		case (int)'3': // Autonomous Disabled
			setLeds( 0 , 255 , 0 , 0 );
			fadeOutControl = true;
			break;
		case (int)'4': // Telop Enabled
			setLeds( 0 , 0 , 255 , 0 );
			break;
		case (int)'5': // Telop Disable
			setLeds( 0 , 0 , 255 , 0 );
			fadeOutControl = true;
			break;
		case (int)'6': // Test enable
			setLeds( 255 , 0 , 0 , 0 );
			break;
		case (int)'7': // Test disable
			setLeds( 255 , 0 , 0 , 0 );
			fadeOutControl = true;
			break;
		case (int)'8': // Emergency stop
			/*fillLeds( 255 , 0 , 0 );
			delay(1000);
			fadeAll();
			setLeds( 0 , 0 , 0 , 0 );*/
			setLeds( 255 , 0 , 0 , 0 );
			fadeOutControl = true;
			stripeControl = true;
			break;
		case (int)'9': // Battery Critical
			/*FastLED.setBrightness(10);
			cautionStripes(CRGB::Brown);*/
			setLeds( 255 , 100 , 100 , 0 );
			stripeControl = true;
			break;
		case (int)'a': // Signal Hatch
			setLeds( 255 , 255 , 0 , NORMAL_FLASH_SPEED );
			break;
		case (int)'b': // Signal Cargo
			setLeds( 255 , 0 , 0 , NORMAL_FLASH_SPEED );
			break;
		case (int)'c': // Compressor Full
			/*fillLeds(255, 0, 255);
			millidel(100);
			fadeAll();
			setLeds( 0 , 0 , 0 , 0 );*/
			setLeds( 255 , 0 , 255 , 0 );
			fadeOutControl = true;
			break;
		case (int)'d': // Connection Fault
			setLeds( 255 , 0 , 0 , SLOW_FLASH_SPEED );
			break;
		case (int)'e': // Connection Made
			setLeds( 0 , 255 , 0 , SLOW_FLASH_SPEED );
			break;
		default:
			setLeds( 0 , 0 , 0 , 0 );
			Serial.print( (char)x );
			Serial.print( " (0x" );
			Serial.print( x , HEX );
			Serial.println( ") is not a code‼");
			break;
	}
}

/// sets LED controls (settings) and flags to be applied via applySetLeds().
void setLeds ( uint8_t r , uint8_t g , uint8_t b , int _flashRate ) { // NOTE: not all settings can be applied at the same time!
	// set controls:
	RControl = r;
	GControl = g;
	BControl = b;
	flashRateControl = _flashRate;

	// set lesser-used controls to their default values:
	flashCountControl = 5;
	fadeOutControl = false;
	stripeControl = false;

	// reset other variables:
	flashCounter = 0;
	flashingTimer = 0;
	flashingLedsOn = false;
	fadeBrightness = 1.0;
}
void applySetLeds () {
	// How am I supposed to structure this code 🤷‍♂️? All three main settings (fade, flash, and stripe) are supposed to be appliable in any combination.
	if ( flashRateControl != 0 ) {
		if ( millis() > flashingTimer ) {
			flashingTimer = millis() + flashRateControl;

			if ( flashingLedsOn ) {
				fillLeds( 0 , 0 , 0 );
			} else {
				if ( flashCounter++ == flashCountControl ) {
					setLeds( 0 , 0 , 0 , 0 );
				} else {
					fillLeds( RControl , GControl , BControl );
				}
			}
			flashingLedsOn = !flashingLedsOn;
		}
	} else if ( fadeOutControl ) {
		fadeBrightness -= FADE_SPEED * frameRate;
		if ( fadeBrightness <= 0.0 ) {
			setLeds( 0 , 0 , 0 , 0 );
		}
		
		if ( stripeControl ) {
			cautionStripes( CRGB( (int)(RControl * fadeBrightness) , (int)(GControl * fadeBrightness) , (int)(BControl * fadeBrightness) ) );
		} else {
			fillLeds( (int)(RControl * fadeBrightness) , (int)(GControl * fadeBrightness) , (int)(BControl * fadeBrightness) );
		}
	} else if ( stripeControl ) {
		cautionStripes( CRGB( RControl , GControl , BControl ) );
	} else { // solid:
		fillLeds( RControl , GControl , BControl );
	}
	
	FastLED.show();
}

void fillLeds(int r, int g, int b){
	for(int i = 0; i < NUM_LEDS; i++){
		leds[i] = CRGB(r, g, b);
	}
	FastLED.show();
}
void fadeAll() {
	for(int i = 0; i < 100; i++){
		leds.fadeToBlackBy(10);
		FastLED.delay(1000/UPDATES_PER_SECOND);
		FastLED.show();
		
	}
}
void cautionStripes ( CRGB color ) {
	for ( int i = 0 ; i < NUM_LEDS ; i++ ) {
		if ( (i % 4) < 2 ) {
			leds[i] = color;
		} else {
			leds[i] = CRGB::Black;
		}
	}
}
/*void flash(CRGB color, int count, int del){
	for(int i = 0; i < count; i++){
		if(Serial.available()){
			fillLeds(0,0,0);
			break;
		}
		fillLeds(color.r, color.g, color.b);
		millidel(del);
		fillLeds(0,0,0);
		millidel(del);
	}
}*/
void millidel(int del){
	int start = millis();
	while(millis() <= start + del){
		if(Serial.available()){
			break;
		}
	}
}
