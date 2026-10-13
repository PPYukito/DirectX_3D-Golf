/*==============================================================================

   入力管理ラッパー [input.h]
                                                         Author : Youhei Sato
--------------------------------------------------------------------------------
   keyboard.h / mouse.h をラップし、以下の入力状態を提供します。
     - Press   : キー/ボタンを押し続けている間 true
     - Trigger : キー/ボタンを押した瞬間だけ true
     - Release : キー/ボタンを離した瞬間だけ true
     - Repeat  : 押し続けると delay フレーム後から interval フレームごとに true
==============================================================================*/
#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include "keyboard.h"
#include "mouse.h"


// -------- キーボード --------

// 押し続けている間 true
bool Input_IsKeyPress(Keyboard_Keys key);

// 押した瞬間だけ true
bool Input_IsKeyTrigger(Keyboard_Keys key);

// 離した瞬間だけ true
bool Input_IsKeyRelease(Keyboard_Keys key);

// キーリピート
//   最初に delay フレーム押し続けると反応し、
//   その後 interval フレームごとに true を返す
bool Input_IsKeyRepeat(Keyboard_Keys key, int delay = 20, int interval = 5);


// -------- マウス --------

// マウスボタン番号
enum MouseButton
{
    MOUSE_BUTTON_LEFT   = 0,
    MOUSE_BUTTON_RIGHT  = 1,
    MOUSE_BUTTON_MIDDLE = 2,
};

// ボタンを押し続けている間 true
bool Input_IsMousePress(MouseButton button);

// ボタンを押した瞬間だけ true
bool Input_IsMouseTrigger(MouseButton button);

// ボタンを離した瞬間だけ true
bool Input_IsMouseRelease(MouseButton button);

// マウスの現在座標を取得
int Input_GetMouseX(void);
int Input_GetMouseY(void);

// スクロールホイールの値を取得
int Input_GetMouseScrollWheel(void);


// -------- 全体管理 --------

// 入力系を初期化（Application コンストラクタで呼ぶ）
void Input_Initialize(HWND hWnd);

// 入力系を毎フレーム更新（Application::Update の先頭で呼ぶ）
void Input_Update(void);

// 入力系を終了（Application デストラクタで呼ぶ）
void Input_Finalize(void);

#endif // INPUT_H
