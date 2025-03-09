#include "GameManager.h"

GameManager::GameManager() {
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init((IMG_INIT_JPG | IMG_INIT_PNG));
  Mix_Init(MIX_INIT_MP3);
  TTF_Init();

  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  // 显示候选词列表，默认是关闭的
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

  window =
      SDL_CreateWindow(u8"村庄保卫战!", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

  // 硬件加速，垂直同步, 设置渲染目标为Texture（纹理，生成瓦片地图时用）
  render =
      SDL_CreateRenderer(window, -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                             SDL_RENDERER_TARGETTEXTURE);
  std::cout << "GameManager Created" << std::endl;
}

int GameManager::run(int argc, char **argv) {
  Uint64 last_counter = SDL_GetPerformanceCounter();
  const Uint64 counter_freq = SDL_GetPerformanceFrequency(); // 获取频率

  while (!is_quit) {
    while (SDL_PollEvent(&event))
      on_input();
    // 更新数据

    Uint64 current_counter = SDL_GetPerformanceCounter();
    double delta = (double)(current_counter - last_counter) / counter_freq;
    last_counter = current_counter;

    if (delta * 1000 < 1000.0 / 60) {
      SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));
    }

    on_update(delta);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);

    // 绘制画面
    on_render();
    SDL_RenderPresent(render);
  }
  return 0;
}

void GameManager::on_input() {}

void GameManager::on_render() {}

void GameManager::on_update(double delta) { std::cout << delta << std::endl; }

GameManager::~GameManager() {
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);

  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
}
