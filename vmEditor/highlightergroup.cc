#include "highlightergroup.h"
#include "highlighter.h"

HighlighterGroup::HighlighterGroup(std::initializer_list<std::regex> regs):regs{regs}{}
void HighlighterGroup::addHighlighter(std::unique_ptr<Highlighter> highlighter){ highlighters.push_back(std::move(highlighter));}
bool HighlighterGroup::match(const std::string &str) const {
    for(auto &reg: regs){
        if(std::regex_match(str, reg)) return true;
    }
    return false;
}
void HighlighterGroup::apply(const std::string &str, std::map<int, std::map<int, std::pair<int, int>>> &highlight) const{
    for(auto &highlighter: highlighters) highlighter->apply(str, highlight);
}
