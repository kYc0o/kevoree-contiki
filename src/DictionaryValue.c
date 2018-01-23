#include "Visitor.h"
#include "Dictionary.h"
#include "DictionaryValue.h"

DictionaryValue* new_DictionaryValue(void)
{
	DictionaryValue* pObj = NULL;
	/* Allocating memory */
	pObj = (DictionaryValue*)my_malloc(sizeof(DictionaryValue));

	if (pObj == NULL)
	{
		return NULL;
	}
	
	pObj->name = NULL;
	pObj->value = NULL;
	pObj->eContainer = NULL;

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
		free(this->eContainer);
		free(this);
		/*this = NULL;*/
	}
}

char* DictionaryValue_InternalGetKey(DictionaryValue* const this)
{
	return this->name;
}

char* DictionaryValue_MetaClassName(DictionaryValue* const this)
{
	char *name;

	name = my_malloc(sizeof(char) * (strlen("DictionaryValue")) + 1);
	if(name != NULL)
		strcpy(name, "DictionaryValue");
	else
		return NULL;
	
	return name;
}

void DictionaryValue_VisitAttributes(void* const this, char* parent, Visitor* visitor)
{
	char path[256];
	char *cClass = NULL;
	memset(&path[0], 0, sizeof(path));

	/*sprintf(path,"%s\\cClass", parent);*/
	cClass = my_malloc(sizeof(char) * (strlen("org.kevoree.") + strlen(((DictionaryValue*)this)->MetaClassName((DictionaryValue*)this))) + 1);
	sprintf(cClass, "org.kevoree.%s", ((DictionaryValue*)this)->MetaClassName((DictionaryValue*)this));
	sprintf(path,"eClass");
	/*cClass = ((DictionaryValue*)this)->MetaClassName((DictionaryValue*)this);*/
	visitor->action(path, STRING, cClass);
	visitor->action(NULL, COLON, NULL);
	/*free(cClass);*/
	str_free(cClass);

	/*sprintf(path, "%s\\name", parent);*/
	sprintf(path, "name");
	visitor->action(path, STRING, ((DictionaryValue*)(this))->name);
	visitor->action(NULL, COLON, NULL);
	
	/*sprintf(path, "%s\\value", parent);*/
	sprintf(path, "value");
	visitor->action(path, STRING, ((DictionaryValue*)(this))->value);
	visitor->action(NULL, RETURN, NULL);
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