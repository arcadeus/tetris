
// TetrisDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Tetris.h"
#include "TetrisDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTetrisDlg::CTetrisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TETRIS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTetrisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTetrisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTetrisDlg::OnTcnSelchangeTab1)
	ON_WM_TIMER()
END_MESSAGE_MAP()

#define IDC_EXTRA_EDIT 12345

// Обработчики сообщений CTetrisDlg

BOOL CTetrisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	/*
	{
		CTabCtrl* tabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB1);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;

		tcItem.pszText = _T("Tab #1 aaaa");
		tabCtrl->InsertItem(0, &tcItem);

		tcItem.pszText = _T("Tab #2 bbbb");
		tabCtrl->InsertItem(0, &tcItem);
	}*/

	int id = IDC_EXTRA_EDIT;

	constexpr int size = 16;
	constexpr int delta = 2;

	{
		m_Bitmap.LoadBitmap(IDB_BITMAP1);		

		for(int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				CStatic& cell = m_Static[x][y];

				const int _x = 50 + x * (size + delta);
				const int _y = 50 + y * (size + delta);

				CRect rect(_x, _y, _x + size, _y + size);

				int style = WS_CHILD | WS_BORDER | SS_BITMAP | SS_CENTERIMAGE;

				//				if ((x + y) % 2)					style |= WS_VISIBLE;

				cell.Create(L"", style, rect, this, id++);
				cell.SetBitmap(m_Bitmap);
			}
		}
	}

	CRect m_rec(45, 38, 52 + 10 * (size + delta), 52 + 20 * (size + delta));
	m_GroupBox.Create(L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, m_rec,
		this, id++);

	auto m_nTimer = SetTimer(id++, 1000, 0);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTetrisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTetrisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CTetrisDlg::Draw(const Tetramino& tetramino, bool a_Show)
{
	for (const Point& point : tetramino.m_points)
	{
		m_Static[point.m_x][point.m_y].ShowWindow(a_Show ? SW_SHOW : SW_HIDE);
	}
}

afx_msg BOOL CTetrisDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_LEFT:
		case VK_RIGHT:
			if (m_State == State_t::Fall)
			{
				const Tetramino backup = m_Tetramino;
				m_Tetramino.MoveAside(pMsg->wParam == VK_RIGHT);
				if (m_Tetramino.IsValid(*this))
				{
					Draw(backup, false);
					Draw(m_Tetramino, true);
				}
				else
					m_Tetramino = backup;
			}
			break;		
		case VK_UP:
			if (m_State == State_t::Fall)
			{
				const Tetramino backup = m_Tetramino;
				m_Tetramino.Rotate();
				if (m_Tetramino.IsValid(*this))
				{
					Draw(backup, false);
					Draw(m_Tetramino, true);
				}
				else
					m_Tetramino = backup;
			}
			break;		
		case VK_DOWN:
			if (m_State == State_t::Fall)
			{
				for (;;)
				{
					const Tetramino backup = m_Tetramino;
					m_Tetramino.MoveDown();
					if (m_Tetramino.IsValid(*this))
					{
						Draw(backup, false);
						Draw(m_Tetramino, true);
					}
					else
					{
						m_Tetramino = backup;
						OnTimer((UINT_PTR)nullptr);
						break;
					}
				}
			}
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

}

void CTetrisDlg::OnTimer(UINT_PTR)
{
	switch (m_State)
	{
	case State_t::GameOver:
		return;
	case State_t::Empty:
		m_Tetramino.Init();
		Draw(m_Tetramino, true);
		if (!m_Tetramino.IsValid(*this))
		{
			m_State = State_t::GameOver;
			return;
		}
		m_State = State_t::Fall;
		break;
	case State_t::Fall:
		const Tetramino backup = m_Tetramino;
		m_Tetramino.MoveDown();
		if (m_Tetramino.IsValid(*this))
		{
			Draw(backup, false);
			Draw(m_Tetramino, true);
			break;
		}

		for (const Point& point : backup.m_points)
		{
			m_Fallen.insert(point);
		}
		m_State = State_t::Empty;
		break;
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTetrisDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}
