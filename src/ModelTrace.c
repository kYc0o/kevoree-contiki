#include "ModelTrace.h"

/**
 * Author: fco.ja.ac@gmail.com
 * Date: 21/07/2014
 * Time: 17:55
 */

ModelTrace* new_ModelTrace()
{
	ModelTrace* pObj = NULL;
	/* Allocating memory */
	pObj = (ModelTrace*)malloc(sizeof(ModelTrace));

	if (pObj == NULL)
	{
		return NULL;
	}

	/* pointing to itself as we are creating base class object*/
	pObj->pDerivedObj = pObj;
	
	pObj->srcPath = NULL;
	pObj->refName = NULL;
	
	pObj->ToString = ModelTrace_ToString;
	pObj->Delete = delete_ModelTrace;
}

void delete_ModelTrace(ModelTrace* const this)
{
	if(this != NULL)
	{
		free(this->srcPath);
		free(this->refName);
		free(this);
	}
}

char* ModelTrace_ToString(void* const this)
{
  
}

/*ModelSetTrace::ModelSetTrace (string _srcPath, string _refName, string _objPath, string _content, string _typeName)
{
	srcPath = _srcPath;
	refName = _refName;
	typeName = _typeName;
	content = _content;
	typeName = _typeName;
}*/

ModelTrace* newPoly_ModelSetTrace(char* _srcPath, char* _refName, char* _objPath, char* _content, char* _typeName)
{
	ModelSetTrace* pModSetTraceObj = NULL;
	ModelTrace* pObj = new_ModelTrace();

	/* Allocating memory */
	pModSetTraceObj = (ModelSetTrace*)malloc(sizeof(ModelSetTrace));

	if (pModSetTraceObj == NULL)
	{
		pObj->Delete(pObj);
		return NULL;
	}

	pObj->pDerivedObj = pModSetTraceObj; /* Pointing to derived object */
	
	pObj->srcPath = malloc(sizeof(char)*(strlen(_srcPath)+1));
	strcpy(pObj->srcPath, _srcPath);
	pObj->refName = malloc(sizeof(char)*(strlen(_refName)+1));
	strcpy(pObj->refName, _refName);
	pModSetTraceObj->objPath = malloc(sizeof(char)*(strlen(_objPath)+1));
	strcpy(pModSetTraceObj->objPath, _objPath);
	pModSetTraceObj->content = malloc(sizeof(char)*(strlen(_content)+1));
	strcpy(pModSetTraceObj->content, _content);
	pModSetTraceObj->typeName = malloc(sizeof(char)*(strlen(_typeName)+1));
	strcpy(pModSetTraceObj->typeName, _typeName);
	
	pModSetTraceObj->Delete = delete_ModelSetTrace;
	pModSetTraceObj->ToString = ModelSetTrace_ToString;
	pObj->ToString = ModelSetTrace_ToString;

	return pObj;
}

ModelSetTrace* new_ModelSetTrace(char* _srcPath, char* _refName, char* _objPath, char* _content, char* _typeName)
{
	ModelSetTrace* pModSetTraceObj = NULL;
	ModelTrace* pObj = new_ModelTrace();

	/* Allocating memory */
	pModSetTraceObj = (ModelSetTrace*)malloc(sizeof(ModelSetTrace));

	if (pModSetTraceObj == NULL)
	{
		return NULL;
	}

	/*pObj->pDerivedObj = pInstanceObj; Pointing to derived object */
	pModSetTraceObj->super = pObj;
	
	pObj->srcPath = malloc(sizeof(char)*(strlen(_srcPath)+1));
	strcpy(pObj->srcPath, _srcPath);
	pObj->refName = malloc(sizeof(char)*(strlen(_refName)+1));
	strcpy(pObj->refName, _refName);
	pModSetTraceObj->objPath = malloc(sizeof(char)*(strlen(_objPath)+1));
	strcpy(pModSetTraceObj->objPath, _objPath);
	pModSetTraceObj->content = malloc(sizeof(char)*(strlen(_content)+1));
	strcpy(pModSetTraceObj->content, _content);
	pModSetTraceObj->typeName = malloc(sizeof(char)*(strlen(_typeName)+1));
	strcpy(pModSetTraceObj->typeName, _typeName);
	
	pModSetTraceObj->Delete = delete_ModelSetTrace;
	pModSetTraceObj->ToString = ModelSetTrace_ToString;
	pObj->ToString = ModelSetTrace_ToString;

	return pModSetTraceObj;
}

