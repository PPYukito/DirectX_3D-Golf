/*==============================================================================

   アプリケーションクラス [application.h]
                                                         Author : Youhei Sato
                                                         Date   : 2026/04/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <Windows.h>

#include "input.h"

class Application
{
private:
    float m_x{}, m_y{};

public:
    Application(HWND hWnd);
    ~Application();

    void Update(float delta_time);
    void Draw();
};

#endif // APPLICATION_H
