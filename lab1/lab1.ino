#include "button.h"
#define BUTTON_PIN0 9
#define BUTTON_PIN1 10
#define BUTTON_PIN2 11
#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5
#define TIME1 250
#define TIME2 80
#define NUMBER_OF_BLINKS 6
#define NUMBER_OF_COMPONENTS 3
#define MAX_COLOR_VALUE 255
#define MIN_COLOR_VALUE 0
#define NEW_COLOR_VALUE 240

Button buttons[] =
{
    new Button(BUTTON_PIN0),
    new Button(BUTTON_PIN1),
    new Button(BUTTON_PIN2)
};
char pinsAndValues[3][2] =
{
    { RED_PIN, MIN_COLOR_VALUE },
    { GREEN_PIN, MIN_COLOR_VALUE },
    { BLUE_PIN, MIN_COLOR_VALUE }
};
char indexPin1 = 0;
char indexPin2 = 1;
char state = 1;
char isBlink = 0;
char countBlink = 0;

void setup()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void changeColorsValues()
{
    analogWrite(RED_PIN, MAX_COLOR_VALUE);
    analogWrite(GREEN_PIN, MAX_COLOR_VALUE);
    analogWrite(BLUE_PIN, MAX_COLOR_VALUE);
    for (int i = 0; i < NUMBER_OF_COMPONENTS; ++i)
        if (buttons[i].wasPressed())
            pinsAndValues[i][1] = NEW_COLOR_VALUE - pinsAndValues[i][1];
}

void loop()
{
    changeColorsValues();
    state = !state;
    if (!isBlink && !countBlink || state)
        analogWrite(pinsAndValues[indexPin1][0], pinsAndValues[indexPin1][1]);
    if (!isBlink && countBlink)
    {
        countBlink = 0;
        state = !state;
    }
    else if (state)
    {
        if (!isBlink || countBlink >= NUMBER_OF_BLINKS >> 1)
            analogWrite(pinsAndValues[indexPin2][0], pinsAndValues[indexPin2][1]);
        if (!isBlink || ++countBlink == NUMBER_OF_BLINKS)
        {
            if (++indexPin2 == NUMBER_OF_COMPONENTS)
                indexPin2 = 0;
            if (++indexPin1 == NUMBER_OF_COMPONENTS)
            {
                indexPin1 = 0;
                isBlink = !isBlink;
                if (isBlink)
                    delay(TIME1 - TIME2);
            }
            if (isBlink)
                countBlink = 0;
        }
    }
    delay(isBlink ? TIME2 : TIME1);
}
