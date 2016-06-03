
#include "FieldHandler.h"
#include "FieldObject.h"
#include "../../network/GameData.h"
#include "../ServerGame.h"


FieldHandler* FieldHandler::fh = nullptr;

FieldHandler* FieldHandler::instance()
{
	if (!fh)
	{
		fh = new FieldHandler();
	}
	return fh;
}

FieldHandler::FieldHandler()
{
	activeFields = new std::vector<FieldObject*>;
}

FieldHandler::~FieldHandler() {}

void FieldHandler::HandleFields()
{
	auto it = activeFields->begin();

	// Check through list of active fields
	while (it != activeFields->end())
	{
		// if field is done, remove from queue
		if ((*it)->handleField())
		{
			auto tempIt = (*it);
            ServerGame::instance()->sendRemovePacket(ClassId::FIELD, (*it)->oid);
			it = activeFields->erase(it);
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
	activeFields->push_back(newField);
}