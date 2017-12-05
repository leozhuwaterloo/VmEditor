#include "parser.h"
#include <list>
#include <fstream>
#include "store.h"


std::unique_ptr<Store> Parser::parse(const std::string &fileName) {
    this->fileName = fileName;
    if(!fileName.empty()){
        std::system(("dos2unix " + fileName + " >&- 2>&-").c_str());
        std::fstream f {fileName};
        newFile = !(f.good());
    }else{
        newFile = true;
    }
    return runParse(fileName);
}

std::string Parser::getFileName() const { return fileName; }
void Parser::setFileName(const std::string &fileName){ this->fileName = fileName; }
bool Parser::isNewFile() const { return newFile; }
int Parser::getlCount() const { return lCount; }
int Parser::getcCount() const { return cCount; }

std::unique_ptr<Store> DefaultParser::runParse(const std::string &fileName){
    if(newFile){
        std::list<std::string> strs;
        strs.push_back("");
        return std::make_unique<Store>(strs);
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
