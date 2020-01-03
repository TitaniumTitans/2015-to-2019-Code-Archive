/***declaring RGB pins in PWM mode***/
int red = 10;
int green = 9;
int blue = 11;

/***brightness control variables***/
int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;
  
  void setup() {
  
    Serial.begin(9600);         //initialising serial monitor
  
    /***Pin mode declaration***/
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
    
  }
  
  void loop() {
    
    Serial.println("::Enter color brightness::");
    
    Serial.print("Red : ");         //for red
    while(Serial.available()==0){
    }
    redBrightness = Serial.parseInt();
    Serial.println(redBrightness);
    
    Serial.print("Green : ");         //for green
    while(Serial.available()==0){
    }
    greenBrightness = Serial.parseInt();
    Serial.println(greenBrightness);
  
    Serial.print("Blue : ");         //for blue
    while(Serial.available()==0){
    }
    blueBrightness = Serial.parseInt();
    Serial.println(blueBrightness);
    Serial.println("   ");
  
    /***sending variable voltages to RGB led***/
    analogWrite(red, redBrightness);
    analogWrite(green, greenBrightness);
    analogWrite(blue, blueBrightness);
    delay(3000);
    
    }