void deletePoly_ModelSetTrace(ModelTrace* const this)
{
	ModelSetTrace* pModSetTraceObj;
	pModSetTraceObj = this->pDerivedObj;
	/*destroy derived obj*/
	free(pModSetTraceObj->objPath);
	free(pModSetTraceObj->content);
	free(pModSetTraceObj->typeName);
	free(pModSetTraceObj);
	/*destroy base Obj*/
	delete_ModelTrace(this);
}

void delete_ModelSetTrace(ModelSetTrace* const this)
{
	/* destroy base object */
	delete_ModelTrace(this->super);
	/* destroy data memebers */
	free(this->objPath);
	free(this->content);
	free(this->typeName);
	free(this);
}

/*std::string ModelSetTrace::toString ()*/
char* ModelSetTrace_ToString(ModelSetTrace* const this)
{
	/*string buffer = "";*/
	char* buffer = NULL;
	
	/*buffer.append ("{ \"traceType\" : " + ActionType (SET) + " , \"src\" : \"" + srcPath + "\", \"refname\" : \"" + refName + "\"");*/
	sprintf(buffer, "{ \"traceType\" : %s, \"src\" : \"%s\", \"refname\" : \"%s\"", ActionType(SET), this->super->srcPath, this->super->refName);
	
	/*if (!objPath.empty ())*/
	if(this->objPath != NULL)
	{
		/*buffer.append (", \"objpath\" : \"" + objPath + "\"");*/
		sprintf(buffer, "%s, \"objpath\" : \"%s\"", buffer, this->objPath);
	}
	/*if (!content.empty ())*/
	if(this->content != NULL)
	{
		/*buffer.append (", \"content\" : \"" + content + "\"");*/
		sprintf(buffer, "%s, \"content\" : \" %s \"", buffer, this->content);
	}
	/*if (!typeName.empty ())*/
	if(this->typeName != NULL)
	{
		/*buffer.append (", \"typename\" : \"" + typeName + "\"");*/
		sprintf(buffer, "%s, \"typename\" : \"%s\"", buffer, this->typeName);
	}
	
	/*buffer.append ("}\n");*/
	sprintf(buffer, "%s }\n", buffer);

	return buffer;
}  

/*ModelAddTrace::ModelAddTrace(string _srcPath, string _refName, string _previousPath,string _typeName)
{
	srcPath  =_srcPath;
	refName=_refName;
	previousPath =_previousPath;
	typeName=_typeName;
}*/

ModelTrace* newPoly_ModelAddTrace(char* _srcPath, char* _refName, char* _previousPath, char* _typeName)
{
	ModelAddTrace* pModAddTraceObj = NULL;
	ModelTrace* pObj = new_ModelTrace();

	/* Allocating memory */
	pModAddTraceObj = (ModelAddTrace*)malloc(sizeof(ModelAddTrace));

	if (pModAddTraceObj == NULL)
	{
		pObj->Delete(pObj);
		return NULL;
	}

	pObj->pDerivedObj = pModAddTraceObj; /* Pointing to derived object */
	
	pObj->srcPath = malloc(sizeof(char)*(strlen(_srcPath)+1));
	strcpy(pObj->srcPath, _srcPath);
	pObj->refName = malloc(sizeof(char)*(strlen(_refName)+1));
	strcpy(pObj->refName, _refName);
	pModAddTraceObj->previousPath = malloc(sizeof(char)*(strlen(_previousPath)+1));
	strcpy(pModAddTraceObj->previousPath, _previousPath);
	pModAddTraceObj->typeName = malloc(sizeof(char)*(strlen(_typeName)+1));
	strcpy(pModAddTraceObj->typeName, _typeName);
	
	pModAddTraceObj->Delete = delete_ModelAddTrace;
	pModAddTraceObj->ToString = ModelAddTrace_ToString;
	pObj->ToString = ModelAddTrace_ToString;

	return pObj;
}

