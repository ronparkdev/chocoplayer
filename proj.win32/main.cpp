#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "cocos2d.h"

#include "COMMON/MgrSetting.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	/*
#if defined( DEBUG ) || defined( _DEBUG )
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG  );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG  );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG  );

	_CrtMemState msPrev;
	_CrtMemCheckpoint(&msPrev);
	//_CrtSetBreakAlloc(64938);
#endif // DEBUG
	*/
	{
		// create the application instance
		AppDelegate app;
		CCEGLView* eglView = CCEGLView::sharedOpenGLView();
		eglView->setViewName("chocoplayer");

#if TRUE
		eglView->setFrameSize(400, 600);
		MgrSetting::getInstance()->setBool(RST_ETC_IS_VERTICAL, true);
#else
		eglView->setFrameSize(1700, 900);
		MgrSetting::getInstance()->setBool(RST_ETC_IS_VERTICAL, false);
#endif
		MgrSetting::getInstance()->SaveAll();

		CCApplication::sharedApplication()->run();
	}
	/*
#if defined( DEBUG ) || defined( _DEBUG )
	_CrtMemDumpAllObjectsSince(&msPrev);
#endif // DEBUG
	*/
	return 0;
}
