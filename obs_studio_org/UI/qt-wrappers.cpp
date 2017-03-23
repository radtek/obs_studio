//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   16时:33分:37秒  
// ><文件     ><  qt-wrappers.cpp  
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
// ><修改日期 ><  2017年:03月:21日   16时:33分:37秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
    Copyright (C) 2013 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "qt-wrappers.hpp"
#include <graphics/graphics.h>
#include <QWidget>
#include <QLayout>
#include <QMessageBox>
#include <QDataStream>

#if !defined(_WIN32) && !defined(__APPLE__)
#include <QX11Info>
#endif

static inline void OBSErrorBoxva(QWidget *parent, const char *msg, va_list args)
{
	char full_message[4096];
	vsnprintf(full_message, 4095, msg, args);

	QMessageBox::critical(parent, "Error", full_message);
}

void OBSErrorBox(QWidget *parent, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	OBSErrorBoxva(parent, msg, args);
	va_end(args);
}

void QTToGSWindow(WId windowId, gs_window &gswindow)
{
#ifdef _WIN32
	gswindow.hwnd = (HWND)windowId;
#elif __APPLE__
	gswindow.view = (id)windowId;
#else
	gswindow.id = windowId;
	gswindow.display = QX11Info::display();
#endif
}

uint32_t TranslateQtKeyboardEventModifiers(Qt::KeyboardModifiers mods)
{
	int obsModifiers = INTERACT_NONE;

	if (mods.testFlag(Qt::ShiftModifier))
		obsModifiers |= INTERACT_SHIFT_KEY;
	if (mods.testFlag(Qt::AltModifier))
		obsModifiers |= INTERACT_ALT_KEY;
#ifdef __APPLE__
	// Mac: Meta = Control, Control = Command
	if (mods.testFlag(Qt::ControlModifier))
		obsModifiers |= INTERACT_COMMAND_KEY;
	if (mods.testFlag(Qt::MetaModifier))
		obsModifiers |= INTERACT_CONTROL_KEY;
#else
	// Handle windows key? Can a browser even trap that key?
	if (mods.testFlag(Qt::ControlModifier))
		obsModifiers |= INTERACT_CONTROL_KEY;
	if (mods.testFlag(Qt::MetaModifier))
		obsModifiers |= INTERACT_COMMAND_KEY;

#endif

	return obsModifiers;
}

QDataStream &operator<<(QDataStream &out,
		const std::vector<std::shared_ptr<OBSSignal>> &)
{
	return out;
}

QDataStream &operator>>(QDataStream &in,
		std::vector<std::shared_ptr<OBSSignal>> &)
{
	return in;
}

QDataStream &operator<<(QDataStream &out, const OBSScene &scene)
{
	return out << QString(obs_source_get_name(obs_scene_get_source(scene)));
}

QDataStream &operator>>(QDataStream &in, OBSScene &scene)
{
	QString sceneName;

	in >> sceneName;

	obs_source_t *source = obs_get_source_by_name(QT_TO_UTF8(sceneName));
	scene = obs_scene_from_source(source);

	return in;
}

QDataStream &operator<<(QDataStream &out, const OBSSceneItem &si)
{
	obs_scene_t  *scene  = obs_sceneitem_get_scene(si);
	obs_source_t *source = obs_sceneitem_get_source(si);
	return out << QString(obs_source_get_name(obs_scene_get_source(scene)))
		   << QString(obs_source_get_name(source));
}

QDataStream &operator>>(QDataStream &in, OBSSceneItem &si)
{
	QString sceneName;
	QString sourceName;

	in >> sceneName >> sourceName;

	obs_source_t *sceneSource =
		obs_get_source_by_name(QT_TO_UTF8(sceneName));

	obs_scene_t *scene = obs_scene_from_source(sceneSource);
	si = obs_scene_find_source(scene, QT_TO_UTF8(sourceName));

	obs_source_release(sceneSource);

	return in;
}

void DeleteLayout(QLayout *layout)
{
	if (!layout)
		return;

	for (;;) {
		QLayoutItem *item = layout->takeAt(0);
		if (!item)
			break;

		QLayout *subLayout = item->layout();
		if (subLayout) {
			DeleteLayout(subLayout);
		} else {
			delete item->widget();
			delete item;
		}
	}

	delete layout;
}