ModelAddTrace* new_ModelAddTrace(char* _srcPath, char* _refName, char* _previousPath, char* _typeName)
{
	ModelAddTrace* pModAddTraceObj = NULL;
	ModelTrace* pObj = new_ModelTrace();

	/* Allocating memory */
	pModAddTraceObj = (ModelAddTrace*)malloc(sizeof(ModelAddTrace));

	if (pModAddTraceObj == NULL)
	{
		return NULL;
	}

	/*pObj->pDerivedObj = pInstanceObj; Pointing to derived object */
	pModAddTraceObj->super = pObj;
	
	pObj->srcPath = malloc(sizeof(char)*(strlen(_srcPath)+1));
	strcpy(pObj->srcPath, _srcPath);
	pObj->refName = malloc(sizeof(char)*(strlen(_refName)+1));
	strcpy(pObj->refName, _refName);
	pModAddTraceObj->previousPath = malloc(sizeof(char)*(strlen(_previousPath)+1));
	strcpy(pModAddTraceObj->previousPath, _previousPath);
	pModAddTraceObj->typeName = malloc(sizeof(char)*(strlen(_typeName)+1));
	strcpy(pModAddTraceObj->typeName, _typeName);
	
	pModAddTraceObj->Delete = delete_ModelAddTrace;
	pModAddTraceObj->ToString = ModelAddTrace_ToString;
	pObj->ToString = ModelAddTrace_ToString;

	return pModAddTraceObj;
}

void deletePoly_ModelAddTrace(ModelTrace* const this)
{
	ModelAddTrace* pModAddTraceObj;
	pModAddTraceObj = this->pDerivedObj;
	/*destroy derived obj*/
	free(pModAddTraceObj->previousPath);
	free(pModAddTraceObj->typeName);
	/*destroy base Obj*/
	delete_ModelTrace(this);
}

void delete_ModelAddTrace(ModelAddTrace* const this)
{
	/* destroy base object */
	delete_ModelTrace(this->super);
	/* destroy data memebers */
	free(this->previousPath);
	free(this->typeName);
	free(this);
}

/*std::string ModelAddTrace::toString ()*/
char* ModelAddTrace_ToString(ModelAddTrace* const this)
{
	/*string buffer = "";*/
	char* buffer = NULL;

	/*buffer.append ("{ \"traceType\" : " + ActionType (ADD) + " , \"src\" : \"" + srcPath + "\", \"refname\" : \"" + refName + "\"");*/
	sprintf(buffer, "{ \"traceType\" : %s, \"src\" : \"%s\", \"refname\" : \"%s\"", ActionType(ADD), this->super->srcPath, this->super->refName);
	
	/*if (!previousPath.empty ())*/
	if(this->previousPath != NULL)
	{
		/*buffer.append (", \"previouspath\" : \"" + previousPath + "\"");*/
		sprintf(buffer, "%s, \"previouspath\" : \"%s\"", buffer, this->previousPath);
	}
	/*if (!typeName.empty ())*/
	if(this->typeName != NULL)
	{
		/*buffer.append (", \"typename\" : \"" + typeName + "\"");*/
		sprintf(buffer, "%s, \"typename\" : \"%s\"", buffer, this->typeName);
	}
	/*buffer.append ("}\n");*/
	sprintf(buffer, "%s }\n", buffer);
	return buffer;
}


/*ModelAddAllTrace::~ModelAddAllTrace(){

}

ModelAddAllTrace::ModelAddAllTrace (string _srcPath, string _refName,list < string > &_previousPath, list < string > &_typeName)
{
	srcPath = _srcPath;
	refName = _refName;
	previousPath = _previousPath;
	typeName = _typeName;

}

std::string ModelAddAllTrace::toString ()
{
	string buffer;
	buffer.append ("{ \"traceType\" : " + ActionType (ADD_ALL) +
			" , \"src\" : \"" + srcPath + "\", \"refname\" : \"" +
			refName + "\"");
	if (!previousPath.empty ())
	{
		buffer.append (", \"previouspath\" : \"" + mkString (previousPath) +
				"\"");
	}
	if (typeName.empty ())
	{
		buffer.append (", \"typename\" : \"" + mkString (typeName) + "\"");
	}
	buffer.append ("}\n");
	return buffer;
}

std::string ModelAddAllTrace::mkString (list < string > ss){

	string buffer;
	if (!ss.empty ())
	{
		return buffer;
	}
	std::list < std::string >::iterator it;
	bool isFirst = true;
	for (it = ss.begin (); it != ss.end (); ++it)
	{
		if (!isFirst)
		{
			buffer.append (",");
		}
		buffer.append (*it);
		isFirst = false;
	}
	return buffer;
}*/

