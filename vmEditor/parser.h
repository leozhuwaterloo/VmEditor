#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <memory>

class Store;

class Parser{
private:
    std::string fileName;
    virtual std::unique_ptr<Store> runParse(const std::string &fileName) = 0;
public:
    Parser() = default;
    virtual ~Parser() = default;
    std::unique_ptr<Store> parse(const std::string &fileName);
    std::string getFileName() const;
    std::string getFileType() const;
};

class DefaultParser: public Parser{
private:
    virtual std::unique_ptr<Store> runParse(const std::string &fileName) override;
};


#endif
