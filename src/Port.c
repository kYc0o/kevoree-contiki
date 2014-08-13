#include "Port.h"

Port* new_Port()
{
	Port* pObj = NULL;
	/* Allocating memory */
	pObj = (Port*)malloc(sizeof(Port));

	if (pObj == NULL)
	{
		return NULL;
	}

	pObj->generated_KMF_ID = malloc(sizeof(char) * 8 + 1);
	rand_str(pObj->generated_KMF_ID, 8);
	
	pObj->bindings = NULL;
	pObj->portTypeRef = NULL;
	
	pObj->AddBindings = Port_AddBindings;
	pObj->AddPortTypeRef = Port_AddPortTypeRef;
	pObj->RemoveBindings = Port_RemoveBindings;
	pObj->RemovePortTypeRef = Port_RemovePortTypeRef;

	pObj->InternalGetKey = Port_InternalGetKey;
	pObj->MetaClassName = Port_MetaClassName;
	pObj->Delete = delete_Port;
	pObj->VisitAttributes = Port_VisitAttributes;
	pObj->VisitReferences = Port_VisitReferences;
	pObj->FindByPath = Port_FindByPath;
	
	return pObj;
}

void delete_Port(void* this)
{
	if((Port*)this != NULL)
	{
		free(((Port*)this)->generated_KMF_ID);
		hashmap_free(((Port*)this)->bindings);
		free(((Port*)this)->portTypeRef);
	}
}

char* Port_InternalGetKey(Port* const this)
{
	char* internalKey;

	if (this == NULL)
		return NULL;

	internalKey = malloc(sizeof(char) * (strlen(this->generated_KMF_ID)));

	if (internalKey == NULL)
		return NULL;

	strcpy(internalKey, this->generated_KMF_ID);

	return internalKey;
}

char* Port_MetaClassName(Port* const this)
{
	char* name;

	name = malloc(sizeof(char) * (strlen("Port") + 1));
	strcpy(name, "Port");
	
	return name;
}

MBinding* Port_FindBindingsByID(Port* const this, char* id)
{
	MBinding* value;

	if(this->bindings != NULL)
	{
		if(hashmap_get(this->bindings, id, (void**)(&value)) == MAP_OK)
			return value;
		else
			return NULL;
	}
	else
	{
		return NULL;
	}
}

void Port_AddBindings(Port* const this, MBinding* ptr)
{
	MBinding* container = (MBinding*)ptr;

	if(container->InternalGetKey(container) == NULL)
	{
		printf("The MBinding cannot be added in Port because the key is not defined\n");
	}
	else
	{
		if(this->bindings == NULL)
		{
			this->bindings = hashmap_new();
		}

		if(hashmap_get(this->bindings, container->InternalGetKey(container), (void**)(&container)) == MAP_MISSING);
		{
			container = (MBinding*)ptr;
			hashmap_put(this->bindings, container->InternalGetKey(container), ptr);
		}
	}
}

void Port_AddPortTypeRef(Port* const this, PortTypeRef* ptr)
{
	this->portTypeRef = ptr;
}

void Port_RemoveBindings(Port* const this, MBinding* ptr)
{
	MBinding* container = (MBinding*)ptr;

	if(container->InternalGetKey(container) == NULL)
	{
		printf("The MBinding cannot be removed in Port because the key is not defined\n");
	}
	else
	{
		hashmap_remove(this->bindings, container->InternalGetKey(container));
	}
}

void Port_RemovePortTypeRef(Port* const this, PortTypeRef* ptr)
{
	free(ptr);
	this->portTypeRef = NULL;
}

void Port_VisitAttributes(Port* const this, char* parent, Visitor* visitor)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	sprintf(path, "%s\\generated_KMF_ID", parent);
	visitor->action(path, STRING, this->generated_KMF_ID);
}

void Port_VisitReferences(Port* const this, char* parent, Visitor* visitor)
{
	int i;
	char path[256];
	memset(&path[0], 0, sizeof(path));
	
	hashmap_map* m = NULL;
	
	if((m = (hashmap_map*)this->bindings) != NULL)
	{
		for(i = 0; i< m->table_size; i++)
		{
			if(m->data[i].in_use != 0)
			{
				any_t data = (any_t) (m->data[i].data);
				MBinding* n = data;
				sprintf(path, "%s/bindings[%s]", parent, n->InternalGetKey(n));
				n->VisitAttributes(n, path, visitor);
				n->VisitReferences(n, path, visitor);
			}
		}
	}
	
	if(this->portTypeRef != NULL)
	{
		sprintf(path, "%s/portTypeRef[%s]", parent, this->portTypeRef->InternalGetKey(this->portTypeRef));
		this->portTypeRef->VisitAttributes(this->portTypeRef, path, visitor);
		this->portTypeRef->VisitReferences(this->portTypeRef, path, visitor);
	}
}
void* Port_FindByPath(char* attribute, Port* const this)
{
	/* Local attributes */
	if(!strcmp("generated_KMF_ID", attribute))
	{
		return this->generated_KMF_ID;
	}
	/* Local references */
	else
	{
		char* nextAttribute = NULL;
		char* path = strdup(attribute);
		char* pch;

		if(indexOf(path,"/") != -1)
		{
			pch = strtok (path,"/");
			
			if(strchr(attribute,'\\') != NULL)
			{
				nextAttribute = strtok(NULL, "\\");
				sprintf(nextAttribute, "%s\\%s", nextAttribute, strtok(NULL, "\\"));
			}
			else
			{
				nextAttribute = strtok(NULL, "\\");
			}
		}
		else
		{
			pch = path;
			nextAttribute = strtok(pch, "\\");
			nextAttribute = strtok(NULL, "\\");
		}

		int i = indexOf(pch,"[") + 2;
		int y = lastIndexOf(pch,"]") - i + 1;

		char* relationName = (char*)Substring(pch, 0, i - 2);
		char* queryID = (char*)Substring(pch, i, y);
	  
		if(!strcmp("bindings", relationName))
		{
			if(nextAttribute == NULL)
			{
				
				return this->FindBindingsByID(this, queryID);
			}
			else
			{
				MBinding* binding = this->FindBindingsByID(this, queryID);
				if(binding != NULL)
					return binding->FindByPath(nextAttribute, binding);
				else
					return NULL;
			}
		}
		else
		{
			printf("Wrong attribute or reference\n");
			return NULL;
		}
	}
}