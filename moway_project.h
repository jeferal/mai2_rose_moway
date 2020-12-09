
// moway_project.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// Símbolos principales


// CmowayprojectApp:
// Consulte moway_project.cpp para obtener información sobre la implementación de esta clase
//

class CmowayprojectApp : public CWinApp
{
public:
	CmowayprojectApp();

// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CmowayprojectApp theApp;
