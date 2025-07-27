#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Manager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>

class GameManager : public Manager<GameManager> {
    friend class Manager<GameManager>; // 允许基类访问私有构造函数
private:
    SDL_Window *window = nullptr;
    SDL_Renderer *render = nullptr;
    SDL_Event event;
    bool is_quit = false;

private:
    GameManager(); // 私有构造函数，防止外部实例化
    ~GameManager();

    //初始化断言函数， 检查SDL是否初始化成功
    void Init_Assert(bool flag, const char *err_msg) {
        if (flag)//false则初始化失败
            return;
        //则弹窗
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"游戏启动失败", err_msg,
                                 window);
        exit(-1);
    }

    //处理键盘，鼠标等输入事件
    void on_input();

    //更新游戏状态（位置，动画等）,通常delta为时间差参数
    void on_update(double delta);

    //渲染画面到窗口上， 通过Renderer
    void on_render();

public:
    int run(int argc, char **argv);
};

#endif // GAME_MANAGER_H
