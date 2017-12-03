#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <memory>
#include <list>

class KeyListener;
class ColorManager;
class Parser;
class Store;

enum State { STATE_INSERT, STATE_NORMAL };
enum CursorState { STATE_STATUS, STATE_EDIT };

class Window{
    class Cursor{
    private:
        int y, x;
        int statusY, statusX;
        int nLine, nChar;
        int xLoss;
        int preX;
        Window *window;
        CursorState cursorState;
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
        void erase();
        const int getY() const;
        const int getX() const;
        std::list<std::string>::iterator &getItLst();
        std::string::iterator &getItStr();
        const int getNLine() const;
        const int getNChar() const;
        const int currChar() const;
        bool isAtEmptyLine() const;
        bool isAtBegin() const;
        bool isAtEnd() const;
        bool isAtLineBegin() const;
        bool isAtLineEnd(const int &offset) const;
        bool isAtLineEnd() const;
        void setState(CursorState cursorState);
    };

private:
    int maxY, maxX;
    std::unique_ptr<Cursor> cursor;
    std::unique_ptr<KeyListener> keyListener;
    std::unique_ptr<ColorManager> colorManager;
    std::unique_ptr<Parser> parser;
    std::unique_ptr<Store> store;
    std::string status;
    State state;
public:
    Window(std::unique_ptr<KeyListener> keyListener,
        std::unique_ptr<ColorManager> colorManager, std::unique_ptr<Parser> parser);
    ~Window();

    void init(const std::string &fileName);
    void render();
    void resize();
    void refreshCursor();
    void showStatus();
    void showStatus(const std::string &status);

    const int getMaxY() const;
    const int getMaxX() const;
    const int getStateLineEnd() const;
    void setState(State state);
    Cursor* getCursor();
    Store* getStore();
    std::unique_ptr<Store> &getUniqueStore();
    KeyListener* getKeyListener();
    ColorManager* getColorManager();
};

#endif
