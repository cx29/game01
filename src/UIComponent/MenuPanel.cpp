//
// Created by 二九 on 2025/8/3.
//

#include"MenuPanel.h"               // 包含对应头文件，提供类声明
#include<SDL_ttf.h>                 // SDL字体库

//初始化面板位置和尺寸, :后面为初始化类的成员
MenuPanel::MenuPanel(SDL_Renderer *renderer, int x, int y, int width, int height): renderer(renderer), x(x), y(y),
    width(width), height(height) {
    font = TTF_OpenFont("resource/ttf/ipix.ttf", 20);
    if (!font) {
        SDL_Log("Failed to load font:%s",TTF_GetError());
    }
}

//添加顶级菜单项
void MenuPanel::addMenuItem(const MenuItem &item) {
    items.push_back(item);
}

// 处理SDL事件（鼠标点击事件）
void MenuPanel::handleEvent(const SDL_Event &event) {
    //判断是否是鼠标左键点击
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x; // 获取鼠标X坐标
        int mouseY = event.button.y; // 获取鼠标的Y坐标

        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
            int offsetY = y; // 当前绘制菜单项的Y轴偏移
            for (auto &item: items) {
                if (handleClick(item, offsetY, 0, mouseX, mouseY)) {
                    break; // 如果点击命中某项菜单则结束循环
                }
            }
        }
    }
}

//递归处理点击事件，检测点击的菜单项
bool MenuPanel::handleClick(MenuItem &item, int &offsetY, int depth, int mouseX, int mouseY) {
    int itemY = offsetY; //当前菜单项的垂直位置
    offsetY += itemHeight; // 每处理一个菜单项，下移高度

    // 判断点击是否在当前菜单项的范围内
    if (mouseY >= itemY && mouseY <= itemY + itemHeight) {
        // 如果有子菜单则展开/折叠
        if (!item.children.empty()) {
            item.expanded = !item.expanded;
        }
        // 如果没有子菜单，有回调函数则执行
        else if (item.onClick) {
            item.onClick();
        }
        return true; // 表示点击命中
    }

    //如果该菜单是展开的状态，则递归检查子菜单
    if (item.expanded) {
        for (auto &child: item.children) {
            if (handleClick(child, offsetY, depth + 1, mouseX, mouseY)) {
                return true;
            }
        }
    }
    return false;
}

//渲染整个菜单面板
void MenuPanel::render() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // 深灰色
    SDL_Rect panelRect = {x, y, width, height}; //菜单矩形区域
    // 填充背景
    SDL_RenderFillRect(renderer, &panelRect);
    int offsetY = y; //从顶部开始渲染
    //如果是展开状态，渲染子菜单；
    for (auto &item: items) {
        renderItem(item, offsetY, 0); //递归渲染菜单
    }
}

// 递归渲染菜单项
void MenuPanel::renderItem(const MenuItem &item, int &offsetY, int depth) {
    // 设置菜单项背景色
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect rect = {x + depth * 20, offsetY, width - depth * 20, itemHeight};
    //x位置随层级缩进（depth*20），表现为树形结构
    SDL_RenderFillRect(renderer, &rect);

    if (font) {
        SDL_Surface *textSurface = TTF_RenderText_Blended(font, item.title.c_str(), textColor);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                int textW = 0, textH = 0;
                TTF_SizeText(font, item.title.c_str(), &textW, &textH);
                SDL_Rect dstRect = {x + depth * 20 + 5, offsetY + (itemHeight - textH) / 2, textW, textH};
                SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }

    offsetY += itemHeight; //渲染完一个菜单项， Y坐标下移
    // 如果是展开状态，则渲染子菜单
    if (item.expanded) {
        for (auto &child: item.children) {
            renderItem(child, offsetY, depth + 1);
        }
    }
}

//更新菜单（可用于菜单动画）
void MenuPanel::update(double delta) {
}

MenuPanel::~MenuPanel() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}
