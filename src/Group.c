#include "Group.h"

Instance* newPoly_Group()
{
	Group* pGroupObj = NULL;
	Instance* pObj = new_Instance();

	/* Allocating memory */
	pGroupObj = (Group*)malloc(sizeof(Group));

	if (pGroupObj == NULL)
	{
		pObj->Delete(pObj);
		return NULL;
	}

	pObj->pDerivedObj = pGroupObj; /* Pointing to derived object */
	
	/*pGroupObj->subNodes = hashmap_new();*/
	pGroupObj->subNodes = NULL;;
	
	pGroupObj->AddSubNodes = Group_AddSubNodes;
	pGroupObj->RemoveSubNodes = Group_RemoveSubNodes;
	pGroupObj->FindSubNodesByID = Group_FindSubNodesByID;
	pGroupObj->FindByPath = Group_FindByPath;
	
	pObj->InternalGetKey = Group_InternalGetKey;
	pObj->MetaClassName = Group_MetaClassName;
	pObj->VisitAttributes = Group_VisitAttributes;
	pObj->VisitReferences = Group_VisitReferences;
	
	pObj->Delete = deletePoly_Group;

	return pObj;
}

Group* new_Group()
{
	Group* pGroupObj = NULL;
	Instance* pObj = new_Instance();
	
	if(pObj == NULL)
		return NULL;

	/* Allocating memory */
	pGroupObj = (Group*)malloc(sizeof(Group));

	if (pGroupObj == NULL)
	{
		return NULL;
	}

	/*pObj->pDerivedObj = pGroupObj; Pointing to derived object */
	pGroupObj->super = pObj;
	
	/*pGroupObj->subNodes = hashmap_new();*/
	pGroupObj->subNodes = NULL;
	
	pGroupObj->AddSubNodes = Group_AddSubNodes;
	pGroupObj->RemoveSubNodes = Group_RemoveSubNodes;
	
	pGroupObj->InternalGetKey = Group_InternalGetKey;
	pGroupObj->MetaClassName = Group_MetaClassName;
	pGroupObj->VisitAttributes = Group_VisitAttributes;
	pGroupObj->VisitReferences = Group_VisitReferences;
	pGroupObj->FindSubNodesByID = Group_FindSubNodesByID;
	pGroupObj->FindByPath = Group_FindByPath;
	pGroupObj->Delete = delete_Group;

	return pGroupObj;
}

char* Group_InternalGetKey(Group* const this)
{
	char* internalKey;

	if (this == NULL)
		return NULL;

	internalKey = malloc(sizeof(char) * (strlen(this->super->super->name)));

	if (internalKey == NULL)
		return NULL;

	strcpy(internalKey, this->super->super->name);

	return internalKey;
}

char* Group_MetaClassName(Group* const this)
{
	char* name;

	name = malloc(sizeof(char) * (strlen("Group") + 1));
	strcpy(name, "Group");
	
	return name;
}

ContainerNode* Group_FindSubNodesByID(Group* const this, char* id)
{
	ContainerNode* value;
	
	if(this->subNodes != NULL)
	{
		if(hashmap_get(this->subNodes, id, (void**)(&value)) == MAP_OK)
			return value;
		else
			return NULL;
	}
	else
	{
		return NULL;
	}
}

/*void Group::addsubNodes(ContainerNode *ptr)*/
void Group_AddSubNodes(Group* const this, ContainerNode* ptr)
{
	ContainerNode* container = (ContainerNode*)ptr;
	
	/*if(container->internalGetKey().empty())*/
	if(container->InternalGetKey(container) == NULL)
	{
		/*LOGGER_WRITE(Logger::WARNING,"The ContainerNode cannot be added in Group because the key is not defined");*/
		printf("The ContainerNode cannot be added in Group because the key is not defined\n");
	}
	else
	{
		/*if(subNodes.find(container->internalGetKey()) == subNodes.end())*/
		if(this->subNodes == NULL)
		{
			this->subNodes = hashmap_new();
		}
		if(hashmap_get(this->subNodes, container->InternalGetKey(container), (void**)(&container)) == MAP_MISSING);
		{
			/*subNodes[container->internalGetKey()]=ptr;*/
			container = (ContainerNode*)ptr;
			hashmap_put(this->subNodes, container->InternalGetKey(container), ptr);
		}
	}
}

