#include "JSONModelLoader.h"
#include "DefaultFactorykevoree.h"
#include "kevoree.h"
#include "json.h"
#include "jsonparse.h"
#include "hashmap.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

JSONModelLoader *new_JSONModelLoader()
{
	JSONModelLoader *pObj = NULL;

	pObj = (JSONModelLoader*)malloc(sizeof(JSONModelLoader));

	if(pObj == NULL)
	{
		return NULL;
	}

	pObj->objects = hashmap_new();

	pObj->addObjectReference = JSONModelLoader_AddObjectReference;
	pObj->delete = delete_JSONModelLoader;

	return pObj;
}

ObjectReference *new_ObjectReference(char *srcObjId, char *refId)
{
	ObjectReference *pObj = NULL;

	pObj = (ObjectReference*)malloc(sizeof(ObjectReference));

	if(pObj == NULL)
	{
		return NULL;
	}

	pObj->refId = refId;
	pObj->srcObjectId = srcObjId;

	pObj->mapId = malloc(sizeof(char) * strlen(refId) * strlen(srcObjId) + 1);

	sprintf(pObj->mapId, "%s/%s", refId, srcObjId);

	pObj->delete = delete_ObjectReference;

	return pObj;
}

void delete_JSONModelLoader(void *const this)
{
	JSONModelLoader *pObj = (JSONModelLoader*)this;

	if(pObj != NULL)
	{
		hashmap_free(pObj->objects);
		free(this);
	}
}

void delete_ObjectReference(void *const this)
{
	ObjectReference *pObj = (ObjectReference*)this;

	if(this != NULL)
	{
		free(pObj->refId);
		free(pObj->srcObjectId);
		free(pObj->mapId);
		free(this);
	}
}

int JSONModelLoader_AddObjectReference(JSONModelLoader *const this, ObjectReference *obj)
{
	ObjectReference *container;

	if(hashmap_get(this->objects, obj->mapId, (void**)(&container)) == MAP_MISSING)
	{
		return hashmap_put(this->objects, obj->mapId, obj);
	}
	else
	{
		PRINTF("Object %s already exists!!\n", obj->srcObjectId);
	}

	return MAP_MISSING;
}

