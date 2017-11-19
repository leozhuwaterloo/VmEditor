#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <memory>

class KeyListener;
class ColorManager;

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
    std::string fileName, content;
    int maxY, maxX;
    std::unique_ptr<KeyListener> keyListener;
    std::unique_ptr<Cursor> cursor;
    std::unique_ptr<ColorManager> colorManager;
public:
    Window(std::string fileName, std::unique_ptr<KeyListener> keyListener, std::unique_ptr<ColorManager> colorManager);
    ~Window();

    void init();
    void render();
    void refreshCursor();
    void showStatus(const std::string &status);

    const int getMaxY() const;
    const int getMaxX() const;
    Cursor* getCursor();
    KeyListener* getKeyListener();
    ColorManager* getColorManager();
};

#endif
