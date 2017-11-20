#include "parser.h"
#include <list>
#include <fstream>
#include "store.h"

std::unique_ptr<Store> Parser::parse(const std::string &fileName) {
    this->fileName = fileName;
    return runParse(fileName);
}

std::string Parser::getFileName() const { return fileName; }


std::unique_ptr<Store> DefaultParser::runParse(const std::string &fileName){
    if(fileName.empty()){
        return std::make_unique<Store>();
    }else {
        std::fstream f {fileName};
        std::string line;
        std::list<std::string> lst;
        while(std::getline(f, line)){
            lst.push_back(line);
        }
        return std::make_unique<Store>(lst);
    }
}
