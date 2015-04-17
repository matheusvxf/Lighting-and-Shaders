#include "IUpdatable.h"

std::list<IUpdatable*> IUpdatable::list;

IUpdatable::IUpdatable()
{
    list.push_back(this);
}


IUpdatable::~IUpdatable()
{
    list.remove(this);
}
