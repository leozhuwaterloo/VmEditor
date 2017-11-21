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


std::string Parser::getFileType() const {
    std::string fileName = getFileName();
    size_t firstDot = fileName.find(".");
    if(firstDot == std::string::npos){
        return "";
    }else{
        return fileName.substr(firstDot+1, fileName.length() - firstDot - 1);
    }
}


std::unique_ptr<Store> DefaultParser::runParse(const std::string &fileName){
    if(fileName.empty()){
        return std::make_unique<Store>();
    }else {
        std::fstream f {fileName};
        std::string line;
        std::list<std::string> strs;
        while(std::getline(f, line)){
            strs.push_back(line);
        }
        return std::make_unique<Store>(strs);
    }
}
