#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <memory>

class KeyListener;
class ColorManager;
class Parser;
class Store;

class Window{
    class Cursor{
    private:
        int y;
        int x;
        Window *window;
    public:
        Cursor(int y, int x, Window *window);
        ~Cursor() = default;
        void moveTo(const int &y, const int &x);
        void moveY(const int &y);
        void moveX(const int &x);
        const int getY() const;
        const int getX() const;
    };

private:
    int maxY, maxX;
    std::unique_ptr<Cursor> cursor;
    std::unique_ptr<KeyListener> keyListener;
    std::unique_ptr<ColorManager> colorManager;
    std::unique_ptr<Parser> parser;
    std::unique_ptr<Store> store;
public:
    Window(std::unique_ptr<KeyListener> keyListener,
        std::unique_ptr<ColorManager> colorManager, std::unique_ptr<Parser> parser);
    ~Window();

    void init(const std::string &fileName);
    void render();
    void refreshCursor();
    void showStatus(const std::string &status);

    const int getMaxY() const;
    const int getMaxX() const;
    Cursor* getCursor();
    Store* getStore();
    KeyListener* getKeyListener();
    ColorManager* getColorManager();
};

#endif
