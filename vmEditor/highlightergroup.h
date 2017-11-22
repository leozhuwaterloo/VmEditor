#ifndef HIGHLIGHTERGROUP_H
#define HIGHLIGHTERGROUP_H
#include <regex>
#include <vector>
#include <memory>
#include <initializer_list>

class Highlighter;

class HighlighterGroup{
private:
    std::vector<std::regex> regs;
    std::vector<std::unique_ptr<Highlighter>> highlighters;
public:
    HighlighterGroup(std::initializer_list<std::regex> regs);
    ~HighlighterGroup() = default;
    void addHighlighter(std::unique_ptr<Highlighter> highlighter);
    bool match(const std::string &str) const;
    void apply(const std::string &str, std::map<int, std::map<int, std::pair<int, int>>> &highlight) const;
};

#endif