/*void Group::removesubNodes(ContainerNode *ptr)*/
void Group_RemoveSubNodes(Group* const this, ContainerNode* ptr)
{
	ContainerNode* container = (ContainerNode*)ptr;
	
	/*if(container->internalGetKey().empty())*/
	if(container->InternalGetKey(container) == NULL)
	{
		/*LOGGER_WRITE(Logger::WARNING,"The ContainerNode cannot be removed in Group because the key is not defined");*/
		printf("The ContainerNode cannot be removed in Group because the key is not defined\n");
	}
	else
	{
		/*subNodes.erase( subNodes.find(container->internalGetKey()));*/
		hashmap_remove(this->subNodes, container->InternalGetKey(container));
		/*container->setEContainer(NULL,NULL,"");*/
	}
}

void deletePoly_Group(Instance* const this)
{
	Group* pGroupObj;
	pGroupObj = this->pDerivedObj;
	/*destroy derived obj*/
	hashmap_free(pGroupObj->subNodes);
	free(pGroupObj);
	/*destroy base Obj*/
	delete_Instance(this);
}

void delete_Group(Group* const this)
{
	/* destroy base object */
	delete_Instance(this->super);
	/* destroy data memebers */
	hashmap_free(this->subNodes);
	free(this);
}

void Group_VisitAttributes(void* const this, char* parent, Visitor* visitor)
{
	/*char path[256];
	memset(&path[0], 0, sizeof(path));*/

	/*sprintf(path, "%s", parent, ((Group*)(this))->super->super->name);*/

	Instance_VisitAttributes(((Group*)(this))->super, parent, visitor);
}

void Group_VisitReferences(void* const this, char* parent, Visitor* visitor)
{
	char path[256];
	memset(&path[0], 0, sizeof(path));
	
	Instance_VisitReferences(((Group*)(this))->super, parent, visitor);
	
	if(((Group*)(this))->subNodes != NULL)
	{
		int i;
		
		/* subNodes */
		hashmap_map* m = ((Group*)(this))->subNodes;

		/* compare subNodes */
		for(i = 0; i< m->table_size; i++)
		{
			if(m->data[i].in_use != 0)
			{
				any_t data = (any_t) (m->data[i].data);
				ContainerNode* n = data;
				sprintf(path,"%s/subNodes[%s]", parent, /*n->super->super->name*/n->InternalGetKey(n));
				n->VisitAttributes(n, path, visitor);
				n->VisitReferences(n, path, visitor);
			}
		}
	}
}

void* Group_FindByPath(char* attribute, Group* const this)
{
	/* There is no local attributes */
	
	/* Instance attributes and references */
	if(!strcmp("name",attribute) ||  !strcmp("metaData",attribute) || !strcmp("started",attribute) || !strcmp("typeDefinition",attribute))
	{
		return Instance_FindByPath(attribute, this->super);/*return this->super->metaData;*/
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
			/*nextAttribute = strtok(NULL, "\\");
			sprintf(nextAttribute, "%s\\%s", nextAttribute, strtok(NULL, "\\"));*/
			if(strchr(attribute,'\\') != NULL)
			{
				/*printf("Attribute found at: %d\n", strchr(attribute,'\\')-attribute+1);*/
				nextAttribute = strtok(NULL, "\\");
				sprintf(nextAttribute, "%s\\%s", nextAttribute, strtok(NULL, "\\"));
			}
			else
			{
				/*printf("Attribute not found, looking for path\n");*/
				nextAttribute = strtok(NULL, "\\");
			}
		}
		else
		{
			pch = path;
			nextAttribute = strtok(pch, "\\");
			nextAttribute = strtok(NULL, "\\");
		}
		
		/*printf("Token: %s\n", pch);*/

		int i = indexOf(pch,"[") + 2;
		int y = lastIndexOf(pch,"]") - i + 1;

		char* relationName = (char*)Substring(pch, 0, i - 2);
		char* queryID = (char*)Substring(pch, i, y);
		
		/*printf("relationName: %s\n", relationName);
		printf("queryID: %s\n", queryID);
		printf("next attribute: %s\n", nextAttribute);*/
	  
		if(!strcmp("subNodes", relationName))
		{
			if(nextAttribute == NULL)
			{
				
				return this->FindSubNodesByID(this, queryID);
			}
			else
			{
				ContainerNode* contnode = this->FindSubNodesByID(this, queryID);
				if(contnode != NULL)
					return contnode->FindByPath(nextAttribute, contnode);
				else
					return NULL;
			}
		}
		else
		{
			return Instance_FindByPath(attribute, this->super);
		}
	}
}

/*int _acceptGroup(Group* this, Group* c, Visitor* visitor)
{
	int i;

	for(i = 0; i < this->count_subNodes; i++)
	{
		visitor->action((void*)this->subNodes, (void*)c->subNodes, 0);
	}
}*/
