//
// Created by 二九 on 2025/8/3.
//

#ifndef MENUPANEL_H                         // 防止头文件重复包含
#define MENUPANEL_H

#include <SDL.h>                            // 提供窗口，渲染，事件等功能
#include<string>                            // 字符串库
#include<vector>                            // 存储菜单列表项，集合
#include <functional>                       // 定义菜单点击回调函数
#include <SDL_ttf.h>

// 菜单项结构体， 表示树形菜单中的一个节点
struct MenuItem {
    std::string title; // 菜单项的标题
    std::vector<MenuItem> children; // 子菜单项
    bool expanded = false; // 是否展开子菜单
    std::function<void()> onClick; // 点击此菜单项时的回调函数
};

//负责管理和渲染树形菜单
class MenuPanel {
public:
    //构造函数，传入渲染器和面板的位置和大小
    MenuPanel(SDL_Renderer *renderer, int x, int y, int width, int height);
    ~MenuPanel();

    // 处理SDL事件（鼠标点击事件）
    void handleEvent(const SDL_Event &event);

    // 渲染菜单到屏幕
    void render();

    // 更新菜单状态
    void update(double delta);

    // 添加一个顶级菜单项
    void addMenuItem(const MenuItem &item);

private:
    SDL_Renderer *renderer; //SDL渲染器
    int x, y, width, height; //面板的坐标和尺寸
    std::vector<MenuItem> items; // 顶级菜单项列表
    int itemHeight = 30; // 每个菜单项的高度
    TTF_Font *font;
    SDL_Color textColor = {255, 255, 255, 255};

    // 递归渲染一个菜单项以及其子项
    void renderItem(const MenuItem &item, int &offsetY, int depth);

    // 递归检测鼠标点击的菜单项
    bool handleClick(MenuItem &item, int &offsetY, int depth, int mouseX, int mouseY);
};
#endif //MENUPANEL_H
