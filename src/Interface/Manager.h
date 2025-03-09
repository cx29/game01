#ifndef MANAGER_H
#define MANAGER_H

template <typename T> class Manager {
protected:
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;
  Manager() = default;
  virtual ~Manager() = default;

public:
  static T &getInstance() {
    static T instance;
    return instance;
  }
};

#endif // Manager_H
