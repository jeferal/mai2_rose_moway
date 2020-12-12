
// moway_projectDlg.cpp: archivo de implementación
//

#include "pch.h"
#include "framework.h"
#include "moway_project.h"
#include "moway_projectDlg.h"
#include "afxdialogex.h"
#include "stdlib.h"

#include "moway_lib/CMoway.h"
#include "moway_lib/RFModule.h"
#include "moway_lib/usb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Variable global
CMoway mymoway;

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cuadro de diálogo de CmowayprojectDlg



CmowayprojectDlg::CmowayprojectDlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_MOWAY_PROJECT_DIALOG, pParent)
	, edit_show_routine(_T(""))
	, debug_control(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmowayprojectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ROUTINE, b_routine);
	DDX_Control(pDX, IDC_PROGRESS_LS, ProxLsBar);
	DDX_Control(pDX, IDC_PROGRESS_CLS, ProxClsBar);
	DDX_Control(pDX, IDC_PROGRESS_CRS, ProxCrsBar);
	DDX_Control(pDX, IDC_PROGRESS_RS, ProxRsBar);
	DDX_Text(pDX, IDC_EDIT_SHOW_ROUTINE, edit_show_routine);
	DDX_Control(pDX, IDC_PROGRESS_BATTERY, battery_bar);
}

BEGIN_MESSAGE_MAP(CmowayprojectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CmowayprojectDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CmowayprojectDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_ROUTINE, &CmowayprojectDlg::OnBnClickedButtonRoutine)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Controladores de mensajes de CmowayprojectDlg

BOOL CmowayprojectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Establecer el icono para este cuadro de diálogo.  El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// Inicialización del hilo de ejecución y de los visualizadores
	routine_thread = NULL;

	ProxLsBar.SetRange(0, 100);
	ProxLsBar.SetPos(0);

	ProxClsBar.SetRange(0, 100);
	ProxClsBar.SetPos(0);

	ProxCrsBar.SetRange(0, 100);
	ProxCrsBar.SetPos(0);

	ProxRsBar.SetRange(0, 100);
	ProxRsBar.SetPos(0);

	battery_bar.SetRange(0, 100);
	battery_bar.SetPos(0);

	edit_show_routine = "Moway desconectado";
	UpdateData(FALSE);

	srand(time(NULL));

	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

void CmowayprojectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono.  Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CmowayprojectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CmowayprojectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmowayprojectDlg::OnBnClickedButtonConnect()
{
	// Conexión con comprobación de error
	if (mymoway.ConnectMoway(ID_MOWAY)) {
		AfxMessageBox((CString)"Moway conectado, timers encendidos");
		is_connected = true;
		edit_show_routine = "Listo para rutina";
		Sleep(300);
		if (!mymoway.ReadBatteryStatus(&mymoway_state.battery))
			AfxMessageBox((CString)"Error leyendo el estado de la batería");
		Sleep(300);
		UpdateData(FALSE);
	}
	else {
		AfxMessageBox((CString)"Error de conexion");
		is_connected = false;
		edit_show_routine = "Prueba a conectar de nuevo";
		UpdateData(FALSE);
	}
}


void CmowayprojectDlg::OnBnClickedButtonDisconnect()
{
	//Desconexión con comprobación de error
	if (mymoway.DisconnectMoway()) {
		AfxMessageBox((CString)"Moway desconectado");
		is_connected = false;
		edit_show_routine = "Moway desconectado";
		UpdateData(FALSE);
	}
	else {
		AfxMessageBox((CString)"Error al desconectar");
		is_connected = true;
	}
}



UINT CmowayprojectDlg::RoutineThread( LPVOID pParam ) 
{
	//Casting params
	CmowayprojectDlg *projectData = (CmowayprojectDlg *)pParam;
	moway_state *mymoway_state = (moway_state*)&projectData->mymoway_state;

	if (!projectData->is_connected) {
		AfxEndThread(1);
		return 0;
	}

	while (projectData->routine) {
		switch (mymoway_state->my_routine) {
		case random:
			projectData->random_routine(projectData, mymoway_state);
			break;
		case wall_follower:
			projectData->wall_follower_routine(projectData, mymoway_state);
			break;
		case spiral:
			projectData->spiral_routine(projectData, mymoway_state);
			break;
		default:
			break;
		}
	}
	
	mymoway.MotorStop();
	AfxEndThread(1);
	return 0;
}