int resolveReferences(any_t root, any_t objRef)
{
	ContainerRoot *model = (ContainerRoot*)root;
	char *srcObj;
	char srcId[50];
	char *srcObjId;
	char *refObj;
	char refId[50];
	char *refObjId;
	char *srcObj2;
	char srcId2[50];
	char *srcObjId2;

	if(objRef != NULL && model != NULL)
	{
		ObjectReference *obj = (ObjectReference*)objRef;

		srcObj = strdup(obj->srcObjectId);
		PRINTF("srcObj: %s\n", srcObj);
		refObj = strdup(obj->refId);
		PRINTF("refObj: %s\n", refObj);

		sscanf(srcObj, "%*[^[][%[^]]", srcId);
		PRINTF("srcId: %s\n", srcId);
		sscanf(refObj, "%*[^[][%[^]]", refId);
		PRINTF("refId: %s\n", refId);

		srcObjId = strtok(srcObj, "[");
		PRINTF("srcObjId: %s\n", srcObjId);
		refObjId = strtok(refObj, "[");
		PRINTF("refObjId: %s\n", refObjId);

		if(!strcmp(srcObjId, "nodes"))
		{
			srcObj2 = strtok(srcObj, "/");
			srcObj2 = strtok(NULL, "/");
			PRINTF("srcObj2: %s\n", srcObj2);

			if(srcObj2 != NULL)
			{
				sscanf(srcObj2, "%*[^[][%[^]]", srcId2);
				PRINTF("srcId2: %s\n", srcId2);

				srcObjId2 = strtok(srcObj2, "[");
				PRINTF("srcObjId2: %s\n", srcObjId2);
			}
			else
			{
				PRINTF("There is no srcObj2!\n");
				memset(&srcId2[0], 0, sizeof(srcId2));
				srcObjId2 = NULL;
			}

		}

		if(!strcmp(srcObjId, "nodes"))
		{
			if(!strcmp(refObjId, "groups"))
			{
				Group *grp = model->FindGroupsByID(model, refId);
				ContainerNode *node = model->FindNodesByID(model, srcId);
				if(grp != NULL && node != NULL)
				{
					PRINTF("Adding group %s to node %s\n", grp->InternalGetKey(grp), node->InternalGetKey(node));
					node->AddGroups(node, grp);
					return MAP_OK;
				}
				else
				{
					PRINTF("Cannot add group reference to node!\n");
				}
			}
			else if(!strcmp(refObjId, "typeDefinitions") && strcmp(srcObjId2, "components"))
			{
				TypeDefinition *typdef = model->FindTypeDefsByID(model, refId);
				ContainerNode *node = model->FindNodesByID(model, srcId);
				if(typdef != NULL && node != NULL)
				{
					PRINTF("Adding typeDefinition %s to node %s\n", typdef->InternalGetKey(typdef), node->InternalGetKey(node));
					node->super->AddTypeDefinition(node->super, typdef);
					return MAP_OK;
				}
				else
				{
					PRINTF("Cannot add typeDefinitions reference to node!\n");
				}
			}
			else if(!strcmp(refObjId, "typeDefinitions") && !strcmp(srcObjId2, "components"))
			{
				TypeDefinition *typdef = model->FindTypeDefsByID(model, refId);
				ContainerNode *node = model->FindNodesByID(model, srcId);
				ComponentInstance *comp = node->FindComponentsByID(node, srcId2);
				if(typdef != NULL && node != NULL && comp != NULL)
				{
					PRINTF("Adding typeDefinition %s to component %s\n", typdef->InternalGetKey(typdef), comp->InternalGetKey(comp));
					comp->super->AddTypeDefinition(comp->super, typdef);
					return MAP_OK;
				}
				else
				{
					PRINTF("Cannot add typeDefinitions reference to node!\n");
				}
			}
		}
		else if(!strcmp(srcObjId, "typeDefinitions"))
		{
			if(!strcmp(refObjId, "deployUnits"))
			{
				DeployUnit *du = model->FindDeployUnitsByID(model, refId);
				TypeDefinition *typdef = model->FindTypeDefsByID(model, srcId);
				if(du != NULL && typdef != NULL)
				{
					PRINTF("Adding deployUnit %s to typeDefinition %s\n", du->InternalGetKey(du), typdef->InternalGetKey(typdef));
					typdef->AddDeployUnit(typdef, du);
					return MAP_OK;
				}
				else
				{
					PRINTF("Cannot add deployUnits reference to typeDefinitions!\n");
				}
			}
		}
		else if(!strcmp(srcObjId, "libraries"))
		{
			if(!strcmp(refObjId, "typeDefinitions"))
			{
				TypeDefinition *typdef = model->FindTypeDefsByID(model, refId);
				TypeLibrary *typlib = model->FindLibrariesByID(model, srcId);
				if(typdef != NULL && typlib != NULL)
				{
					PRINTF("Adding typeDefinition %s to library %s\n", typdef->InternalGetKey(typdef), typlib->InternalGetKey(typlib));
					typlib->AddSubTypes(typlib, typdef);
					return MAP_OK;
				}
				else
				{
					PRINTF("Cannot add typeDefinition reference to libraries!\n");
				}
			}
		}
		else if(!strcmp(srcObjId, "groups"))
		{
			if(!strcmp(refObjId, "nodes"))
			{
				ContainerNode *node = model->FindNodesByID(model, refId);
				Group *group = model->FindGroupsByID(model, srcId);
				if(node != NULL && group != NULL)
				{
					PRINTF("Adding node %s to group %s\n", node->InternalGetKey(node), group->InternalGetKey(group));
					group->AddSubNodes(group, node);
					return MAP_OK;
				}
				else
				{
					PRINTF("Cannot add node reference to group!\n");
				}
			}
			else if(!strcmp(refObjId, "typeDefinitions"))
			{
				TypeDefinition *typdef = model->FindTypeDefsByID(model, refId);
				Group *group = model->FindGroupsByID(model, srcId);
				if(typdef != NULL && group != NULL)
				{
					group->super->AddTypeDefinition(group->super, typdef);
					return MAP_OK;
				}
				else
				{
					PRINTF("Cannot add node reference to group!\n");
				}
			}
		}
		else
		{
			PRINTF("Source not found!\n");
			return MAP_MISSING;
		}
	}
	else
	{
		PRINTF("Reference and/or model invalid!\n");
		return MAP_MISSING;
	}

	return MAP_MISSING;
}

