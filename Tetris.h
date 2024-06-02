
// Tetris.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTetrisApp:
// Сведения о реализации этого класса: Tetris.cpp
//

class CTetrisApp : public CWinApp
{
public:
	CTetrisApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTetrisApp theApp;
