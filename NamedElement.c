#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Visitor.h"
#include "NamedElement.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

NamedElement* new_NamedElement()
{
	NamedElement* pObj = NULL;
	/* Allocating memory */
	pObj = (NamedElement*)malloc(sizeof(NamedElement));

	if (pObj == NULL)
	{
		return NULL;
	}

	/* pointing to itself as we are creating base class object*/
	pObj->pDerivedObj = pObj;
	pObj->name = NULL;

	pObj->InternalGetKey = NamedElement_InternalGetKey;
	pObj->MetaClassName = NamedElement_MetaClassName;
	pObj->Delete = delete_NamedElement;
	pObj->VisitAttributes = NamedElement_VisitAttributes;
	pObj->VisitPathAttributes = NamedElement_VisitPathAttributes;
	pObj->VisitReferences = NamedElement_VisitAttributes;
	pObj->VisitPathReferences = NamedElement_VisitPathAttributes;
	pObj->FindByPath = NamedElement_FindByPath;
	
	return pObj;
}

char* NamedElement_InternalGetKey(NamedElement* const this)
{
	return this->name;
}

char* NamedElement_MetaClassName(NamedElement* const this)
{
	char *name;

	name = malloc(sizeof(char) * (strlen("NamedElement")) + 1);
	if(name != NULL)
		strcpy(name, "NamedElement");
	else
		return NULL;
	
	return name;
}

void delete_NamedElement(NamedElement* const this)
{
	if(this != NULL)
	{
		free(this->name);
		free(this);
	}
}

void NamedElement_VisitAttributes(void *const this, char *parent, Visitor *visitor, bool recursive)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	if(recursive)
	{
		char* cClass = NULL;
		cClass = malloc(sizeof(char) * (strlen("org.kevoree.") + strlen(((NamedElement*)this)->MetaClassName((NamedElement*)this))) + 1);
		sprintf(cClass, "org.kevoree.%s", ((NamedElement*)this)->MetaClassName((NamedElement*)this));
		sprintf(path,"eClass");
		visitor->action(path, STRING, cClass);
		visitor->action(NULL, COLON, NULL);
		str_free(cClass);

		sprintf(path, "name", parent);
		visitor->action(path, STRING, ((NamedElement*)(this))->name);
		visitor->action(NULL, COLON, NULL);
	}
	else
	{
		visitor->action("", STRING, ((NamedElement*)(this))->name);
		visitor->action(NULL, COLON, NULL);
	}
}

void NamedElement_VisitPathAttributes(void *const this, char *parent, Visitor *visitor, bool recursive)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	if(recursive)
	{
		/*char* cClass = NULL;
		sprintf(path,"%s\\cClass", parent);
		cClass = ((NamedElement*)this)->MetaClassName((NamedElement*)this);
		visitor->action(path, STRING, cClass);
		free(cClass);*/

		sprintf(path, "%s\\name", parent);
		visitor->action(path, STRING, ((NamedElement*)(this))->name);
	}
	else
	{
		sprintf(path, "%s\\name", parent);
		visitor->action(path, STRING, ((NamedElement*)(this))->name);
	}
}

void* NamedElement_FindByPath(char* attribute, NamedElement* const this)
{
	if(!strcmp("name",attribute))
	{
		return this->name;
	}
	else
	{
		PRINTF("Wrong attribute\n");
		return NULL;
	}
}
