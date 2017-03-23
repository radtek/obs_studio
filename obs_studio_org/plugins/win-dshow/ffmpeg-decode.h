//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   16时:47分:22秒  
// ><文件     ><  ffmpeg-decode.h  
// ><文件路径 ><  D:\newSvnCode\OBS\trunk\obs_studio\plugins\win-dshow  
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
// ><修改日期 ><  2017年:03月:21日   16时:47分:22秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
    Copyright (C) 2014 by Hugh Bailey <obs.jim@gmail.com>

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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <obs.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4204)
#endif

#include <libavcodec/avcodec.h>
#include <libavutil/log.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

struct ffmpeg_decode {
	AVCodecContext *decoder;
	AVCodec        *codec;

	AVFrame        *frame;

	uint8_t        *packet_buffer;
	size_t         packet_size;
};

extern int ffmpeg_decode_init(struct ffmpeg_decode *decode, enum AVCodecID id);
extern void ffmpeg_decode_free(struct ffmpeg_decode *decode);

extern int ffmpeg_decode_audio(struct ffmpeg_decode *decode,
		uint8_t *data, size_t size,
		struct obs_source_audio *audio,
		bool *got_output);

extern int ffmpeg_decode_video(struct ffmpeg_decode *decode,
		uint8_t *data, size_t size, long long *ts,
		struct obs_source_frame *frame,
		bool *got_output);

static inline bool ffmpeg_decode_valid(struct ffmpeg_decode *decode)
{
	return decode->decoder != NULL;
}

#ifdef __cplusplus
}
#endif
