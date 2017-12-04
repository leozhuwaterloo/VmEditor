#ifndef KEYLISTENER_H
#define KEYLISTENER_H
#include <map>
#include <vector>
#include <stack>
#include <memory>

class Command;
class Window;
class Event;

class KeyListener{
private:
    bool searched, recording, finded;
    int targetReg;
    std::stack<int> trigger;
    std::map<int, std::vector<int>> macros;
    std::map<std::vector<int>, std::unique_ptr<Command>> commands;
    std::vector<int> currKeys;
    std::stack<std::unique_ptr<Event>> eventHistory;
    std::pair<std::string, int> latestSearch;
    std::pair<char, int> latestFind;
public:
    KeyListener();
    ~KeyListener() = default;
    void init(Window *window);
    void addCommand(std::unique_ptr<Command> command);
    std::pair<std::string, int> &getLatestSearch();
    bool getSearched() const;
    void setLatestSearch(const std::string &target, const int &direction);
    std::pair<char, int> &getLatestFind();
    bool getFinded() const;
    void setLatestFind(const char &target, const int &direction);
    std::stack<std::unique_ptr<Event>> &getEventHistory();
    bool isRecording() const;
    void startRecording(const int &targetReg);
    void stopRecording();
    std::map<int, std::vector<int>> &getMacros();
    std::string getRecordingStatus() const;
    void pushStream(const std::vector<int> &stream);
    void stream(const std::vector<int> &stream, Window *window);
    int get();
};

#endif
