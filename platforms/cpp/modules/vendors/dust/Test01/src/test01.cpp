#include <iostream>
#include <string>

#include <DustModule.h>

using namespace std;

class TestModule : public DustModule {
public:
	virtual ~TestModule() {
	}

	virtual DustProcessResult DustResourceInit() {
		cout << "TestModule::DustResourceInit" << endl;
		return DUST_PROCESS_ACCEPT;
	}
	virtual DustProcessResult DustResourceRelease() {
		cout << "TestModule::DustResourceRelease" << endl;
		return DUST_PROCESS_ACCEPT;
	}

	virtual void* createNative(int typeId) const {
		return 0;
	}
	virtual DustProcessResult dispatchCommand(int logicId, DustNativeLogic* pLogic, DustEntity cmd, DustEntity param = DUST_ENTITY_INVALID) const {
		return DUST_PROCESS_NOTIMPLEMENTED;
	}
	virtual void releaseNative(int typeId, void* pNativeObject) const {
	}
};

TestModule module;

extern "C" DustModule* getModule() {
	return &module;
}
