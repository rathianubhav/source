#include <inbuilts.h>
#include <node.h>

class io : public InBuilt {
public:

    class println : public InBuilt {
    virtual Value run(std::vector<Value> args) override {
        
    }
    };

    virtual Value run(std::vector<Value> args) override {
        return Value("I am running");
    }

};

extern "C" InBuilt* sys_create() {
    return new io;
}

extern "C" void sys_destroy(InBuilt* s) {
    delete s;
}