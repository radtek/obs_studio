//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   17时:43分:33秒  
// ><文件     ><  axisang.c  
// ><文件路径 ><  D:\newSvnCode\OBS\trunk\obs_studio\libobs\graphics  
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
// ><修改日期 ><  2017年:03月:21日   17时:43分:33秒  
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

#include "axisang.h"
#include "quat.h"

void axisang_from_quat(struct axisang *dst, const struct quat *q)
{
	float len, leni;

	len = q->x*q->x + q->y*q->y + q->z*q->z;
	if (!close_float(len, 0.0f, EPSILON)) {
		leni = 1.0f/sqrtf(len);
		dst->x = q->x * leni;
		dst->y = q->y * leni;
		dst->z = q->z * leni;
		dst->w  = acosf(q->w)*2.0f;
	} else {
		dst->x = 0.0f;
		dst->y = 0.0f;
		dst->z = 0.0f;
		dst->w = 0.0f;
	}
}
