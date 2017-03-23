//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   17时:51分:33秒  
// ><文件     ><  ComPtr.hpp  
// ><文件路径 ><  D:\newSvnCode\OBS\trunk\obs_studio\libobs\util\windows  
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
// ><修改日期 ><  2017年:03月:21日   17时:51分:33秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Copyright (c) 2013 Hugh Bailey <obs.jim@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

/* Oh no I have my own com pointer class, the world is ending, how dare you
 * write your own! */

template<class T> class ComPtr {

protected:
	T *ptr;

	inline void Kill()
	{
		if (ptr)
			ptr->Release();
	}

	inline void Replace(T *p)
	{
		if (ptr != p) {
			if (p)   p->AddRef();
			if (ptr) ptr->Release();
			ptr = p;
		}
	}

public:
	inline ComPtr() : ptr(nullptr)                 {}
	inline ComPtr(T *p) : ptr(p)                   {if (ptr) ptr->AddRef();}
	inline ComPtr(const ComPtr<T> &c) : ptr(c.ptr) {if (ptr) ptr->AddRef();}
	inline ComPtr(ComPtr<T> &&c) : ptr(c.ptr)      {c.ptr = nullptr;}
	inline ~ComPtr()                               {Kill();}

	inline void Clear()
	{
		if (ptr) {
			ptr->Release();
			ptr = nullptr;
		}
	}

	inline ComPtr<T> &operator=(T *p)
	{
		Replace(p);
		return *this;
	}

	inline ComPtr<T> &operator=(const ComPtr<T> &c)
	{
		Replace(c.ptr);
		return *this;
	}

	inline ComPtr<T> &operator=(ComPtr<T> &&c)
	{
		if (this != &c) {
			Kill();
			ptr = c.ptr;
			c.ptr = nullptr;
		}

		return *this;
	}

	inline T *Detach()
	{
		T *out = ptr;
		ptr = nullptr;
		return out;
	}

	inline void CopyTo(T **out)
	{
		if (out) {
			if (ptr) ptr->AddRef();
			*out = ptr;
		}
	}

	inline ULONG Release()
	{
		ULONG ref;

		if (!ptr) return 0;
		ref = ptr->Release();
		ptr = nullptr;
		return ref;
	}

	inline T **Assign()                {Clear(); return &ptr;}
	inline void Set(T *p)              {Kill(); ptr = p;}

	inline T *Get() const              {return ptr;}

	inline T **operator&()             {return Assign();}

	inline    operator T*() const      {return ptr;}
	inline T *operator->() const       {return ptr;}

	inline bool operator==(T *p) const {return ptr == p;}
	inline bool operator!=(T *p) const {return ptr != p;}

	inline bool operator!() const      {return !ptr;}
};

#ifdef _WIN32

template<class T> class ComQIPtr : public ComPtr<T> {

public:
	inline ComQIPtr(IUnknown *unk)
	{
		this->ptr = nullptr;
		unk->QueryInterface(__uuidof(T), (void**)&this->ptr);
	}

	inline ComPtr<T> &operator=(IUnknown *unk)
	{
		ComPtr<T>::Clear();
		unk->QueryInterface(__uuidof(T), (void**)&this->ptr);
		return *this;
	}
};

#endif
