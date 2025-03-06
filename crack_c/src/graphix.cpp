#include <stdio.h>
#include "graphics.h"
#include "crack.h"

const RGBColor COLOR_VELOCITY = {1, 2, 3};

void GraphEngine()
{
    int window_size_x = GetSystemMetrics (SM_CXSCREEN) / 2;
    int window_size_y = GetSystemMetrics (SM_CYSCREEN) / 2;
    txCreateWindow(window_size_x, window_size_y);

    Button start_dvd_button = {};
    ButtonCtor(&start_dvd_button, {window_size_x / 2, window_size_y / 2}, {DEFAULT_BUTTON_SIZE * 3, DEFAULT_BUTTON_SIZE * 2}, {255, 0, 0}, {255, 0, 0}, {}, {}, START_DVD_BUTTON_TEXT);
    DrawButton(&start_dvd_button);

    Vector2 window_sizes = {window_size_x, window_size_y};
    RunDVD(&start_dvd_button, window_sizes);                        // передвигает кнопку как в двд до тех пор, пока на неё не нажмут
    txPlaySound("intro.wav");                                       // скам
    txSleep(3);

    Button res_message  = {};
    ButtonCtor(&res_message, {window_size_x / 2, DEFAULT_BUTTON_SIZE * 3 / 2 + window_size_y / 8}, {DEFAULT_BUTTON_SIZE * 5, DEFAULT_BUTTON_SIZE * 3}, RGB_GREEN, RGB_LIGHTCYAN, {}, {}, "");
    
    bool user_choice = DialogWindow();

    if (user_choice == USER_ANSWER_YES)
    {
        CrackProg();
        res_message.text = "Successfully crack";
    }

    else
    {
        res_message.border_color = RGB_RED;
        res_message.bckg_color   = RGB_LIGHTCYAN;
        res_message.text = "Exit..";
    }

    DrawButton(&res_message);
}

void ButtonCtor(Button *button, Vector2 center, Vector2 sizes, 
                RGBColor border_color, RGBColor bckg_color, RGBColor click_border_color, RGBColor click_bckg_color, 
                const char *text)
{
    assert(button);
    assert(text);

    button->center = center; // TODO: assert
    button->sizes  = sizes;

    button->border_color = border_color;
    button->bckg_color   = bckg_color;

    button->click_border_color = click_border_color;
    button->click_bckg_color   = click_bckg_color;

    button->text = text;
}

void DrawButton(Button *button)
{
    double lu_corner_x = double (button->center.x - button->sizes.x / 2);  // левый  верхний угол
    double lu_corner_y = double (button->center.y - button->sizes.y / 2);

    double rd_corner_x = double (button->center.x + button->sizes.x / 2);  // правый нижний
    double rd_corner_y = double (button->center.y + button->sizes.y / 2);

    txSetColor     (RGBtoCOLORREF(button->border_color), CRACK_BUTTON_BORDER_THICKNESS);
    txSetFillColor (RGBtoCOLORREF(button->bckg_color));

    txRectangle (lu_corner_x, lu_corner_y, rd_corner_x, rd_corner_y);

    // txSetColor(TX_WHITE);
    txDrawText  (lu_corner_x, lu_corner_y, rd_corner_x, rd_corner_y, button->text);
}

void RunDVD(Button *button, Vector2 window_sizes)                           // передвигает кнопку как в двд до тех пор, пока на неё не нажмут
{
    assert(button);

    Vector2 velocity = {CRACK_BUTTON_VELOCITY_X, CRACK_BUTTON_VELOCITY_Y};
    Vector2 window_refl_borders = SubVecToVec(window_sizes, button->sizes);

    RGBColor color_velocity = COLOR_VELOCITY;

    size_t prev_time = clock();
    
    txPlaySound("bckg_music.wav");

    while (CheckButtonClick(button) == false)
    {
        size_t cur_time = clock();  // время в тактах
        
        size_t delta_time = cur_time - prev_time;   // в тактах
        size_t periods_num = delta_time / (CLOCKS_PER_SEC / FPS);
        
        if (periods_num >= 1)
        {
            prev_time = cur_time;
            
            Vector2 motion = MulVecOnScalar(velocity, (int)periods_num);
            button->center = AddVecToVec(button->center, motion);            

            ColorGradient(&button->border_color, &color_velocity);
            // ColorGradient(&button->bckg_color,   &color_velocity);

            DrawButton(button);
            CheckReflection(button, &velocity, window_refl_borders);
        }
    }
    
    return;
}

