#include "Action.h"

Action::Action(const std::string& name, const std::string& type)
    :   m_name(name),
        m_type(type)
{
}

Action::Action()
{
}

const std::string& Action::getName() const
{
    return m_name;
}

const std::string& Action::getType() const
{
    return m_type;
}
