#include "stdafx.h"
#include "BaseControllerNew.h"

namespace scope {
	std::once_flag BaseControllerNew::onceFlag;
	std::unique_ptr<BaseControllerNew> BaseControllerNew::instance = nullptr;

	BaseControllerNew& BaseControllerNew::GetInstance() {
		std::call_once(onceFlag, [] {
			instance.reset(new BaseControllerNew());
		});
		return *instance.get();
	}
}
