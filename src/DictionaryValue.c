#include "DictionaryValue.h"

DictionaryValue* new_DictionaryValue(void)
{
	DictionaryValue* pObj = NULL;
	/* Allocating memory */
	pObj = (DictionaryValue*)malloc(sizeof(DictionaryValue));

	if (pObj == NULL)
	{
		return NULL;
	}
	
	pObj->name = NULL;
	pObj->value = NULL;

	pObj->InternalGetKey = DictionaryValue_InternalGetKey;
	pObj->MetaClassName = DictionaryValue_MetaClassName;
	pObj->Delete = delete_DictionaryValue;
	pObj->VisitAttributes = DictionaryValue_VisitAttributes;
	pObj->FindByPath = DictionaryValue_FindByPath;
	
	return pObj;
}

void delete_DictionaryValue(DictionaryValue* const this)
{
	if(this != NULL)
	{
		free(this->name);
		free(this->value);
		free(this);
	}
}

char* DictionaryValue_InternalGetKey(DictionaryValue* const this)
{
	char* internalKey;

	if (this == NULL)
		return NULL;

	internalKey = malloc(sizeof(char) * (strlen(this->name)));

	if (internalKey == NULL)
		return NULL;

	strcpy(internalKey, this->name);

	return internalKey;
}

char* DictionaryValue_MetaClassName(DictionaryValue* const this)
{
	char* name;

	name = malloc(sizeof(char) * (strlen("DictionaryValue") + 1));
	strcpy(name, "DictionaryValue");
	
	return name;
}

void DictionaryValue_VisitAttributes(void* const this, char* parent, Visitor* visitor)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	sprintf(path, "%s\\name", parent);
	visitor->action(path, STRING, ((DictionaryValue*)(this))->name);
	
	sprintf(path, "%s\\value", parent);
	visitor->action(path, STRING, ((DictionaryValue*)(this))->value);
}

void* DictionaryValue_FindByPath(char* attribute, DictionaryValue* const this)
{
	/* Local attributes */
	if(!strcmp("name", attribute))
	{
		return this->name;
	}
	else if(!strcmp("value", attribute))
	{
		return this->value;
	}
	/* There is no local references */
	else
	{
		printf("Wrong attribute or reference\n");
		return NULL;
	}
}