Vector2 AddVecToVec(Vector2 vec1, Vector2 vec2)
{
    return {vec1.x + vec2.x, vec1.y + vec2.y};
}

Vector2 SubVecToVec(Vector2 vec1, Vector2 vec2)
{
    return {vec1.x - vec2.x, vec1.y - vec2.y};
}

Vector2 MulVecOnScalar(Vector2 vec, int scalar)
{
    return {vec.x * scalar, vec.y * scalar};
}

Vector2 DivVecOnScalar(Vector2 vec, int scalar)
{
    return {vec.x / scalar, vec.y / scalar};
}

void ColorGradient(RGBColor *color, RGBColor *color_velocity)
{
    assert(color);
    assert(color_velocity);

    if ((color->red >= 254 && color_velocity->red > 0) || (color->red <= 1 && color_velocity->red <= 0))   // переполнение && движение в сторону ещё большего переполнения
        color_velocity->red *= -1;

    if ((color->blue >= 254 && color_velocity->blue > 0) || (color->blue <= 1 && color_velocity->blue <= 0))
        color_velocity->blue *= -1;

    if ((color->green >= 254 && color_velocity->green > 0) || (color->green <= 1 && color_velocity->green <= 0))
        color_velocity->green *= -1;

    color->red   += color_velocity->red;
    color->blue  += color_velocity->blue; 
    color->green += color_velocity->green;
}

bool CheckReflection(Button *button, Vector2 *button_velocity, Vector2 window_refl_borders)    // window_refl_borders = условные границы, по достижении которых центром рамки она должна отразиться
{
    assert(button);
    assert(button_velocity);

    Vector2 window_center = {txGetExtentX() / 2, txGetExtentY() / 2};

    bool reflection_right = button->center.x > (window_center.x + window_refl_borders.x / 2);
    bool reflection_left  = button->center.x < (window_center.x - window_refl_borders.x / 2);

    bool reflection_up    = button->center.y > (window_center.y + window_refl_borders.y / 2);
    bool reflection_down  = button->center.y < (window_center.y - window_refl_borders.y / 2);

    if ((reflection_right && button_velocity->x > 0) || (reflection_left && button_velocity->x < 0))
    {
        button_velocity->x *= -1;
        return true;        // есть отражение
    }

    if ((reflection_up && button_velocity->y > 0) || (reflection_down && button_velocity->y < 0))
    {
        button_velocity->y *= -1;
        return true;
    }

    return false;
}

COLORREF RGBtoCOLORREF(RGBColor rgb_color)
{
    return RGB(rgb_color.red, rgb_color.green, rgb_color.blue);
}

CornersCoords CenterAndSizeToCornersCoords(Vector2 center, Vector2 sizes)
{
    CornersCoords corner_coords = {};

    corner_coords.lu_x = center.x - sizes.x;
    corner_coords.lu_y = center.y - sizes.y;
    corner_coords.rd_x = center.x + sizes.x;
    corner_coords.rd_y = center.y + sizes.y;

    return corner_coords;
}

bool CheckButtonClick(Button *button)
{
    assert(button);
    return (txMouseButtons() == 1 && MousePointsOnButton(button));  // нажатие на ЛКМ + наведение на кнопку
}

bool MousePointsOnButton(Button *button)
{
    assert(button);

    Vector2 mouse_pos = {(int)txMouseX(), (int)txMouseY()};

    int lu_corner_x = button->center.x - button->sizes.x / 2;   // left-up corner of button
    int lu_corner_y = button->center.y - button->sizes.y / 2;
    int rd_corner_x = button->center.x + button->sizes.x / 2;   // right-down corner of button
    int rd_corner_y = button->center.y + button->sizes.y / 2;
    
    if (mouse_pos.x <= rd_corner_x && mouse_pos.x >= lu_corner_x
     && mouse_pos.y <= rd_corner_y && mouse_pos.y >= lu_corner_y)
        return true;
    
    else
        return false;
}

