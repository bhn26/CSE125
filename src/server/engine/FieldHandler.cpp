#include "FieldHandler.h"

#include "FieldObject.h"
#include "network/GameData.h"

FieldHandler::~FieldHandler()
{
    // delete fields???
    for (auto* field : m_activeFields)
    {
        delete field;
    }
    m_activeFields.clear();
}

void FieldHandler::HandleFields()
{
    auto it = m_activeFields.begin();

    // Check through list of active fields
    while (it != m_activeFields.end())
    {
        // if field is done, remove from queue
        if ((*it)->handleField())
        {
            auto tempIt = (*it);
            it = m_activeFields.erase(it);
            delete tempIt;
        }
        else
        {
            it++;
        }
    }
}

void FieldHandler::AddField(FieldObject* newField)
{
    m_activeFields.push_back(newField);
}