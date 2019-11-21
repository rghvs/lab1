#define BUTTON_PIN0 9
#define BUTTON_PIN1 10
#define BUTTON_PIN2 11
#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5
#define TIME1 250
#define TIME2 80
#define NUMBER_OF_COMPONENTS 3
#define BRIGHT_VALUE 8

unsigned char redBright = BRIGHT_VALUE;
unsigned char greenBright = BRIGHT_VALUE;
unsigned char blueBright = BRIGHT_VALUE;

char isUpButton0 = 1;
char isUpButton1 = 1;
char isUpButton2 = 1;

void setup()
{
    pinMode(BUTTON_PIN0, INPUT);
    pinMode(BUTTON_PIN1, INPUT);
    pinMode(BUTTON_PIN2, INPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void changeColorValue(int buttonPin, char *isUpButton, unsigned char *bright)
{
    if (digitalRead(buttonPin) == HIGH)
    {
        if (*isUpButton)
        {
            *bright += BRIGHT_VALUE;
            *isUpButton = 0;
        }
    }
    else
        *isUpButton = 1;
}

void changeColorsValues()
{
    changeColorValue(BUTTON_PIN0, &isUpButton0, &redBright);
    changeColorValue(BUTTON_PIN1, &isUpButton1, &greenBright);
    changeColorValue(BUTTON_PIN2, &isUpButton2, &blueBright);
}

void setValueByPin(int pin, int value, int time)
{
    changeColorsValues();
    analogWrite(pin, value);
    delay(time);
}

void changeColorLEDSmoothly(int colorPin1, int colorValue1, int colorPin2, int colorValue2)
{
    setValueByPin(colorPin1, colorValue1, TIME1);
    setValueByPin(colorPin2, colorValue2, TIME1);
    changeColorsValues();
    analogWrite(colorPin1, 0);
}

void changeColorLEDBlinking(int colorPin1, int colorValue1, int colorPin2, int colorValue2)
{
    int i;
    for (i = 0; i < NUMBER_OF_COMPONENTS; ++i)
    {
        setValueByPin(colorPin1, colorValue1, TIME2);
        setValueByPin(colorPin1, 0, TIME2);
    }
    for (i = 0; i < NUMBER_OF_COMPONENTS; ++i)
    {
        analogWrite(colorPin1, colorValue1);
        setValueByPin(colorPin2, colorValue2, TIME2);
        analogWrite(colorPin1, 0);
        setValueByPin(colorPin2, 0, TIME2);
    }
}

void loop()
{
    changeColorLEDSmoothly(RED_PIN, redBright, GREEN_PIN, greenBright);
    changeColorLEDSmoothly(GREEN_PIN, greenBright, BLUE_PIN, blueBright);
    changeColorLEDSmoothly(BLUE_PIN, blueBright, RED_PIN, redBright);
    analogWrite(RED_PIN, 0);
    delay(TIME1);
    changeColorsValues();

    changeColorLEDBlinking(RED_PIN, redBright, GREEN_PIN, greenBright);
    changeColorLEDBlinking(GREEN_PIN, greenBright, BLUE_PIN, blueBright);
    changeColorLEDBlinking(BLUE_PIN, blueBright, RED_PIN, redBright);
    changeColorsValues();
    delay(TIME1 - TIME2);
}