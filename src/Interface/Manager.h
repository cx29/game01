#ifndef MANAGER_H
#define MANAGER_H

template<typename T>
class Manager {//模板类，用于泛型编程，使用CRTP实现静态多态
protected:
    //delete禁止拷贝
    Manager(const Manager &) = delete;//默认构造函数，允许子类构造

    Manager &operator=(const Manager &) = delete;

    Manager() = default;

    virtual ~Manager() = default;//虚析构，支持子类析构时正确调用

public:
    static T &getInstance() {
        static T instance;
        return instance;
    }
};

#endif // Manager_H
