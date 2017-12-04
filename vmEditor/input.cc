#include "command.h"
#include "window.h"
#include "keylistener.h"
#include "event.h"
#include "store.h"
#include "utils.h"
#include <locale>
#include <ncurses.h>
#include <stack>
#include <regex>
#include "parser.h"
#include "saver.h"

/*
https://www.fprintf.net/vimCheatSheet.html
*/

/*
cc c[any motion] dd d[any motion] p s yy y[any motion]
P S %
^b ^d ^u
*/

std::vector<std::unique_ptr<Event>> CommandDot::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events;
    if(!w->getKeyListener()->getEventHistory().empty()){
        return w->getKeyListener()->getEventHistory().top()->reExecute(w);
    }
    return events;
}
void CommandDot::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
