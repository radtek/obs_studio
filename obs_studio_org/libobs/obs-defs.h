//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   17时:52分:39秒  
// ><文件     ><  obs-defs.h  
// ><文件路径 ><  D:\newSvnCode\OBS\trunk\obs_studio\libobs  
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
// ><修改日期 ><  2017年:03月:21日   17时:52分:39秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
    Copyright (C) 2013-2014 by Hugh Bailey <obs.jim@gmail.com>

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

/** Maximum number of source channels for output and per display */
#define MAX_CHANNELS 64

#define OBS_ALIGN_CENTER (0)
#define OBS_ALIGN_LEFT   (1<<0)
#define OBS_ALIGN_RIGHT  (1<<1)
#define OBS_ALIGN_TOP    (1<<2)
#define OBS_ALIGN_BOTTOM (1<<3)

#define MODULE_SUCCESS             0
#define MODULE_ERROR              -1
#define MODULE_FILE_NOT_FOUND     -2
#define MODULE_MISSING_EXPORTS    -3
#define MODULE_INCOMPATIBLE_VER   -4

#define OBS_OUTPUT_SUCCESS         0
#define OBS_OUTPUT_BAD_PATH       -1
#define OBS_OUTPUT_CONNECT_FAILED -2
#define OBS_OUTPUT_INVALID_STREAM -3
#define OBS_OUTPUT_ERROR          -4
#define OBS_OUTPUT_DISCONNECTED   -5
#define OBS_OUTPUT_UNSUPPORTED    -6
#define OBS_OUTPUT_NO_SPACE       -7

#define OBS_VIDEO_SUCCESS           0
#define OBS_VIDEO_FAIL             -1
#define OBS_VIDEO_NOT_SUPPORTED    -2
#define OBS_VIDEO_INVALID_PARAM    -3
#define OBS_VIDEO_CURRENTLY_ACTIVE -4
#define OBS_VIDEO_MODULE_NOT_FOUND -5
