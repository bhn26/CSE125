#pragma once

#include <vector>
#include <memory>

class FieldObject;

class FieldHandler
{
private:
	std::vector<FieldObject*>* activeFields;
	static FieldHandler *fh;
	FieldHandler();

public:

	~FieldHandler();

	static FieldHandler *instance();

	void HandleFields();

	void AddField(FieldObject* newField);
};