UserAnswer DialogWindow()   // вопрос запустить кряк или нет (выйти)
{
    Vector2 window_center = {txGetExtentX() / 2, txGetExtentY() / 2};

    const char *question_text = "Start vzloma?";
    const char *question_text_mousepointing = "Sosal?";

    // draw question
    Button question_button = {};
    Vector2 question_center = AddVecToVec(window_center, {0, -DEFAULT_BUTTON_SIZE});
    ButtonCtor(&question_button, question_center, {DEFAULT_BUTTON_SIZE * 4, DEFAULT_BUTTON_SIZE * 2}, RGB_BLUE,  RGB_LIGHTCYAN, RGB_LIGHTCYAN, RGB_BLUE, question_text);

    DrawButton(&question_button);

    // yes
    Button yes_button = {};
    Vector2 ans_yes_center = AddVecToVec(window_center, {-DEFAULT_BUTTON_SIZE * 2, DEFAULT_BUTTON_SIZE * 2});
    ButtonCtor(&yes_button, ans_yes_center, {DEFAULT_BUTTON_SIZE * 2, DEFAULT_BUTTON_SIZE * 2}, RGB_GREEN,  RGB_LIGHTCYAN, RGB_LIGHTCYAN, RGB_GREEN, "YES");
    DrawButton(&yes_button);
    
    // no
    Button no_button = {};
    Vector2 ans_no_center = AddVecToVec(window_center, {DEFAULT_BUTTON_SIZE * 2, DEFAULT_BUTTON_SIZE * 2});
    ButtonCtor(&no_button, ans_no_center, {DEFAULT_BUTTON_SIZE * 2, DEFAULT_BUTTON_SIZE * 2}, RGB_RED,  RGB_LIGHTCYAN, RGB_LIGHTCYAN, RGB_RED, "NO");
    DrawButton(&no_button);


    bool mouse_points_on_yes_button = false;
    bool mouse_points_on_no_button  = false;
    
    while (true)
    {
        if (CheckButtonClick(&yes_button))
            return USER_ANSWER_YES;

        if (CheckButtonClick(&no_button))
            return USER_ANSWER_NO;

        DrawPointingOnButton(&yes_button, &mouse_points_on_yes_button);
        DrawPointingOnButton(&no_button,  &mouse_points_on_no_button);

        if (mouse_points_on_yes_button && question_button.text != question_text_mousepointing)
        {
            question_button.text = question_text_mousepointing;
            DrawButton(&question_button);
        }

        else if (!mouse_points_on_yes_button && question_button.text == question_text_mousepointing)
        {
            question_button.text = question_text;
            DrawButton(&question_button);
        }
    }
    
    no_button.bckg_color   = RGB_LIGHTCYAN;
    no_button.border_color = RGB_RED; 
    DrawButton(&no_button);

    yes_button.bckg_color = RGB_LIGHTCYAN;
    yes_button.border_color = RGB_GREEN;
    DrawButton(&yes_button);

    return USER_ANSWER_YES;
}

void DrawPointingOnButton(Button *button, bool *points_on_prev_frame)
{
    if (MousePointsOnButton(button))   // сейчас указывает на кнопку
    {
        if (*points_on_prev_frame == false)        // но раньше не указывал
        {
            SwapColor(&button->bckg_color,   &button->click_bckg_color);
            SwapColor(&button->border_color, &button->click_border_color);
            
            DrawButton(button);
            
            *points_on_prev_frame = true;
        }
    }

    else
    {
        if (*points_on_prev_frame)         // раньше указывала, теперь нет
        {
            SwapColor(&button->bckg_color,   &button->click_bckg_color);
            SwapColor(&button->border_color, &button->click_border_color);
            
            DrawButton(button);
        }
        
        *points_on_prev_frame = false;
    }
}

void SwapColor(RGBColor *color_1, RGBColor *color_2)
{
    RGBColor color_tmp = *color_1;
    *color_1 = *color_2;
    *color_2 = color_tmp;
}