ContainerRoot *JSONKevDeserializer(struct jsonparse_state *jsonState, char _jsonType)
{
	ContainerRoot *new_model = NULL;

	JSONModelLoader *loader = new_JSONModelLoader();
	if(loader == NULL)
	{
		PRINTF("Loader cannot be created!\n");
		return NULL;
	}

	char strJson[MAX_STRJSON_SIZE];
	char jsonType;

	switch(_jsonType)
	{
	case JSON_TYPE_OBJECT:
		/*jsonparse_copy_value(jsonState, strJson, sizeof strJson);*/
		PRINTF("JSON_TYPE_OBJECT\n\t{\n");
		while((jsonType = jsonparse_next(jsonState))!= '}')
		{
			switch(jsonType)
			{
			case JSON_TYPE_PAIR_NAME:
				jsonparse_copy_value(jsonState, strJson, sizeof strJson);
				PRINTF("JSON_TYPE_PAIR_NAME\n%s\n", strJson);
				if(!strcmp(strJson, "eClass"))
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, sizeof strJson);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_STRING:
							jsonparse_copy_value(jsonState, strJson, sizeof strJson);
							PRINTF("%s\n", strJson);
							if(!strcmp(strJson, "org.kevoree.ContainerRoot"))
							{
								new_model = new_ContainerRoot();
								if(new_model == NULL)
								{
									PRINTF("New model cannot be created!\n");
								}
								else
								{
									PRINTF("New model %s created!\n", new_model->generated_KMF_ID);
								}
							}
							break;
						}
						break;
					}
				}
				else if(!strcmp(strJson, "generated_KMF_ID"))
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_STRING:
							jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
							strcpy(new_model->generated_KMF_ID, strJson);
							PRINTF("%s -> %s\n", strJson, new_model->generated_KMF_ID);
							break;
						}
						break;
					}
				}
				else if(!strcmp(strJson, "nodes"))
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_ARRAY:
							/*jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);*/
							PRINTF("[\n");
							while((jsonType = jsonparse_next(jsonState))!= ']')
							{
								switch(jsonType)
								{
								case JSON_TYPE_OBJECT:
									PRINTF("JSON_TYPE_OBJECT\n\t{\n");
									switch(jsonparse_next(jsonState))
									{
									case JSON_TYPE_PAIR_NAME:
										jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
										PRINTF("JSON_TYPE_PAIR_NAME\n%s\n", strJson);
										if(!strcmp(strJson, "eClass"))
										{
											switch(jsonparse_next(jsonState))
											{
											case JSON_TYPE_PAIR:
												jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
												PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
												switch(jsonparse_next(jsonState))
												{
												case JSON_TYPE_STRING:
													jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
													if(!strcmp(strJson, "org.kevoree.ContainerNode"))
													{
														ContainerNode *node = createContainerNode(jsonState, jsonparse_next(jsonState), strJson, new_model, loader);
														if(new_model != NULL && node != NULL)
														{
															new_model->AddNodes(new_model, node);
															PRINTF("Successfully added node %s to new_model %s\n", node->InternalGetKey(node), new_model->InternalGetKey(new_model));
														}
														else
														{
															PRINTF("Node cannot be added to model!\n");
														}
													}
													break;
												}
												break;
											}
										}
										else
										{
											PRINTF("Invalid object!\n");
										}
										break;
									}
									break;
								}
							}
							PRINTF("]\n");
						}
						break;
					}
				}
				else if(!strcmp(strJson, "typeDefinitions"))
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_ARRAY:
							/*jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);*/
							PRINTF("[\n");
							while((jsonType = jsonparse_next(jsonState))!= ']')
							{
								switch(jsonType)
								{
								case JSON_TYPE_OBJECT:
									PRINTF("JSON_TYPE_OBJECT\n\t{\n");
									switch(jsonparse_next(jsonState))
									{
									case JSON_TYPE_PAIR_NAME:
										jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
										PRINTF("JSON_TYPE_PAIR_NAME\n%s\n", strJson);
										if(!strcmp(strJson, "eClass"))
										{
											switch(jsonparse_next(jsonState))
											{
											case JSON_TYPE_PAIR:
												jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
												PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
												switch(jsonparse_next(jsonState))
												{
												case JSON_TYPE_STRING:
													jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
													if(!strcmp(strJson, "org.kevoree.NodeType") ||
															!strcmp(strJson, "org.kevoree.GroupType") ||
															!strcmp(strJson, "org.kevoree.ComponentType"))
													{
														TypeDefinition *typdef = createTypeDefinition(jsonState, jsonparse_next(jsonState), strJson, new_model, loader);
														if(new_model != NULL && typdef != NULL)
														{
															PRINTF("Adding TypeDefinition %s to new_model %s...\n", typdef->InternalGetKey(typdef), new_model->InternalGetKey(new_model));
															new_model->AddTypeDefinitions(new_model, typdef);
															PRINTF("TypeDefinition added successfully!\n");
														}
														else
														{
															PRINTF("TypeDefinition cannot be added to model!\n");
														}
													}
													break;
												}
												break;
											}
										}
										else
										{
											PRINTF("Invalid object!\n");
										}
										break;
									}
									break;
								}
							}
							PRINTF("]\n");
						}
						break;
					}
				}
				else if(!strcmp(strJson, "libraries"))
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_ARRAY:
							/*jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);*/
							PRINTF("[\n");
							while((jsonType = jsonparse_next(jsonState))!= ']')
							{
								switch(jsonType)
								{
								case JSON_TYPE_OBJECT:
									PRINTF("JSON_TYPE_OBJECT\n\t{\n");
									switch(jsonparse_next(jsonState))
									{
									case JSON_TYPE_PAIR_NAME:
										jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
										PRINTF("JSON_TYPE_PAIR_NAME\n%s\n", strJson);
										if(!strcmp(strJson, "eClass"))
										{
											switch(jsonparse_next(jsonState))
											{
											case JSON_TYPE_PAIR:
												jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
												PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
												switch(jsonparse_next(jsonState))
												{
												case JSON_TYPE_STRING:
													jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
													if(!strcmp(strJson, "org.kevoree.TypeLibrary"))
													{
														TypeLibrary *typlib = createTypeLibrary(jsonState, jsonparse_next(jsonState), strJson, new_model, loader);
														if(new_model != NULL && typlib != NULL)
															new_model->AddLibraries(new_model, typlib);
														else
															PRINTF("TypeLibrary cannot be added to model!\n");
													}
													break;
												}
												break;
											}
										}
										else
										{
											PRINTF("Invalid object!\n");
										}
										break;
									}
									break;
								}
							}
							PRINTF("]\n");
						}
						break;
					}
				}
				else if(!strcmp(strJson, "deployUnits"))
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_ARRAY:
							/*jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);*/
							PRINTF("[\n");
							while((jsonType = jsonparse_next(jsonState))!= ']')
							{
								switch(jsonType)
								{
								case JSON_TYPE_OBJECT:
									PRINTF("JSON_TYPE_OBJECT\n\t{\n");
									switch(jsonparse_next(jsonState))
									{
									case JSON_TYPE_PAIR_NAME:
										jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
										PRINTF("JSON_TYPE_PAIR_NAME\n%s\n", strJson);
										if(!strcmp(strJson, "eClass"))
										{
											switch(jsonparse_next(jsonState))
											{
											case JSON_TYPE_PAIR:
												jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
												PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
												switch(jsonparse_next(jsonState))
												{
												case JSON_TYPE_STRING:
													jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
													if(!strcmp(strJson, "org.kevoree.DeployUnit"))
													{
														DeployUnit *depunit = createDeployUnit(jsonState, jsonparse_next(jsonState), strJson, new_model);
														if(new_model != NULL && depunit != NULL)
															new_model->AddDeployUnits(new_model, depunit);
														else
															PRINTF("DeployUnit cannot be added to model!\n");
													}
													break;
												}
												break;
											}
										}
										else
										{
											PRINTF("Invalid object!\n");
										}
										break;
									}
									break;
								}
							}
							PRINTF("]\n");
						}
						break;
					}
				}
				else if(!strcmp(strJson, "groups"))
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_ARRAY:
							/*jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);*/
							PRINTF("[\n");
							while((jsonType = jsonparse_next(jsonState))!= ']')
							{
								switch(jsonType)
								{
								case JSON_TYPE_OBJECT:
									PRINTF("JSON_TYPE_OBJECT\n\t{\n");
									switch(jsonparse_next(jsonState))
									{
									case JSON_TYPE_PAIR_NAME:
										jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
										PRINTF("JSON_TYPE_PAIR_NAME\n%s\n", strJson);
										if(!strcmp(strJson, "eClass"))
										{
											switch(jsonparse_next(jsonState))
											{
											case JSON_TYPE_PAIR:
												jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
												PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
												switch(jsonparse_next(jsonState))
												{
												case JSON_TYPE_STRING:
													jsonparse_copy_value(jsonState, strJson, MAX_STRJSON_SIZE);
													if(!strcmp(strJson, "org.kevoree.Group"))
													{
														Group *group = createGroup(jsonState, jsonparse_next(jsonState), strJson, new_model, loader);
														if(new_model != NULL && group != NULL)
															new_model->AddGroups(new_model, group);
														else
															PRINTF("Group cannot be added to model!\n");
													}
													break;
												}
												break;
											}
										}
										else
										{
											PRINTF("Invalid object!\n");
										}
										break;
									}
									break;
								}
							}
							PRINTF("]\n");
						}
						break;
					}
				}
				else
				{
					switch(jsonparse_next(jsonState))
					{
					case JSON_TYPE_PAIR:
						jsonparse_copy_value(jsonState, strJson, sizeof strJson);
						PRINTF("JSON_TYPE_PAIR\n\t\t\t%s : ", strJson);
						switch(jsonparse_next(jsonState))
						{
						case JSON_TYPE_STRING:
							jsonparse_copy_value(jsonState, strJson, sizeof strJson);
							PRINTF("%s\n", strJson);
							break;

						case JSON_TYPE_ARRAY:
							/*jsonparse_copy_value(jsonState, strJson, sizeof strJson);*/
							PRINTF("[\n");
							while((jsonType = jsonparse_next(jsonState))!= ']')
							{
								switch(jsonType)
								{
								/*case JSON_TYPE_OBJECT:
																	JSONKevDeserializer(jsonState, jsonType, NULL);
																	break;*/

								case JSON_TYPE_STRING:
									jsonparse_copy_value(jsonState, strJson, sizeof strJson);
									PRINTF("\t\t\t\t%s\n", strJson);
									break;

								case JSON_TYPE_INT:
									jsonparse_copy_value(jsonState, strJson, sizeof strJson);
									PRINTF("\t\t\t\t%s\n", strJson);
									break;

								case JSON_TYPE_NUMBER:
									jsonparse_copy_value(jsonState, strJson, sizeof strJson);
									PRINTF("\t\t\t\t%s\n", strJson);
									break;
								}
							}
							PRINTF("\t\t\t]\n");
							break;

						case JSON_TYPE_INT:
							jsonparse_copy_value(jsonState, strJson, sizeof strJson);
							PRINTF("JSON_TYPE_INT\n\t%s\n", strJson);
							break;

						case JSON_TYPE_NUMBER:
							jsonparse_copy_value(jsonState, strJson, sizeof strJson);
							PRINTF("JSON_TYPE_NUMBER\n\t%s\n", strJson);
							break;

						case JSON_TYPE_ERROR:
							PRINTF("JSON_TYPE_ERROR\n\t(empty)\n");
							break;
						}
						break;
					}
				}
				break;
			}
		}
		PRINTF("}\n");
		break;
	}

	if(loader->objects != NULL)
	{
		PRINTF("Resolving dependencies...\n");
		hashmap_iterate(loader->objects, resolveReferences, new_model);
		return new_model;
	}
	else
	{
		PRINTF("No dependencies!\n");
		return new_model;
	}
}
