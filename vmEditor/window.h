#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <memory>

class KeyListener;

class Window{
private:
    std::string fileName, content;
    int x, y;
    std::unique_ptr<KeyListener> keyListener;
public:
    Window(std::string fileName, std::unique_ptr<KeyListener> keyListener);
    ~Window();
    KeyListener* getKeyListener();
    void init();
    void render();
};

#endif
