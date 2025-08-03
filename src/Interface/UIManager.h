//
// Created by 二九 on 2025/8/3.
//

#ifndef UIMANAGER_H
#define UIMANAGER_H
#include "Manager.h"
#include "../UIComponent/MenuPanel.h"
#include "../UIComponent/ContentPanel.h"
#include <SDL.h>

class UIManager : public Manager<UIManager> {
    friend class Manager<UIManager>;

private:
    SDL_Renderer *renderer = nullptr;

    MenuPanel *menu = nullptr;
    ContentPanel *content = nullptr;

    UIManager();

    ~UIManager() = default;

public:
    void init(SDL_Renderer *renderer);

    void handleEvent(const SDL_Event &event);

    void render();

    void update(double delta);
};
#endif //UIMANAGER_H
