#include "DplStl.h"

DplStlRuntime::~DplStlRuntime() {
}

void DplStlRuntime::setConnector(DustRuntimeConnector* pConn) {
}

DustEntity DplStlRuntime::getMetaUnit(const char* name) {
	return DUST_ENTITY_INVALID;
}

DustEntity DplStlRuntime::getMetaEntity(DustEntity primaryType, const char* name, DustEntity parent, DustEntity constId) {
	return DUST_ENTITY_INVALID;
}

DustEntity DplStlRuntime::createEntity(DustEntity primaryType) {
	return DUST_ENTITY_INVALID;
}

int DplStlRuntime::getInt(DustEntity entity, DustEntity token, int defValue) {
	return 0;
}

double DplStlRuntime::getDouble(DustEntity entity, DustEntity token, double defValue) {
	return 0;
}

void DplStlRuntime::setInt(DustEntity entity, DustEntity token, int val) {
}

void DplStlRuntime::setDouble(DustEntity entity, DustEntity token, double val) {
}


unsigned int DplStlRuntime::getRefCount(DustEntity entity, DustEntity token) {
	return 0;
}

DustEntity DplStlRuntime::getRefKey(DustEntity entity, DustEntity token, int idx) {
	return DUST_ENTITY_INVALID;
}

DustEntity DplStlRuntime::getRef(DustEntity entity, DustEntity token, int key) {
	return DUST_ENTITY_INVALID;
}

bool DplStlRuntime::setRef(DustEntity entity, DustEntity token, DustEntity target, int key) {
	return false;
}

void* DplStlRuntime::getNative(DustEntity entity, DustEntity type) {
	return 0;
}

DustProcessResult DplStlRuntime::DustResourceInit() {
	return DUST_PROCESS_ACCEPT;
}
DustProcessResult DplStlRuntime::DustResourceRelease() {
	return DUST_PROCESS_ACCEPT;
}

DustProcessResult DplStlRuntime::DustActionExecute() {
	return DUST_PROCESS_NOTIMPLEMENTED;
}

