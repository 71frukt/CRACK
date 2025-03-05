#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "TXLib.h"
#include "TXWave.h"

const size_t  WINDOW_WIDTH = 800;
const size_t  WINDOW_HIGHT = 600;

const int DEFAULT_SIZE = WINDOW_WIDTH / 20;

const size_t  CRACK_BUTTON_WIDTH = 100;
const size_t  CRACK_BUTTON_HIGHT = 80;
const size_t  CRACK_BUTTON_BORDER_THICKNESS = 5;
const char   *const START_DVD_BUTTON_TEXT = "CATCH ME!";

const COLORREF DIALOG_WINDOW_COLOR = TX_GRAY;

const size_t  CRACK_BUTTON_VELOCITY_X = 2;      // pixels per shot
const size_t  CRACK_BUTTON_VELOCITY_Y = 2;      // pixels per shot
const size_t  FPS = 120;                        // shots  per sec

const size_t BUTTON_TEXT_LEN = 100;


struct Vector2
{
    int x;
    int y;
};

struct RGBColor
{
    short red;
    short green;
    short blue;
};

struct Button
{
    Vector2 center;
    Vector2 sizes;
    
    RGBColor border_color;
    RGBColor bckg_color;
    
    const char* text;
};

struct CornersCoords
{
    int lu_x;
    int lu_y;
    int rd_x;
    int rd_y;
};

enum UserAnswer
{
    USER_ANSWER_YES,
    USER_ANSWER_NO
};

void GraphEngine         ();
UserAnswer DialogWindow  ();   // вопрос запустить кряк или нет (выйти)
void RunDVD              (Button *button, Vector2 window_sizes);
void ButtonCtor          (Button *button, Vector2 center, Vector2 sizes, RGBColor border_color, RGBColor bckg_color, const char *text);
void DrawButton          (Button *button);
bool CheckReflection     (Button *button, Vector2 *button_velocity, Vector2 window_refl_borders);    // window_refl_borders = условные границы, по достижении которых центром рамки она должна отразиться
bool MousePointsOnButton (Button *button);
bool CheckButtonClick    (Button *button);


Vector2 AddVecToVec    (Vector2 vec1, Vector2 vec2);
Vector2 SubVecToVec    (Vector2 vec1, Vector2 vec2);
Vector2 MulVecOnScalar (Vector2 vec, int scalar);
Vector2 DivVecOnScalar (Vector2 vec, int scalar);

COLORREF RGBtoCOLORREF (RGBColor rgb_color);
void     ColorGradient (RGBColor *color, RGBColor *color_velocity);

CornersCoords CenterAndSizeToCornersCoords(Vector2 center, Vector2 sizes);


#endif