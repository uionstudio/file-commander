#include "cfilecommanderplugin.h"
#include "cpluginproxy.h"
#include "assert/advanced_assert.h"

DISABLE_COMPILER_WARNINGS
#include <QDebug>
RESTORE_COMPILER_WARNINGS

CFileCommanderPlugin::CFileCommanderPlugin()
{
	AdvancedAssert::setLoggingFunc([](const char* message){
		qDebug() << message;
	});
}

void CFileCommanderPlugin::setProxy(CPluginProxy *proxy)
{
	assert_r(proxy);
	_proxy = proxy;
	proxySet();
}

void CFileCommanderPlugin::proxySet()
{
}
