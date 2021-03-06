#include "saver.h"
#include <fstream>
#include "parser.h"
#include "store.h"

bool Saver::save(){
    if(parser->getFileName().empty()) return false;
    else{ if(runSave()){ modified = false; return true; } else return false; }
}
void Saver::init(Parser *parser, Store *store){
    this->parser = parser;
    this->store = store;
}
bool Saver::getModified() const{ return modified; }
void Saver::setModified(const bool &modified){ this->modified = modified; }



bool DefaultSaver::runSave(){
    std::fstream f {parser->getFileName(), std::ios_base::out};
    for(auto &it : store->getStrs()){
        f << it << std::endl;
    }
    f.close();
    return true;
}