void CmowayprojectDlg::wall_follower_routine(CmowayprojectDlg * projectData, moway_state * mymoway_state)
{

	//###	SEGUIMIENTO DE PAREDES	###//

	//Variables para la máquina de estados
	enum State { go_forward, wall_izq, wall_izq_correction, corner_izq };
	static State S = go_forward;
	static State S_prev = go_forward;
	int umbral = 100;
	int umbral_high = 150;
	int umbral_low = 75;

	if (!projectData->is_connected) {
		S = go_forward;
		S_prev = go_forward;
		AfxEndThread(1);
		return;
	}

	switch (S) {
	case go_forward:
		//Aplicar salidas
		mymoway.SetSpeed(30, 40, CMoway::FORWARD, CMoway::FORWARD, 0, 0);
		Sleep(100);

		//Comprobar sensores
		if (!mymoway.ReadProximitySensors(&mymoway_state->lS, &mymoway_state->clS, &mymoway_state->crS, &mymoway_state->rS))
			AfxMessageBox((CString)"Error leyendo los sensores de proximidad");
		Sleep(100);

		//Condiciones para las transiciones
		if (mymoway_state->lS > umbral && mymoway_state->clS > umbral) { S = corner_izq; S_prev = go_forward; break; }
		else if (mymoway_state->crS > umbral_high || mymoway_state->clS > umbral_high){	S = corner_izq; S_prev = go_forward; break; }
		else if (mymoway_state->lS < umbral_low && S_prev == wall_izq) { S = wall_izq_correction; S_prev = go_forward; break; }
		else if (mymoway_state->lS > umbral) { S = wall_izq; S_prev = go_forward; break; }
		else { S = go_forward; S_prev = go_forward; break; }

		case wall_izq:
			//Girar muy poco a la derecha
			Sleep(100);
			mymoway.SetSpeed(10, 0, CMoway::FORWARD, CMoway::BACKWARD, 0, 0);
			Sleep(300);
			S = go_forward;
			S_prev = wall_izq;
			break;

		case wall_izq_correction:
			//Girar muy poco a la izquierda
			Sleep(100);
			mymoway.SetSpeed(0, 10, CMoway::BACKWARD, CMoway::FORWARD, 0, 0);
			Sleep(200);

			S = go_forward;
			S_prev = wall_izq_correction;
			break;

		case corner_izq:
			//Hacia detrás y después girar hacia la derecha
			Sleep(100);
			mymoway.SetSpeed(10, 10, CMoway::BACKWARD, CMoway::BACKWARD, 0, 0);
			Sleep(500);
			mymoway.SetSpeed(30, 0, CMoway::FORWARD, CMoway::BACKWARD, 0, 0);
			Sleep(300);
			S = go_forward;
			S_prev = corner_izq;
			break;

		default:
			S = go_forward;
			S_prev = go_forward;
			break;
		}
}

void CmowayprojectDlg::random_routine(CmowayprojectDlg * projectData, moway_state * mymoway_state)
{
	//###	COMPORTAMIENTO EVITAR OBSTÁCULOS	###//

	//Variables para la máquina de estados
	enum State { no_obs, obs_der, obs_izq, obs_front };
	static State S = no_obs;
	int umbral = 100;

	int angulo, time_spin;

	if (!projectData->is_connected) {
		S = no_obs;
		AfxEndThread(1);
		return;
	}


	switch (S) {
	case no_obs:
		//Aplicar salidas
		mymoway.GoStraight(50);
		Sleep(100);

		//Comprobar sensores
		if (!mymoway.ReadProximitySensors(&mymoway_state->lS, &mymoway_state->clS, &mymoway_state->crS, &mymoway_state->rS))
			AfxMessageBox((CString)"Error leyendo los sensores de proximidad");
		Sleep(100);

		if (mymoway_state->clS > umbral) { S = obs_front; break; }
		else if (mymoway_state->lS > umbral) { S = obs_izq; break; }
		else if (mymoway_state->rS > umbral) { S = obs_der; break; }
		else if (mymoway_state->crS > umbral) { S = obs_front; break; }
		else { S = no_obs; break; }

	case obs_der:
		//Aplicar salidas
		mymoway.MotorStop();
		Sleep(100);
		mymoway.SetSpeed(100, 10, CMoway::BACKWARD, CMoway::BACKWARD, 0, 0);
		angulo = rand() % (LATANGMAX - LATANGMIN + 1) + LATANGMIN;
		time_spin = int(500 / 30)*angulo;
		Sleep(time_spin);
		S = no_obs;
		break;

	case obs_izq:
		//Aplicar salidas
		mymoway.MotorStop();
		Sleep(100);
		mymoway.SetSpeed(10, 100, CMoway::BACKWARD, CMoway::BACKWARD, 0, 0);
		angulo = rand() % (LATANGMAX - LATANGMIN + 1) + LATANGMIN;
		time_spin = int(500 / 30)*angulo;
		Sleep(time_spin);
		S = no_obs;
		break;

	case obs_front:
		//Generar ángulo aleatorio
		mymoway.MotorStop();
		Sleep(100);
		mymoway.SetSpeed(10, 10, CMoway::BACKWARD, CMoway::BACKWARD, 0, 0);
		Sleep(400);
		angulo = rand() % (FRONTANGMAX - FRONTANGMIN + 1) + FRONTANGMIN;
		time_spin = int(5.3751)*angulo - 550;
		mymoway.SetSpeed(60, 60, CMoway::BACKWARD, CMoway::FORWARD, 0, 0);
		Sleep(time_spin);
		Sleep(350);
		S = no_obs;
		break;
	}
}

