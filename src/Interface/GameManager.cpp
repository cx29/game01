#include "GameManager.h"

//构造函数
GameManager::GameManager() {
    //初始化视频，音频，定时器，时间等所有子系统
    SDL_Init(SDL_INIT_EVERYTHING);
    //支持JPG和PNG图像加载
    IMG_Init((IMG_INIT_JPG | IMG_INIT_PNG));
    //支持MP3音频解码
    Mix_Init(MIX_INIT_MP3);
    //初始化字体支持
    TTF_Init();

    //打开音频设备
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    // 设置中文输入法显示候选词列表，默认是关闭的
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    window = SDL_CreateWindow(u8"案例合集", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

    // 硬件加速，垂直同步, 设置渲染目标为Texture（纹理，生成瓦片地图时用）
    render = SDL_CreateRenderer(window, -1,
                                SDL_RENDERER_ACCELERATED | //GPU加速
                                SDL_RENDERER_PRESENTVSYNC | // 垂直同步，防止画面撕裂
                                SDL_RENDERER_TARGETTEXTURE); // 渲染目标可以是纹理， 允许离屏渲染
    std::cout << "GameManager Created" << std::endl;
}

// 整个软件入口
int GameManager::run(int argc, char **argv) {
    //获取当前CPU高精度计数器的值
    Uint64 last_counter = SDL_GetPerformanceCounter();
    const Uint64 counter_freq = SDL_GetPerformanceFrequency(); // 每秒多少个tick


    //主循环
    while (!is_quit) {
        //负责SDL的事件轮询
        while (SDL_PollEvent(&event)) {
            on_input();
        }
        // 帧时间计算（Delta）,上一帧到当前帧实际的时间差
        Uint64 current_counter = SDL_GetPerformanceCounter();
        double delta = (double) (current_counter - last_counter) / counter_freq;
        last_counter = current_counter;

        // 帧率限制（固定60FPS约每帧16.67毫秒）
        if (delta * 1000 < 1000.0 / 60) {
            SDL_Delay((Uint32) (1000.0 / 60 - delta * 1000));
        }

        //游戏逻辑更新
        on_update(delta);

        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);

        // 绘制画面
        on_render();
        SDL_RenderPresent(render);
    }
    return 0;
}

//处理SDL_Event， 比如按键，鼠标，窗口关闭等
void GameManager::on_input() {
    switch (event.type) {
        case SDL_QUIT:
            is_quit = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.repeat == 0) {
                SDL_Keycode key = event.key.keysym.sym;
                std::cout << "Key down: " << key << std::endl;
                if (key == SDLK_ESCAPE) {
                    is_quit = true;
                }
            }
            break;
        default:
            break;
    }
}

//渲染画面（贴图，UI，地图等）
void GameManager::on_render() {
}

// 更新游戏状态（逻辑，动画，碰撞等）
void GameManager::on_update(double delta) {
}

// 析构函数，防止内存泄露
GameManager::~GameManager() {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
