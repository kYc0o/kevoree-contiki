#ifndef __PortType_H
#define __PortType_H

#include <stdbool.h>

typedef struct _PortType PortType;
typedef struct _TypeDefinition TypeDefinition;
typedef struct _Visitor Visitor;

typedef char* (*fptrPortTypeMetaClassName)(PortType*);
typedef char* (*fptrPortTypeInternalGetKey)(PortType*);
typedef void (*fptrDeletePortType)(PortType*);
typedef void (*fptrVisitAttrPortType)(void*, char*, Visitor*, bool);
typedef void (*fptrVisitRefsPortType)(void*, char*, Visitor*);
typedef void* (*fptrFindByPathPortType)(char*, TypeDefinition*);

typedef struct _PortType {
	void* pDerivedObj;
	TypeDefinition* super;
	bool synchrone;
	fptrPortTypeMetaClassName MetaClassName;
	fptrPortTypeInternalGetKey InternalGetKey;
	fptrDeletePortType Delete;
	fptrVisitAttrPortType VisitAttributes;
	fptrVisitRefsPortType VisitReferences;
	fptrFindByPathPortType FindByPath;
} PortType;

TypeDefinition* newPoly_PortType(void);
PortType* new_PortType(void);
char* PortType_MetaClassName(PortType* const this);
char* PortType_InternalGetKey(PortType* const this);
void deletePoly_PortType(TypeDefinition* const this);
void delete_PortType(PortType* const this);
void PortType_VisitAttributes(void* const this, char* parent, Visitor* visitor, bool recursive);
void* PortType_FindByPath(char* attribute, TypeDefinition* const this);

#endif /*__PortType_H */