void CmowayprojectDlg::spiral_routine(CmowayprojectDlg * projectData, moway_state * mymoway_state)
{
	//Variables para la máquina de estados
	enum State { no_obs, obs_der, obs_izq, obs_front };
	static State S = no_obs;
	int umbral = 100;
	static int iter = 0;
	int angulo, time_spin;

	//###	COMPORTAMIENTO ESPIRAL	###//

	if (!projectData->is_connected) {
		S = no_obs;
		iter = 0;
		AfxEndThread(1);
		return;
	}


	switch (S) {
		case no_obs:
			//Aplicar salidas
			
			if (iter < 10)
			{
				//Grado de apertura 1
				mymoway.SetSpeed(2, 100, CMoway::FORWARD, CMoway::FORWARD, 0, 0);
			}
			else if (iter < 15)
			{
				//Grado de apertura 2
				mymoway.SetSpeed(30, 100, CMoway::FORWARD, CMoway::FORWARD, 0, 0);
			}
			else
			{
				//Grado de apertura 3
				mymoway.SetSpeed(50, 100, CMoway::FORWARD, CMoway::FORWARD, 0, 0);
			}
			

			if (iter > 20)
				iter = 0;
			else
				iter++;

			Sleep(100);

			//Comprobar sensores
			if (!mymoway.ReadProximitySensors(&mymoway_state->lS, &mymoway_state->clS, &mymoway_state->crS, &mymoway_state->rS))
				AfxMessageBox((CString)"Error leyendo los sensores de proximidad");
			Sleep(100);

			if (mymoway_state->clS > umbral) { S = obs_front; break; }
			else if (mymoway_state->lS > umbral) { S = obs_izq; break; }
			else if (mymoway_state->rS > umbral) { S = obs_der; break; }
			else if (mymoway_state->crS > umbral) { S = obs_front; break; }
			else { S = no_obs; break; }
		case obs_der:
			mymoway.SetSpeed(0, 100, CMoway::BACKWARD, CMoway::FORWARD, 0, 0);
			Sleep(400);
			S = no_obs;
			break;

		case obs_izq:
			mymoway.SetSpeed(100, 0, CMoway::BACKWARD, CMoway::FORWARD, 0, 0);
			Sleep(400);
			S = no_obs;
			break;

		case obs_front:
			mymoway.MotorStop();
			Sleep(100);
			mymoway.SetSpeed(10, 10, CMoway::BACKWARD, CMoway::BACKWARD, 0, 0);
			Sleep(300);
			mymoway.SetSpeed(50, 50, CMoway::BACKWARD, CMoway::FORWARD, 0, 0);
			Sleep(300);
			S = no_obs;
			break;
		}
}

void CmowayprojectDlg::OnBnClickedButtonRoutine()
{

	if (!is_connected) {
		AfxMessageBox((CString)"Moway no esta conectado");
		return;
	}
	if (routine) {
		routine = false;
		b_routine.SetWindowTextW(CString("Comenzar rutina"));
		edit_show_routine = "Listo para rutina";

		KillTimer(TIMER_1);

		ProxLsBar.SetPos(0);
		ProxClsBar.SetPos(0);
		ProxCrsBar.SetPos(0);
		ProxRsBar.SetPos(0);
		UpdateData(FALSE);
	}
	else
	{
		routine = true;
		routine_thread = AfxBeginThread(RoutineThread, this);
		if (routine_thread == NULL) {
			AfxMessageBox((CString)"No se ha podido crear la rutina");
			return;
		}
		//Set timer para visualizar estado de los sensores
		SetTimer(TIMER_1, SAMPLE_TIME_1, NULL);
		b_routine.SetWindowTextW(CString("Parar rutina"));

		//Comenzar con rutina random
		mymoway_state.my_routine = random;
		time_routine = 0;
	}
}


void CmowayprojectDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_1) {
		UpdateData(TRUE);
		ProxLsBar.SetPos(mymoway_state.lS);
		ProxClsBar.SetPos(mymoway_state.clS);
		ProxCrsBar.SetPos(mymoway_state.crS);
		ProxRsBar.SetPos(mymoway_state.rS);
		battery_bar.SetPos(mymoway_state.battery);
		
		if (!is_connected)
			edit_show_routine = "Disconnected";
		else 
		{
			switch (mymoway_state.my_routine) {
				case random:
					edit_show_routine = "Evitar obstaculos random";
					break;
				case wall_follower:
					edit_show_routine = "Seguimiento de paredes";
					break;
				case spiral:
					edit_show_routine = "Trayectorias en espirales";
					break;
			}
		}
		UpdateData(FALSE);
		
		time_routine++;
		if (time_routine < 4 * 60)
			mymoway_state.my_routine = random;
		else if (time_routine >= 4 * 60 && time_routine <= 4 * 120)
			mymoway_state.my_routine = wall_follower;
		else if (time_routine > 4 * 120 && time_routine <= 4 * 180)
			mymoway_state.my_routine = spiral;
		else
			time_routine = 0;
	}
	CDialogEx::OnTimer(nIDEvent);
}

