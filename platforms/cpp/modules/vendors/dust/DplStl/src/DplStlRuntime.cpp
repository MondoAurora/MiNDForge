#include "DplStl.h"

#include <iostream>

using namespace std;

DplStlRuntime::DplStlRuntime()
    :store(DUST_LAST_CONST_RUNTIME)
{
}

DplStlRuntime::~DplStlRuntime()
{
}

void DplStlRuntime::setConnector(DustRuntimeConnector* pConn)
{
    pRTC = pConn;
}

map<long, DustToken*> bootEntites;
typedef map<long, DustToken*>::iterator BootIterator;

void setBootInfo(DustToken &token, long id)
{
    bootEntites[id] = &token;
}


DustResultType DplStlRuntime::DustResourceInit()
{
    cout << "Booting runtime" << endl;

    setBootInfo(DustUnitMindText::DustTypePlainText, DUST_BOOT_TYPE_PLAINTEXT);

    setBootInfo(DustUnitMindModel::DustTypeEntity, DUST_BOOT_TYPE_ENTITY);
    setBootInfo(DustUnitMindModel::DustRefUnit, DUST_BOOT_REF_UNIT);
    setBootInfo(DustUnitMindModel::DustIntId, DUST_BOOT_INT_ID);
    setBootInfo(DustUnitMindModel::DustRefPrimaryType, DUST_BOOT_REF_PRIMARYTYPE);
    setBootInfo(DustUnitMindModel::DustRefGlobalId, DUST_BOOT_REF_GLOBALID);
    setBootInfo(DustUnitMindModel::DustRefOwner, DUST_BOOT_REF_OWNER);
    setBootInfo(DustUnitMindModel::DustRefTags, DUST_BOOT_REF_TAGS);

    setBootInfo(DustUnitMindIdea::DustTypeType, DUST_IDEA_TYPE);
    setBootInfo(DustUnitMindIdea::DustTypeMember, DUST_IDEA_MEMBER);
    setBootInfo(DustUnitMindIdea::DustTypeAgent, DUST_IDEA_AGENT);
    setBootInfo(DustUnitMindIdea::DustTypeTag, DUST_IDEA_TAG);

    setBootInfo(DustUnitMindNative::DustTypeConstant, DUST_NATIVE_CONSTANT);
    setBootInfo(DustUnitMindNative::DustTypeService, DUST_NATIVE_SERVICE);
    setBootInfo(DustUnitMindNative::DustTypeCommand, DUST_NATIVE_COMMAND);

    setBootInfo(DustUnitMindIdea::DustTagValInteger, DUST_VAL_INTEGER);
    setBootInfo(DustUnitMindIdea::DustTagValReal, DUST_VAL_REAL);
    setBootInfo(DustUnitMindIdea::DustTagValRef, DUST_VAL_REF);

    setBootInfo(DustUnitMindIdea::DustTagCollSingle, DUST_COLL_SINGLE);
    setBootInfo(DustUnitMindIdea::DustTagCollSet, DUST_COLL_SET);
    setBootInfo(DustUnitMindIdea::DustTagCollArr, DUST_COLL_ARR);
    setBootInfo(DustUnitMindIdea::DustTagCollMap, DUST_COLL_MAP);

    setBootInfo(DustUnitMindNarrative::DustTagResultReject, DUST_RESULT_REJECT);
    setBootInfo(DustUnitMindNarrative::DustTagResultRead, DUST_RESULT_READ);
    setBootInfo(DustUnitMindNarrative::DustTagResultAccept, DUST_RESULT_ACCEPT);
    setBootInfo(DustUnitMindNarrative::DustTagResultAcceptPass, DUST_RESULT_ACCEPT_PASS);
    setBootInfo(DustUnitMindNarrative::DustTagResultAcceptRead, DUST_RESULT_ACCEPT_READ);

    setBootInfo(DustUnitMindDialog::DustTagAccessGet, DUST_ACCESS_GET);
    setBootInfo(DustUnitMindDialog::DustTagAccessSet, DUST_ACCESS_SET);
    setBootInfo(DustUnitMindDialog::DustTagAccessMove, DUST_ACCESS_MOVE);
    setBootInfo(DustUnitMindDialog::DustTagAccessRemove, DUST_ACCESS_REMOVE);
    setBootInfo(DustUnitMindDialog::DustTagAccessClear, DUST_ACCESS_CLEAR);

    for (BootIterator it = bootEntites.begin(); it != bootEntites.end(); ++it)
    {
        DustToken* pT = it->second;
        if ( DUST_IDEA_MEMBER == pT->getPrimaryType() )
        {
            store.tokenInfo[it->first] = new DplStlTokenInfo(pT->getValType(), pT->getCollType());
        }
    }

    for (BootIterator it = bootEntites.begin(); it != bootEntites.end(); ++it)
    {
        setBootToken( *(it->second), it->first);
    }

    bootEntites.clear();

    return DUST_RESULT_ACCEPT;
}

DustResultType DplStlRuntime::DustResourceRelease()
{
    return DUST_RESULT_ACCEPT;
}

DustEntity DplStlRuntime::getTextToken(DustEntity parent,  const char* name)
{
    DustEntity txtParent = parent ? DustData::getRef(parent, DUST_BOOT_REF_GLOBALID) : DUST_ENTITY_INVALID;
    return pRTC->getTextToken(txtParent, name);
}

