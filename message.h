#include <ostream>
#include <sstream>

class Message {
public:
    explicit Message(std::ostream& stream);

    template<class T>
    inline Message& operator<<(T t) {
        Buffer << t;
        return *this;
    }

    virtual ~Message();

private:
    std::ostream& Stream;
    std::stringstream Buffer;
};