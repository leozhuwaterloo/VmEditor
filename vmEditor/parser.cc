#include "parser.h"
#include <vector>
#include <map>
#include <fstream>
#include "store.h"

std::unique_ptr<Store> Parser::parse(const std::string &fileName) {
    this->fileName = fileName;
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
        std::map<int, std::vector<std::string>> strs;
        int counter = 0;
        while(std::getline(f, line)){
            strs[counter++].push_back(line);
        }
        return std::make_unique<Store>(strs);
    }
}
