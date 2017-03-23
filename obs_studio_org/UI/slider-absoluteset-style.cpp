//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   16时:33分:52秒  
// ><文件     ><  slider-absoluteset-style.cpp  
// ><文件路径 ><  D:\newSvnCode\OBS\trunk\obs_studio\UI  
// ><隶属工程><   obs-studio  
// ><当前用户 ><  Administrator  
// ><作者     ><  Open Broadcaster Software   
// ><出处     >< 《 https://obsproject.com/ 》  
// ><目的     >< 【】  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:03月:21日   16时:33分:52秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "slider-absoluteset-style.hpp"

SliderAbsoluteSetStyle::SliderAbsoluteSetStyle(const QString& baseStyle)
	:QProxyStyle(baseStyle)
{
}
SliderAbsoluteSetStyle::SliderAbsoluteSetStyle(QStyle* baseStyle)
	:QProxyStyle(baseStyle)
{
}

int SliderAbsoluteSetStyle::styleHint(QStyle::StyleHint hint,
	const QStyleOption* option = 0, const QWidget* widget = 0,
	QStyleHintReturn* returnData = 0) const
{
	if(hint == QStyle::SH_Slider_AbsoluteSetButtons)
		return (Qt::LeftButton | Qt::MidButton);
	return QProxyStyle::styleHint(hint, option, widget, returnData);
}
