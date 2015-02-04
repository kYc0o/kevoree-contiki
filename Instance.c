#include <stdlib.h>
#include "NamedElement.h"
#include "TypeDefinition.h"
#include "Dictionary.h"
#include "FragmentDictionary.h"
#include "Visitor.h"
#include "Instance.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

NamedElement* newPoly_Instance()
{
	Instance* pInstanceObj = NULL;
	NamedElement* pObj = new_NamedElement();

	/* Allocating memory */
	pInstanceObj = (Instance*)my_malloc(sizeof(Instance));

	if (pInstanceObj == NULL)
	{
		pObj->Delete(pObj);
		return NULL;
	}

	pObj->pDerivedObj = pInstanceObj; /* Pointing to derived object */

	pInstanceObj->path = NULL;
	pInstanceObj->metaData = NULL;
	pInstanceObj->started = -1;
	pInstanceObj->typeDefinition = NULL;
	pInstanceObj->dictionary = NULL;
	pInstanceObj->fragmentDictionary = NULL;

	pInstanceObj->FindFragmentDictionaryByID = Instance_FindFragmentDictionaryByID;
	pInstanceObj->AddTypeDefinition = Instance_AddTypeDefinition;
	pInstanceObj->AddDictionary = Instance_AddDictionary;
	pInstanceObj->AddFragmentDictionary = Instance_AddFragmentDictionary;
	pInstanceObj->RemoveTypeDefinition = Instance_RemoveTypeDefinition;
	pInstanceObj->RemoveDictionary = Instance_RemoveDictionary;
	pInstanceObj->RemoveFragmentDictionary = Instance_RemoveFragmentDictionary;

	pObj->MetaClassName = Instance_MetaClassName;
	pObj->InternalGetKey = Instance_InternalGetKey;
	pObj->Delete = deletePoly_Instance;
	pObj->VisitAttributes = Instance_VisitAttributes;
	pObj->VisitPathAttributes = Instance_VisitPathAttributes;
	pObj->VisitReferences = Instance_VisitReferences;
	pObj->VisitPathReferences = Instance_VisitPathReferences;
	pObj->FindByPath = Instance_FindByPath;

	return pObj;
}

Instance* new_Instance()
{
	Instance* pInstanceObj = NULL;
	NamedElement* pObj = new_NamedElement();

	/* Allocating memory */
	pInstanceObj = (Instance*)my_malloc(sizeof(Instance));

	if (pInstanceObj == NULL)
	{
		return NULL;
	}

	/*pObj->pDerivedObj = pInstanceObj; Pointing to derived object */
	pInstanceObj->super = pObj;

	pInstanceObj->path = NULL;
	pInstanceObj->metaData = NULL;
	pInstanceObj->started = -1;
	pInstanceObj->typeDefinition = NULL;
	pInstanceObj->dictionary = NULL;
	pInstanceObj->fragmentDictionary = NULL;

	pInstanceObj->FindFragmentDictionaryByID = Instance_FindFragmentDictionaryByID;
	pInstanceObj->AddTypeDefinition = Instance_AddTypeDefinition;
	pInstanceObj->AddDictionary = Instance_AddDictionary;
	pInstanceObj->AddFragmentDictionary = Instance_AddFragmentDictionary;
	pInstanceObj->RemoveTypeDefinition = Instance_RemoveTypeDefinition;
	pInstanceObj->RemoveDictionary = Instance_RemoveDictionary;
	pInstanceObj->RemoveFragmentDictionary = Instance_RemoveFragmentDictionary;

	pInstanceObj->MetaClassName = Instance_MetaClassName;
	pObj->MetaClassName = pInstanceObj->MetaClassName;
	pInstanceObj->InternalGetKey = Instance_InternalGetKey;
	pInstanceObj->Delete = delete_Instance;
	pInstanceObj->VisitAttributes = Instance_VisitAttributes;
	pInstanceObj->VisitPathAttributes = Instance_VisitPathAttributes;
	pInstanceObj->VisitReferences = Instance_VisitReferences;
	pInstanceObj->VisitPathReferences = Instance_VisitPathReferences;
	pInstanceObj->FindByPath = Instance_FindByPath;

	return pInstanceObj;
}

