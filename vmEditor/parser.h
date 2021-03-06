#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <memory>

class Store;

class Parser{
private:
    std::string fileName;
    virtual std::unique_ptr<Store> runParse(const std::string &fileName) = 0;
protected:
    int lCount, cCount;
    bool newFile;
public:
    Parser() = default;
    virtual ~Parser() = default;
    std::unique_ptr<Store> parse(const std::string &fileName);
    std::string getFileName() const;
    void setFileName(const std::string &fileName);
    bool isNewFile() const;
    int getlCount() const;
    int getcCount() const;
};

class DefaultParser: public Parser{
private:
    virtual std::unique_ptr<Store> runParse(const std::string &fileName) override;
};


#endif
