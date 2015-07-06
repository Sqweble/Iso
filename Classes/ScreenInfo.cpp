#include "ScreenInfo.h"

USING_NS_CC;

Rect ScreenInfo::s_ScreenInfo;

void ScreenInfo::lazyInit()
{
	// no lazy init
	// Useful if we change the resolution in runtime
	s_ScreenInfo = Director::getInstance()->getOpenGLView()->getVisibleRect();
}

Rect ScreenInfo::getScreenInfo()
{
	lazyInit();
	return s_ScreenInfo;
}

Vec2 ScreenInfo::left()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x, s_ScreenInfo.origin.y + s_ScreenInfo.size.height / 2);
}

Vec2 ScreenInfo::right()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x + s_ScreenInfo.size.width, s_ScreenInfo.origin.y + s_ScreenInfo.size.height / 2);
}

Vec2 ScreenInfo::top()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x + s_ScreenInfo.size.width / 2, s_ScreenInfo.origin.y + s_ScreenInfo.size.height);
}

Vec2 ScreenInfo::bottom()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x + s_ScreenInfo.size.width / 2, s_ScreenInfo.origin.y);
}

Vec2 ScreenInfo::center()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x + s_ScreenInfo.size.width / 2, s_ScreenInfo.origin.y + s_ScreenInfo.size.height / 2);
}

Vec2 ScreenInfo::leftTop()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x, s_ScreenInfo.origin.y + s_ScreenInfo.size.height);
}

Vec2 ScreenInfo::rightTop()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x + s_ScreenInfo.size.width, s_ScreenInfo.origin.y + s_ScreenInfo.size.height);
}

Vec2 ScreenInfo::leftBottom()
{
	lazyInit();
	return s_ScreenInfo.origin;
}

Vec2 ScreenInfo::rightBottom()
{
	lazyInit();
	return Vec2(s_ScreenInfo.origin.x + s_ScreenInfo.size.width, s_ScreenInfo.origin.y);
}