FragmentDictionary* Instance_FindFragmentDictionaryByID(Instance* const this, char* id)
{
	FragmentDictionary* value = NULL;

	if(this->fragmentDictionary != NULL)
	{
		if(hashmap_get(this->fragmentDictionary, id, (void**)(&value)) == MAP_OK)
			return value;
		else
			return NULL;
	}
	else
	{
		return NULL;
	}
}

void Instance_AddTypeDefinition(Instance* this, TypeDefinition* ptr)
{
	this->typeDefinition = ptr;
}

void Instance_AddDictionary(Instance* const this, Dictionary* ptr)
{
	this->dictionary = ptr;
	ptr->eContainer = my_malloc(sizeof(char) * (strlen("instance[]") + strlen(this->InternalGetKey(this))) + 1);
	sprintf(ptr->eContainer, "instance[%s]", this->InternalGetKey(this));
}

void Instance_AddFragmentDictionary(Instance* const this, FragmentDictionary* ptr)
{
	FragmentDictionary* container = NULL;

	char *internalKey = ptr->InternalGetKey(ptr);

	if(internalKey == NULL)
	{
		PRINTF("The FragmentDictionary cannot be added in Instance because the key is not defined\n");
	}
	else
	{
		if(this->fragmentDictionary == NULL)
		{
			this->fragmentDictionary = hashmap_new();
		}
		if(hashmap_get(this->fragmentDictionary, internalKey, (void**)(&container)) == MAP_MISSING)
		{
			/*container = (FragmentDictionary*)ptr;*/
			if(hashmap_put(this->fragmentDictionary, internalKey, ptr) == MAP_OK)
			{
				ptr->eContainer = my_malloc(sizeof(char) * (strlen("instance[]") + strlen(this->InternalGetKey(this))) + 1);
				sprintf(ptr->eContainer, "instance[%s]", this->InternalGetKey(this));
			}
		}
	}
}

void Instance_RemoveTypeDefinition(Instance* const this, TypeDefinition* ptr)
{
	free(ptr);
	this->typeDefinition = NULL;
}

void Instance_RemoveDictionary(Instance* const this, Dictionary* ptr)
{
	ptr->eContainer = NULL;
	free(ptr);
	this->dictionary = NULL;
}

void Instance_RemoveFragmentDictionary(Instance* const this, FragmentDictionary* ptr)
{
	char *internalKey = ptr->InternalGetKey(ptr);

	if(internalKey == NULL)
	{
		PRINTF("The FragmentDictionary cannot be removed in Instance because the key is not defined\n");
	}
	else
	{
		hashmap_remove(this->fragmentDictionary, internalKey);
	}
}

char* Instance_InternalGetKey(Instance* const this)
{
	return this->super->InternalGetKey(this->super);
}

char* Instance_MetaClassName(Instance* const this)
{
	char *name;

	name = my_malloc(sizeof(char) * (strlen("Instance")) + 1);
	if(name != NULL)
		strcpy(name, "Instance");
	else
		return NULL;

	return name;
}

void deletePoly_Instance(NamedElement* const this)
{
	Instance* pInstanceObj;
	pInstanceObj = this->pDerivedObj;
	/*destroy derived obj*/
	free(pInstanceObj->metaData);
	free(pInstanceObj->typeDefinition);
	free(pInstanceObj->dictionary);
	hashmap_free(pInstanceObj->fragmentDictionary);
	free(pInstanceObj);
	/*destroy base Obj*/
	delete_NamedElement(this);
}

void delete_Instance(Instance* const this)
{
	/* destroy base object */
	delete_NamedElement(this->super);
	/* destroy data memebers */
	free(this->metaData);
	free(this->typeDefinition);
	free(this->dictionary);
	hashmap_free(this->fragmentDictionary);
	free(this);

}