/*ModelRemoveTrace::ModelRemoveTrace (string _srcPath, string _refName, string _objPath)
{
	srcPath = _srcPath;
	refName = _refName;
	objPath = _objPath;
}*/

ModelTrace* newPoly_ModelRemoveTrace(char* _srcPath, char* _refName, char* _objPath)
{
	ModelRemoveTrace* pModRemTraceObj = NULL;
	ModelTrace* pObj = new_ModelTrace();

	/* Allocating memory */
	pModRemTraceObj = (ModelRemoveTrace*)malloc(sizeof(ModelRemoveTrace));

	if (pModRemTraceObj == NULL)
	{
		pObj->Delete(pObj);
		return NULL;
	}

	pObj->pDerivedObj = pModRemTraceObj; /* Pointing to derived object */
	
	pObj->srcPath = malloc(sizeof(char)*(strlen(_srcPath)+1));
	strcpy(pObj->srcPath, _srcPath);
	pObj->refName = malloc(sizeof(char)*(strlen(_refName)+1));
	strcpy(pObj->refName, _refName);
	pModRemTraceObj->objPath = malloc(sizeof(char)*(strlen(_objPath)+1));
	strcpy(pModRemTraceObj->objPath, _objPath);
	
	pModRemTraceObj->Delete = delete_ModelRemoveTrace;
	pModRemTraceObj->ToString = ModelRemoveTrace_ToString;
	pObj->ToString = ModelRemoveTrace_ToString;

	return pObj;
}

/*ModelRemoveTrace::~ModelRemoveTrace(){

}*/

void deletePoly_ModelRemoveTrace(ModelTrace* const this)
{
	ModelSetTrace* pModSetTraceObj;
	pModSetTraceObj = this->pDerivedObj;
	/*destroy derived obj*/
	free(pModSetTraceObj->objPath);
	/*destroy base Obj*/
	delete_ModelTrace(this);
}

void delete_ModelRemoveTrace(ModelRemoveTrace* const this)
{
	/* destroy base object */
	delete_ModelTrace(this->super);
	/* destroy data memebers */
	free(this->objPath);
	free(this);
}

ModelRemoveTrace* new_ModelRemoveTrace(char* _srcPath, char* _refName, char* _objPath)
{
	ModelRemoveTrace* pModRemTraceObj = NULL;
	ModelTrace* pObj = new_ModelTrace();

	pModRemTraceObj = (ModelRemoveTrace*)malloc(sizeof(ModelRemoveTrace));

	if (pModRemTraceObj == NULL)
	{
		return NULL;
	}

	/*pObj->pDerivedObj = pInstanceObj; Pointing to derived object */
	pModRemTraceObj->super = pObj;

	pObj->srcPath = malloc(sizeof(char)*(strlen(_srcPath)+1));
	strcpy(pObj->srcPath, _srcPath);
	pObj->refName = malloc(sizeof(char)*(strlen(_refName)+1));
	strcpy(pObj->refName, _refName);
	pModRemTraceObj->objPath = malloc(sizeof(char)*(strlen(_objPath)+1));
	strcpy(pModRemTraceObj->objPath, _objPath);

	pModRemTraceObj->Delete = delete_ModelRemoveTrace;
	pModRemTraceObj->ToString = ModelRemoveTrace_ToString;
	pObj->ToString = ModelRemoveTrace_ToString;

	return pModRemTraceObj;
}

/*std::string ModelRemoveTrace::toString ()*/
char* ModelRemoveTrace_ToString(ModelRemoveTrace* const this)
{
	/*return "{ \"traceType\" : " + ActionType (REMOVE) + " , \"src\" : \"" + srcPath + "\", \"refname\" : \"" + refName + "\", \"objpath\" : \"" + objPath + "\" }\n";*/
	char* buffer = NULL;

	sprintf("{ \"traceType\" : %s , \"src\" : \" %s \", \"refname\" : \" %s \", \"objpath\" : \" %s \" }\n", ActionType(REMOVE), this->super->srcPath, this->objPath, this->objPath);
}