void DplStlRuntime::optSetParent(DustAccessData &ad, DplStlDataEntity* pEntity, DustEntity parent)
{
    if ( parent )
    {
        ad.updateLong(parent, DUST_BOOT_REF_OWNER);
        pEntity->access(ad);

        for ( DustEntity p = parent; p; p = DustData::getRef(p, DUST_BOOT_REF_OWNER) )
        {
            DustEntity unit = DustData::getRef(p, DUST_BOOT_REF_UNIT);
            if ( unit )
            {
                ad.updateLong(unit, DUST_BOOT_REF_UNIT);
                pEntity->access(ad);
                break;
            }
        }
    }
}

DplStlDataEntity* DplStlRuntime::registerGlobalEntity(DustEntity txtToken, DustEntity primaryType, DustEntity parent, DustEntity constId)
{
    DplStlDataEntity* pEntity = store.getEntity(constId, primaryType);
    DustEntity entity = pEntity->id;

    globalEntites[txtToken] = entity;
    DustAccessData ad(entity, DUST_BOOT_REF_GLOBALID, txtToken);
    pEntity->access(ad);

    optSetParent(ad, pEntity, parent);

    return pEntity;
}

DustEntity DplStlRuntime::getUnit(const char* name, DustEntity constId)
{
    DustEntity txtToken = getTextToken(0, name);
    DustEntity unit = findEntity(globalEntites, txtToken);

    if ( !unit )
    {
        DplStlDataEntity* pEntity = registerGlobalEntity(txtToken, DUST_IDEA_UNIT, DUST_ENTITY_INVALID, constId);
        unit = pEntity->id;
        DustAccessData ad(unit, DUST_BOOT_REF_UNIT, unit);
        pEntity->access(ad);
    }

    return unit;
}

DustEntity DplStlRuntime::getTokenEntity(DustEntity parent, const char* name, DustEntity primaryType, DustEntity constId)
{
    DustEntity txtToken = getTextToken(parent, name);
    DustEntity idea = findEntity(globalEntites, txtToken);

    if ( !idea )
    {
        DplStlDataEntity* pEntity = registerGlobalEntity(txtToken, primaryType, parent, constId);
        idea = pEntity->id;
    }

    return idea;
}

DustEntity DplStlRuntime::getMemberEntity(DustEntity type, const char* name, DustValType valType, DustCollType collType, DustEntity constId)
{
    DustEntity txtToken = getTextToken(type, name);
    DustEntity member = findEntity(globalEntites, txtToken);

    if ( !member )
    {
        DplStlDataEntity* pEntity = registerGlobalEntity(txtToken, DUST_IDEA_MEMBER, type, constId);
        member = pEntity->id;

        DustAccessData ad(member, DUST_BOOT_REF_TAGS, (long) valType);
        pEntity->access(ad);

        ad.updateLong(collType);
        pEntity->access(ad);
    }

    return member;
}


DplStlDataEntity* DplStlRuntime::resolveEntity(DustEntity entity)
{
    return store.getEntity(entity);
}


DustEntity DplStlRuntime::createEntity(DustEntity primaryType)
{
    return store.getEntity(DUST_ENTITY_APPEND, primaryType)->id;
}

long DplStlRuntime::getMemberCount(DustEntity entity, DustEntity token)
{
    long c = 0;

    DplStlDataEntity *pEntity = resolveEntity(entity);
    if ( pEntity )
    {
        DplStlDataVariant *pVar = pEntity->getVariant(token, false);
        if ( pVar )
        {
            c = pVar->pColl ? pVar->pColl->size() : 1;
        }
    }

    return c;
}

DustEntity DplStlRuntime::getMemberKey(DustEntity entity, DustEntity token, long idx)
{
    DustEntity key = DUST_ENTITY_INVALID;

    if ( 0 <= idx )
    {
        DplStlDataEntity *pEntity = resolveEntity(entity);
        if ( pEntity )
        {
            DplStlDataVariant *pVar = pEntity->getVariant(token, false);
            if ( pVar && pVar->pColl && ( (unsigned) idx < pVar->pColl->size()))
            {
                key = (DUST_COLL_MAP == pVar->pTokenInfo->collType) ? pVar->pColl->at(idx)->key : idx;
            }
        }
    }

    return key;
}

bool DplStlRuntime::accessMember(DustAccessData &ad)
{
    DplStlDataEntity *pEntity = resolveEntity(ad.entity);
    return pEntity ? pEntity->access(ad) : false;
}



void* DplStlRuntime::getNative(DustEntity entity, DustEntity type)
{
    DplStlDataEntity *pEntity = resolveEntity(entity);
    void* ret = mapOptGet(pEntity->native, type);

    if ( !ret )
    {
        DustModule *pMod = pRTC->getModuleForType(type);

        if ( pMod )
        {
            ret = pMod->createNative(type);
            if ( ret )
            {
                pEntity->native[type] = ret;
            }
        }
    }

    return ret;
}

DustResultType DplStlRuntime::DustActionExecute()
{
    return DUST_RESULT_NOTIMPLEMENTED;
}