void Instance_VisitAttributes(void *const this, char *parent, Visitor *visitor, bool recursive)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	/* NamedElement attributes */
	NamedElement_VisitAttributes(((Instance*)(this))->super, parent, visitor, recursive);

	/* Local attributes */
	if(recursive)
	{
		sprintf(path,"metaData");
		visitor->action(path, STRING, ((Instance*)(this))->metaData);
		visitor->action(NULL, COLON, NULL);

		sprintf(path,"started", parent);
		visitor->action(path, BOOL, (void*)((Instance*)(this))->started);
		visitor->action(NULL, COLON, NULL);
	}
}

void Instance_VisitPathAttributes(void *const this, char *parent, Visitor *visitor, bool recursive)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	/* NamedElement attributes */
	NamedElement_VisitPathAttributes(((Instance*)(this))->super, parent, visitor, recursive);

	/* Local attributes */
	if(recursive)
	{
		sprintf(path,"%s\\metaData", parent);
		visitor->action(path, STRING, ((Instance*)(this))->metaData);

		sprintf(path,"%s\\started", parent);
		visitor->action(path, BOOL, (void*)((Instance*)(this))->started);
	}
}

void Instance_VisitReferences(void *const this, char *parent, Visitor *visitor, bool recursive)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	if(recursive)
	{
		if(((Instance*)(this))->typeDefinition != NULL)
		{
			visitor->action("typeDefinition", SQBRACKET, NULL);
			((Instance*)(this))->typeDefinition->VisitAttributes(((Instance*)(this))->typeDefinition, path, visitor, recursive);
			((Instance*)(this))->typeDefinition->VisitReferences(((Instance*)(this))->typeDefinition, path, visitor);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}
		else
		{
			visitor->action("typeDefinition", SQBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}

		if(((Instance*)(this))->dictionary != NULL)
		{
			visitor->action("dictionary", SQBRACKET, NULL);
			visitor->action(NULL, BRACKET, NULL);
			((Instance*)(this))->dictionary->VisitAttributes(((Instance*)(this))->dictionary, path, visitor);
			((Instance*)(this))->dictionary->VisitReferences(((Instance*)(this))->dictionary, path, visitor);
			visitor->action(NULL, CLOSEBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}
		else
		{
			visitor->action("dictionary", SQBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}

		hashmap_map* m = NULL;
		int i;

		if((m = (hashmap_map*) ((Instance*)(this))->fragmentDictionary) != NULL)
		{
			int length = hashmap_length(((Instance*)(this))->fragmentDictionary);

			visitor->action("fragmentDictionary", SQBRACKET, NULL);
			/* compare fragmentDictionary*/
			for(i = 0; i< m->table_size; i++)
			{
				if(m->data[i].in_use != 0)
				{
					visitor->action(NULL, BRACKET, NULL);
					any_t data = (any_t) (m->data[i].data);
					FragmentDictionary* n = data;
					n->VisitAttributes(n, path, visitor);
					n->VisitReferences(n, path, visitor);
					if(length > 1)
					{
						visitor->action(NULL, CLOSEBRACKETCOLON, NULL);
						length--;
					}
					else
						visitor->action(NULL, CLOSEBRACKET, NULL);
				}
			}
			visitor->action(NULL, CLOSESQBRACKET, NULL);
		}
		else
		{
			visitor->action("fragmentDictionary", SQBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKET, NULL);
		}
	}
	else
	{
		if(((Instance*)(this))->typeDefinition != NULL)
		{
			visitor->action("typeDefinition", SQBRACKET, NULL);
			sprintf(path, "typeDefinitions[%s]", ((Instance*)(this))->typeDefinition->InternalGetKey(((Instance*)(this))->typeDefinition));
			visitor->action(path, STRREF, NULL);
			visitor->action(NULL, RETURN, NULL);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}
		else
		{
			visitor->action("typeDefinition", SQBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}

		if(((Instance*)(this))->dictionary != NULL)
		{
			visitor->action("dictionary", SQBRACKET, NULL);
			visitor->action(NULL, BRACKET, NULL);
			((Instance*)(this))->dictionary->VisitAttributes(((Instance*)(this))->dictionary, path, visitor);
			((Instance*)(this))->dictionary->VisitReferences(((Instance*)(this))->dictionary, path, visitor);
			visitor->action(NULL, CLOSEBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}
		else
		{
			visitor->action("dictionary", SQBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKETCOLON, NULL);
		}

		hashmap_map* m = NULL;
		int i;

		if((m = (hashmap_map*) ((Instance*)(this))->fragmentDictionary) != NULL)
		{
			int length = hashmap_length(((Instance*)(this))->fragmentDictionary);

			visitor->action("fragmentDictionary", SQBRACKET, NULL);
			/* compare fragmentDictionary*/
			for(i = 0; i< m->table_size; i++)
			{
				if(m->data[i].in_use != 0)
				{
					visitor->action(NULL, BRACKET, NULL);
					any_t data = (any_t) (m->data[i].data);
					FragmentDictionary* n = data;
					n->VisitAttributes(n, path, visitor);
					n->VisitReferences(n, path, visitor);
					if(length > 1)
					{
						visitor->action(NULL, CLOSEBRACKETCOLON, NULL);
						length--;
					}
					else
						visitor->action(NULL, CLOSEBRACKET, NULL);
				}
			}
			visitor->action(NULL, CLOSESQBRACKET, NULL);
		}
		else
		{
			visitor->action("fragmentDictionary", SQBRACKET, NULL);
			visitor->action(NULL, CLOSESQBRACKET, NULL);
		}
	}
}

void Instance_VisitPathReferences(void *const this, char *parent, Visitor *visitor, bool recursive)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));

	if(recursive)
	{
		if(((Instance*)(this))->typeDefinition != NULL)
		{
			sprintf(path, "%s/typeDefinition[%s]", parent, ((Instance*)(this))->typeDefinition->InternalGetKey(((Instance*)(this))->typeDefinition));
			((Instance*)(this))->typeDefinition->VisitPathAttributes(((Instance*)(this))->typeDefinition, path, visitor, recursive);
			((Instance*)(this))->typeDefinition->VisitPathReferences(((Instance*)(this))->typeDefinition, path, visitor);
		}
		if(((Instance*)(this))->dictionary != NULL)
		{
			sprintf(path, "%s/dictionary[%s]", parent, ((Instance*)(this))->dictionary->InternalGetKey(((Instance*)(this))->dictionary));
			((Instance*)(this))->dictionary->VisitPathAttributes(((Instance*)(this))->dictionary, path, visitor);
			((Instance*)(this))->dictionary->VisitPathReferences(((Instance*)(this))->dictionary, path, visitor);
		}

		hashmap_map* m = NULL;
		int i;

		if((m = (hashmap_map*) ((Instance*)(this))->fragmentDictionary) != NULL)
		{
			PRINTF("Looking for fragmentDictionary\n");
			/* compare fragmentDictionary*/
			for(i = 0; i< m->table_size; i++)
			{
				if(m->data[i].in_use != 0)
				{
					any_t data = (any_t) (m->data[i].data);
					FragmentDictionary* n = data;
					sprintf(path, "%s/fragmentDictionary[%s]", parent, n->InternalGetKey(n));
					n->VisitPathAttributes(n, path, visitor);
					n->VisitPathReferences(n, path, visitor);
				}
			}
		}
	}
	else
	{
		if(((Instance*)(this))->typeDefinition != NULL)
		{
			sprintf(path, "%s/typeDefinition[%s]", parent, ((Instance*)(this))->typeDefinition->InternalGetKey(((Instance*)(this))->typeDefinition));
			((Instance*)(this))->typeDefinition->VisitPathAttributes(((Instance*)(this))->typeDefinition, path, visitor, recursive);
		}
		if(((Instance*)(this))->dictionary != NULL)
		{
			sprintf(path, "%s/dictionary[%s]", parent, ((Instance*)(this))->dictionary->InternalGetKey(((Instance*)(this))->dictionary));
			((Instance*)(this))->dictionary->VisitPathAttributes(((Instance*)(this))->dictionary, path, visitor);
			((Instance*)(this))->dictionary->VisitPathReferences(((Instance*)(this))->dictionary, path, visitor);
		}

		hashmap_map* m = NULL;
		int i;

		if((m = (hashmap_map*) ((Instance*)(this))->fragmentDictionary) != NULL)
		{
			/* compare fragmentDictionary*/
			for(i = 0; i< m->table_size; i++)
			{
				if(m->data[i].in_use != 0)
				{
					any_t data = (any_t) (m->data[i].data);
					FragmentDictionary* n = data;
					sprintf(path, "%s/fragmentDictionary[%s]", parent, n->InternalGetKey(n));
					n->VisitPathAttributes(n, path, visitor);
					n->VisitPathReferences(n, path, visitor);
				}
			}
		}
	}
}

void* Instance_FindByPath(char* attribute, Instance* const this)
{
	/* NamedElement attributes */
	if(!strcmp("name", attribute))
	{
		return this->super->FindByPath(attribute, this->super);
	}
	/* Local attributes */
	else if(!strcmp("metaData",attribute))
	{
		return this->metaData;
	}
	else if(!strcmp("started",attribute))
	{
		return (void*)this->started;
	}
	/* Local references */
	else
	{
		char path[250];
		memset(&path[0], 0, sizeof(path));
		char token[100];
		memset(&token[0], 0, sizeof(token));
		char *obj = NULL;
		char key[50];
		memset(&key[0], 0, sizeof(key));
		char nextPath[150];
		memset(&nextPath[0], 0, sizeof(nextPath));
		char *nextAttribute = NULL;

		strcpy(path, attribute);

		if(strchr(path, '[') != NULL)
		{
			obj = strdup(strtok(path, "["));
			strcpy(path, attribute);
			PRINTF("Object: %s\n", obj);
			strcpy(token, strtok(path, "]"));
			strcpy(path, attribute);
			sprintf(token, "%s]", token);
			PRINTF("Token: %s\n", token);
			sscanf(token, "%*[^[][%[^]]", key);
			PRINTF("Key: %s\n", key);

			if((strchr(path, '\\')) != NULL)
			{
				nextAttribute = strtok(NULL, "\\");
				PRINTF("Attribute: %s\n", nextAttribute);

				if(strchr(nextAttribute, '['))
				{
					sprintf(nextPath, "%s\\%s", ++nextAttribute, strtok(NULL, "\\"));
					PRINTF("Next Path: %s\n", nextPath);
				}
				else
				{
					strcpy(nextPath, nextAttribute);
					PRINTF("Next Path: %s\n", nextPath);
				}
			}
			else
			{
				nextAttribute = strtok(NULL, "\\");
				strcpy(nextPath, ++nextAttribute);
				PRINTF("Next Path: %s\n", nextPath);
				nextAttribute = NULL;
			}
		}
		else
		{
			nextAttribute = strtok(path, "\\");
			nextAttribute = strtok(NULL, "\\");
			PRINTF("Attribute: %s\n", nextAttribute);
		}

		if(!strcmp("typeDefinition", obj))
		{
			free(obj);
			if(nextAttribute == NULL)
			{
				return this->typeDefinition;
			}
			else
			{
				return this->typeDefinition->FindByPath(nextPath, this->typeDefinition);
			}
		}
		else if(!strcmp("dictionary", obj))
		{
			free(obj);
			if(nextAttribute == NULL)
			{
				return this->dictionary;
			}
			else
			{
				return this->dictionary->FindByPath(nextPath, this->dictionary);
			}
		}
		else if(!strcmp("fragmentDictionary", obj))
		{
			free(obj);
			if(nextAttribute == NULL)
			{
				return this->fragmentDictionary;
			}
			else
			{
				FragmentDictionary* value = this->FindFragmentDictionaryByID(this, key);
				if(value != NULL)
					return value->FindByPath(nextPath, value);
				else
					return NULL;
			}
		}
		else
		{
			free(obj);
			PRINTF("Wrong attribute or reference\n");
			return NULL;
		}
	}
}
