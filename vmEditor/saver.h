#ifndef SAVER_H
#define SAVER_H

class Parser;
class Store;

class Saver{
protected:
    bool modified;
    Parser *parser;
    Store *store;
public:
    Saver() = default;
    virtual ~Saver() = default;
    bool save();
    virtual bool runSave() = 0;
    void init(Parser *parser, Store *store);
    bool getModified() const;
    void setModified(const bool &modified);
};

class DefaultSaver: public Saver{
private:
    virtual bool runSave() override;
};


#endif
