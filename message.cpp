#include "message.h"

Message::Message(std::ostream& stream)
: Stream(stream)
{}

Message::~Message() {
    Buffer << std::endl;
    Stream << Buffer.str() << std::flush;
}
