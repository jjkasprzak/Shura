#include "nlohmann/json.hpp"

class SharedComponent{
public:

SharedComponent() : id(generateID()) {}
int getID() const 
{
    return id;
}

virtual nlohmann::json push() const = 0;
virtual void pull(const nlohmann::json& data) = 0;

private:
    const int id;

    static int generateID()
    {
        static int counter = 0;
        return counter++;
    }
};