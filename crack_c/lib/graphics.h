#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "TXLib.h"
#include "TXWave.h"

const size_t  WINDOW_WIDTH = 800;
const size_t  WINDOW_HIGHT = 600;

const int DEFAULT_BUTTON_SIZE = WINDOW_WIDTH / 20;

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

const RGBColor RGB_RED        = {128, 0,   0  };
const RGBColor RGB_GREEN      = {0,   128, 0  };
const RGBColor RGB_BLUE       = {0,   0,   128};
const RGBColor RGB_LIGHTCYAN  = {0,   255, 255};
const RGBColor RGB_LIGHTGREEN = {0,   255, 128};


struct Button
{
    Vector2 center;
    Vector2 sizes;
    
    RGBColor border_color;
    RGBColor bckg_color;
    
    RGBColor click_border_color;    // ����� ��� ��������� ���� �� ������
    RGBColor click_bckg_color;
    
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

void ButtonCtor(Button *button, Vector2 center, Vector2 sizes, 
    RGBColor border_color, RGBColor bckg_color, RGBColor click_border_color, RGBColor click_bckg_color, 
    const char *text);

void DrawPointingOnButton(Button *button, bool *points_on_prev_frame);

void GraphEngine         ();
UserAnswer DialogWindow  ();   // ������ ��������� ���� ��� ��� (�����)
void RunDVD              (Button *button, Vector2 window_sizes);
void DrawButton          (Button *button);
bool CheckReflection     (Button *button, Vector2 *button_velocity, Vector2 window_refl_borders);    // window_refl_borders = �������� �������, �� ���������� ������� ������� ����� ��� ������ ����������
bool MousePointsOnButton (Button *button);
bool CheckButtonClick    (Button *button);


Vector2 AddVecToVec    (Vector2 vec1, Vector2 vec2);
Vector2 SubVecToVec    (Vector2 vec1, Vector2 vec2);
Vector2 MulVecOnScalar (Vector2 vec, int scalar);
Vector2 DivVecOnScalar (Vector2 vec, int scalar);

COLORREF RGBtoCOLORREF (RGBColor rgb_color);
void     ColorGradient (RGBColor *color,   RGBColor *color_velocity);
void     SwapColor     (RGBColor *color_1, RGBColor *color_2);

CornersCoords CenterAndSizeToCornersCoords(Vector2 center, Vector2 sizes);


#endif