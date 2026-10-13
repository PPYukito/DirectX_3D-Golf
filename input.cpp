/*==============================================================================

   入力管理ラッパー [input.cpp]
                                                         Author : Youhei Sato
--------------------------------------------------------------------------------

==============================================================================*/
#include "input.h"


// キーリピート管理用の最大キー数（Keyboard_Keys は最大 0xFE）
static const int KEY_MAX = 0xFF;

// 前フレームのキーボード状態
static Keyboard_State gPrevKeyState = {};

// 現在フレームのキーボード状態
static Keyboard_State gCurrKeyState = {};

// 各キーの押下継続フレーム数（リピート用）
static int gKeyRepeatCount[KEY_MAX] = {};

// 前フレームのマウスボタン状態
static bool gPrevMouseButton[3] = {};

// 現在フレームのマウス状態
static Mouse_State gMouseState = {};


// ========== 初期化 / 終了 / 更新 ==========

void Input_Initialize(HWND hWnd)
{
    Keyboard_Initialize();
    Mouse_Initialize(hWnd);

    Keyboard_Reset();
    ZeroMemory(&gPrevKeyState, sizeof(gPrevKeyState));
    ZeroMemory(&gCurrKeyState, sizeof(gCurrKeyState));
    ZeroMemory(gKeyRepeatCount, sizeof(gKeyRepeatCount));

    ZeroMemory(&gMouseState, sizeof(gMouseState));
    ZeroMemory(gPrevMouseButton, sizeof(gPrevMouseButton));
}

void Input_Update(void)
{
    // --- キーボード ---
    // 1フレーム前のスナップショットを保持し、最新状態を取り込む
    gPrevKeyState = gCurrKeyState;
    gCurrKeyState = *Keyboard_GetState();

    // --- マウス ---
    // 前フレームのボタン状態を保存
    gPrevMouseButton[MOUSE_BUTTON_LEFT]   = gMouseState.leftButton;
    gPrevMouseButton[MOUSE_BUTTON_RIGHT]  = gMouseState.rightButton;
    gPrevMouseButton[MOUSE_BUTTON_MIDDLE] = gMouseState.middleButton;

    // 現在のマウス状態を取得
    Mouse_GetState(&gMouseState);

    // --- キーリピートカウンタの更新 ---
    for (int i = 0; i < KEY_MAX; ++i)
    {
        Keyboard_Keys k = static_cast<Keyboard_Keys>(i);
        if (Keyboard_IsKeyDown(k, &gCurrKeyState))
        {
            ++gKeyRepeatCount[i];
        }
        else
        {
            gKeyRepeatCount[i] = 0;
        }
    }
}

void Input_Finalize(void)
{
    Mouse_Finalize();
}


// ========== キーボード ==========

bool Input_IsKeyPress(Keyboard_Keys key)
{
    return Keyboard_IsKeyDown(key, &gCurrKeyState);
}

bool Input_IsKeyTrigger(Keyboard_Keys key)
{
    // 前フレームが OFF で現在フレームが ON なら trigger
    bool prev = Keyboard_IsKeyDown(key, &gPrevKeyState);
    bool curr = Keyboard_IsKeyDown(key, &gCurrKeyState);
    return (!prev && curr);
}

bool Input_IsKeyRelease(Keyboard_Keys key)
{
    // 前フレームが ON で現在フレームが OFF なら release
    bool prev = Keyboard_IsKeyDown(key, &gPrevKeyState);
    bool curr = Keyboard_IsKeyDown(key, &gCurrKeyState);
    return (prev && !curr);
}

bool Input_IsKeyRepeat(Keyboard_Keys key, int delay, int interval)
{
    if (delay < 1) { delay = 1; }
    if (interval < 1) { interval = 1; }

    int count = gKeyRepeatCount[static_cast<int>(key)];
    if (count <= 0) { return false; }
    if (count == 1) { return true; }          // 押した瞬間
    if (count < delay) { return false; }      // delay フレームまで待機
    return ((count - delay) % interval == 0); // interval ごとに発火
}


// ========== マウス ==========

bool Input_IsMousePress(MouseButton button)
{
    switch (button)
    {
    case MOUSE_BUTTON_LEFT:   return gMouseState.leftButton;
    case MOUSE_BUTTON_RIGHT:  return gMouseState.rightButton;
    case MOUSE_BUTTON_MIDDLE: return gMouseState.middleButton;
    default:                  return false;
    }
}

bool Input_IsMouseTrigger(MouseButton button)
{
    bool prev = gPrevMouseButton[button];
    bool curr = Input_IsMousePress(button);
    return (!prev && curr);
}

bool Input_IsMouseRelease(MouseButton button)
{
    bool prev = gPrevMouseButton[button];
    bool curr = Input_IsMousePress(button);
    return (prev && !curr);
}

int Input_GetMouseX(void)
{
    return gMouseState.x;
}

int Input_GetMouseY(void)
{
    return gMouseState.y;
}

int Input_GetMouseScrollWheel(void)
{
    return gMouseState.scrollWheelValue;
}
