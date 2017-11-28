#include "parser.h"
#include <list>
#include <fstream>
#include "store.h"

std::unique_ptr<Store> Parser::parse(const std::string &fileName) {
    this->fileName = fileName;
    std::system(("dos2unix " + fileName + " >&- 2>&-").c_str());
    return runParse(fileName);
}

std::string Parser::getFileName() const { return fileName; }
int Parser::getlCount() const { return lCount; }
int Parser::getcCount() const { return cCount; }

std::unique_ptr<Store> DefaultParser::runParse(const std::string &fileName){
    if(fileName.empty()){
        return std::make_unique<Store>();
    }else {
        std::fstream f {fileName};
        std::string line;
        std::list<std::string> strs;
        while(std::getline(f, line)){
            strs.push_back(line);
            ++lCount;
            cCount += line.length() + 1;
        }
        return std::make_unique<Store>(strs);
    }
}
