#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <memory>
#include <list>

class KeyListener;
class ColorManager;
class Parser;
class Store;

class Window{
    class Cursor{
    private:
        int y;
        int x;
        int xLoss;
        int preX;
        Window *window;
        std::list<std::string>::iterator itLst;
        std::string::iterator itStr;
    public:
        Cursor(int y, int x, Window *window);
        ~Cursor() = default;
        void init(Store *store);
        void adjust();
        void moveTo(const int &y, const int &x);
        void moveY(const int &y);
        void moveX(const int &x);
        void moveOne(const int &n);
        void moveLineBegin();
        void moveLineEnd();
        void moveLineBeginNonWs();
        void insert(char c);
        void moveOnePastEnd();
        const int getY() const;
        const int getX() const;
        const int currChar() const;
        bool isAtEmptyLine() const;
        bool isAtBegin() const;
        bool isAtEnd() const;
        bool isAtLineBegin() const;
        bool isAtLineEnd() const;
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
    void resize();
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
