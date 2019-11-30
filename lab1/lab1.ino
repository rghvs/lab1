#include "button.h"
#define BUTTON_PIN0 9
#define BUTTON_PIN1 10
#define BUTTON_PIN2 11
#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5
#define TIME1 450
#define TIME2 80
#define NUMBER_OF_BLINKS 6
#define NUMBER_OF_COMPONENTS 3
#define MAX_COLOR_VALUE 255
#define MIN_COLOR_VALUE 0
#define NEW_COLOR_VALUE 240

Button button0(BUTTON_PIN0);
Button button1(BUTTON_PIN1);
Button button2(BUTTON_PIN2);
Button buttons[] = { button0, button1, button2 };
char pinsAndValues[3][2] =
{
    { RED_PIN, MIN_COLOR_VALUE },
    { GREEN_PIN, MIN_COLOR_VALUE },
    { BLUE_PIN, MIN_COLOR_VALUE }
};
char indexPin1 = 0;
char indexPin2 = 1;
char state = 0;
char isBlink = 0;
char countBlinks = 0;

void setup()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    Serial.begin(9600);
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


enum
{
    SINGLE_COLOR,
    BOTH_COLORS,
    BLINK_SINGLE_COLOR,
    BLINK_BOTH_COLORS,
    PAUSE_END_BLINK,
    PAUSE_END_SMOOTH,
};

void bothColors()
{
    analogWrite(pinsAndValues[indexPin2][0], pinsAndValues[indexPin2][1]);
    analogWrite(pinsAndValues[indexPin1][0], pinsAndValues[indexPin1][1]);
    if (isBlink)
        if (++countBlinks < 3)
            state = BLINK_BOTH_COLORS;
        else
        {
            countBlinks = 0;
            state = BLINK_SINGLE_COLOR;
            ++indexPin1;
            ++indexPin2;
        }
    else
    {
        state = SINGLE_COLOR;
        ++indexPin1;
        ++indexPin2;
    }
    if (indexPin2 == 3)
        indexPin2 = 0;
    else if (indexPin1 == 3)
        state = 5;
}

void singleColor()
{
    analogWrite(pinsAndValues[indexPin1][0], pinsAndValues[indexPin1][1]);
    if (isBlink)
        if (++countBlinks < 3)
            state = BLINK_SINGLE_COLOR;
        else
        {
            countBlinks = 0;
            state = BLINK_BOTH_COLORS;
        }
    else
        state = BOTH_COLORS;
}

void loop()
{
    changeColorsValues();
        switch (state)
        {
            case BOTH_COLORS:
                bothColors();
                break;
            case SINGLE_COLOR:
                singleColor();
                break;
            case BLINK_SINGLE_COLOR:
                if (indexPin1 == 0 && countBlinks == 0)
                    delay(TIME1 - TIME2);
                state = 0;
                break;
            case BLINK_BOTH_COLORS:
                state = BOTH_COLORS;
                break;
            case PAUSE_END_BLINK:
                state = SINGLE_COLOR;
                break;
            case PAUSE_END_SMOOTH:
                delay(TIME1 - TIME2);
                indexPin1 = 0;
                isBlink = !isBlink;
                state = state == SINGLE_COLOR ? BLINK_SINGLE_COLOR : PAUSE_END_BLINK;
                break;
        }
    delay((state < 2 || state == PAUSE_END_SMOOTH) && !isBlink ? TIME1 : TIME2);
}
