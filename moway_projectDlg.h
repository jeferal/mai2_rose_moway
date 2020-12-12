
// moway_projectDlg.h: archivo de encabezado
//

#pragma once

#define ID_MOWAY 18
#define FRONTANGMAX 215
#define FRONTANGMIN 135
#define LATANGMAX 60
#define LATANGMIN 30

#define TIMER_1 1
#define SAMPLE_TIME_1 250


// Cuadro de diálogo de CmowayprojectDlg
class CmowayprojectDlg : public CDialogEx
{
// Construcción
public:
	CmowayprojectDlg(CWnd* pParent = nullptr);	// Constructor estándar

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOWAY_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	bool is_connected = false;
	bool routine = false;
	afx_msg void OnBnClickedButtonRoutine();
	CButton b_routine;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int time_routine=0;
	//Thread que ejecuta la rutina de limpieza
	CWinThread *routine_thread;
	static UINT RoutineThread( LPVOID pParam);
	enum moway_routine {random, wall_follower, spiral, finished};

	//Estructura para guardar el estado del robot
	struct moway_state {
		//Sensores de proximidad
		int lS, clS, crS, rS;
		//Sensor de batería
		int battery;
		//Rutina actual
		moway_routine my_routine;
	} mymoway_state;

	void wall_follower_routine(CmowayprojectDlg *projectData, moway_state *mymoway_state);
	void random_routine(CmowayprojectDlg *projectData, moway_state *mymoway_state);
	void spiral_routine(CmowayprojectDlg *projectData, moway_state *mymoway_state);

	CProgressCtrl ProxLsBar;
	CProgressCtrl ProxClsBar;
	CProgressCtrl ProxCrsBar;
	CProgressCtrl ProxRsBar;
	CString edit_show_routine;
	CProgressCtrl battery_bar;
	int debug_control;
};
