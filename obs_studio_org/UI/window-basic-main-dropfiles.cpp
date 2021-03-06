//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   16时:35分:37秒  
// ><文件     ><  window-basic-main-dropfiles.cpp  
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
// ><修改日期 ><  2017年:03月:21日   16时:35分:37秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <string>

#include "window-basic-main.hpp"
#include "qt-wrappers.hpp"

using namespace std;

static const char *textExtensions[] = {
	"txt", "log", nullptr
};

static const char *imageExtensions[] = {
	"bmp", "tga", "png", "jpg", "jpeg", "gif", nullptr
};

static const char *mediaExtensions[] = {
	"3ga", "669", "a52", "aac", "ac3", "adt", "adts", "aif", "aifc",
	"aiff", "amb", "amr", "aob", "ape", "au", "awb", "caf", "dts",
	"flac", "it", "kar", "m4a", "m4b", "m4p", "m5p", "mid", "mka",
	"mlp", "mod", "mpa", "mp1", "mp2", "mp3", "mpc", "mpga", "mus",
	"oga", "ogg", "oma", "opus", "qcp", "ra", "rmi", "s3m", "sid",
	"spx", "tak", "thd", "tta", "voc", "vqf", "w64", "wav", "wma",
	"wv", "xa", "xm" "3g2", "3gp", "3gp2", "3gpp", "amv", "asf", "avi",
	"bik", "crf", "divx", "drc", "dv", "evo", "f4v", "flv", "gvi",
	"gxf", "iso", "m1v", "m2v", "m2t", "m2ts", "m4v", "mkv", "mov",
	"mp2", "mp2v", "mp4", "mp4v", "mpe", "mpeg", "mpeg1", "mpeg2",
	"mpeg4", "mpg", "mpv2", "mts", "mtv", "mxf", "mxg", "nsv", "nuv",
	"ogg", "ogm", "ogv", "ogx", "ps", "rec", "rm", "rmvb", "rpl", "thp",
	"tod", "ts", "tts", "txd", "vob", "vro", "webm", "wm", "wmv", "wtv",
	nullptr
};

static string GenerateSourceName(const char *base)
{
	string name;
	int inc = 0;

	for (;; inc++) {
		name = base;

		if (inc) {
			name += " (";
			name += to_string(inc+1);
			name += ")";
		}

		obs_source_t *source = obs_get_source_by_name(name.c_str());
		if (!source)
			return name;
	}
}

void OBSBasic::AddDropSource(const char *data, DropType image)
{
	OBSBasic *main = reinterpret_cast<OBSBasic*>(App()->GetMainWindow());
	obs_data_t *settings = obs_data_create();
	obs_source_t *source = nullptr;
	const char *type = nullptr;

	switch (image) {
	case DropType_RawText:
		obs_data_set_string(settings, "text", data);
		type = "text_gdiplus";
		break;
	case DropType_Text:
		obs_data_set_bool(settings, "read_from_file", true);
		obs_data_set_string(settings, "file", data);
		type = "text_gdiplus";
		break;
	case DropType_Image:
		obs_data_set_string(settings, "file", data);
		type = "image_source";
		break;
	case DropType_Media:
		obs_data_set_string(settings, "local_file", data);
		type = "ffmpeg_source";
		break;
	}

	const char *name = obs_source_get_display_name(type);
	source = obs_source_create(type, GenerateSourceName(name).c_str(),
			settings, nullptr);
	if (source) {
		OBSScene scene = main->GetCurrentScene();
		obs_scene_add(scene, source);
		obs_source_release(source);
	}

	obs_data_release(settings);
}

void OBSBasic::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void OBSBasic::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void OBSBasic::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void OBSBasic::dropEvent(QDropEvent *event)
{
	const QMimeData* mimeData = event->mimeData();

	if (mimeData->hasUrls()) {
		QList<QUrl> urls = mimeData->urls();

		for (int i = 0; i < urls.size() && i < 5; i++) {
			QString file = urls.at(i).toLocalFile();
			QFileInfo fileInfo(file);

			if (!fileInfo.exists())
				continue;

			QString suffixQStr = fileInfo.suffix();
			QByteArray suffixArray = suffixQStr.toUtf8();
			const char *suffix = suffixArray.constData();
			bool found = false;

			const char **cmp;

			cmp = textExtensions;
			while (*cmp) {
				if (strcmp(*cmp, suffix) == 0) {
					AddDropSource(QT_TO_UTF8(file),
							DropType_Text);
					found = true;
					break;
				}

				cmp++;
			}

			if (found)
				continue;

			cmp = imageExtensions;
			while (*cmp) {
				if (strcmp(*cmp, suffix) == 0) {
					AddDropSource(QT_TO_UTF8(file),
							DropType_Image);
					found = true;
					break;
				}

				cmp++;
			}

			if (found)
				continue;

			cmp = mediaExtensions;
			while (*cmp) {
				if (strcmp(*cmp, suffix) == 0) {
					AddDropSource(QT_TO_UTF8(file),
							DropType_Media);
					break;
				}

				cmp++;
			}
		}
	} else if (mimeData->hasText()) {
		AddDropSource(QT_TO_UTF8(mimeData->text()), DropType_RawText);
	}
}

