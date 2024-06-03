
// TetrisDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include <vector>
#include <string>
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
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CTetrisDlg::OnTcnSelchangeTab)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

#define IDC_GROUP_BOX  12344
#define IDC_EXTRA_EDIT 12345

// Обработчики сообщений CTetrisDlg

BOOL CTetrisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	int id = IDC_EXTRA_EDIT;

	constexpr int size = 16;
	constexpr int delta = 2;

	CRect m_rec(45, 38, 52 + 10 * (size + delta), 52 + 20 * (size + delta));
	m_GroupBox.Create(L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, m_rec,
		this, IDC_GROUP_BOX);

	{
		m_Bitmap.LoadBitmap(IDB_BITMAP1);		

		for(int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				CStatic& cell = m_Static[x][y];

				const int _x = 4 + x * (size + delta);
				const int _y = 11 + y * (size + delta);

				CRect rect(_x, _y, _x + size, _y + size);

				int style = WS_CHILD | WS_BORDER | SS_BITMAP | SS_CENTERIMAGE;

				cell.Create(L"", style, rect, &m_GroupBox, id++);
				cell.SetBitmap(m_Bitmap);
			}
		}
	}

	auto m_nTimer = SetTimer(id++, 1000, 0);

	m_FontBig.Detach();
	m_FontBig.CreateFont(48, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 0, 0, 0, 0, 0, L"Tahoma");
	GetDlgItem(IDC_SUMMARY)->SetFont(&m_FontBig, TRUE);

	m_FontMedium.Detach();
	m_FontMedium.CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 0, 0, 0, 0, 0, L"Tahoma");
	GetDlgItem(IDC_HELP)->SetFont(&m_FontMedium, TRUE);

	{
		CTabCtrl* tabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;

		tcItem.pszText = _T("Results");
		tabCtrl->InsertItem(0, &tcItem);

		tcItem.pszText = _T("Game");
		tabCtrl->InsertItem(0, &tcItem);

		HWND hwnd = GetDlgItem(IDC_TAB)->m_hWnd;
		TabCtrl_SetCurSel(hwnd, 0);
		LRESULT result;
		OnTcnSelchangeTab(nullptr, &result);
	}

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTetrisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

HBRUSH CTetrisDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	const auto ID = pWnd->GetDlgCtrlID();
	if (ID == IDC_GROUP_BOX)
	{
		CPoint ul(0, 6);
		CRect rect;
		pWnd->GetWindowRect(&rect);
		CPoint lr((rect.right - rect.left - 2), (rect.bottom - rect.top - 2));
		pDC->FillSolidRect(CRect(ul, lr), RGB(255, 255, 255));
		pWnd->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	if (ID == IDC_SUMMARY)
	{
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->SetBkMode(TRANSPARENT);
	}

	if (ID == IDC_HELP)
	{
		pDC->SetTextColor(RGB(200, 200, 200));
		pDC->SetBkMode(TRANSPARENT);
	}

	if (ID == IDC_TAB)
	{
		pDC->SetBkMode(TRANSPARENT);
	}

	return (HBRUSH)GetStockObject(DKGRAY_BRUSH);
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

void CTetrisDlg::Draw(const Point& point, bool a_Show)
{
	m_Static[point.m_x][point.m_y].ShowWindow(a_Show ? SW_SHOW : SW_HIDE);
}

void CTetrisDlg::Draw(const Tetramino& tetramino, bool a_Show)
{
	for (const Point& point : tetramino.m_points)
	{
		Draw(point, a_Show);
	}
}

void CTetrisDlg::Draw(Fallen_t fallen, bool a_Show)
{
	for (const Point& point : fallen)
	{
		Draw(point, a_Show);
	}
}

void CTetrisDlg::DropFullLines()
{
	constexpr int OLD_Y_START = -1;
	int old_y = OLD_Y_START;
	int shift_y = 0;
	std::vector<int> buffer;

	Fallen_t new_Fallen;

	for (const Point& point : m_Fallen)
	{
		if (old_y == OLD_Y_START)
			old_y = point.m_y;

		if (old_y != point.m_y)
		{
			if (buffer.size() == 10)
				shift_y++;
			else
			{
				for (int x : buffer)
				{
					new_Fallen.insert({ x , old_y + shift_y });
				}
			}
			
			buffer.clear();
			old_y = point.m_y;
		}

		buffer.push_back(point.m_x);
	}

	if (buffer.size() == 10)
		shift_y++;
	else
	{
		for (int x : buffer)
		{
			new_Fallen.insert({ x , old_y + shift_y });
		}
	}

	if (shift_y > 0)
	{
		Draw(m_Fallen, false);
		m_Fallen = new_Fallen;
		Draw(m_Fallen, true);

		m_Score += shift_y;
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
			OnTimerImpl();
			break;
		case VK_RETURN:
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
						OnTimerImpl();
						break;
					}
				}
			}
			return FALSE;
		case VK_TAB:		
			if (::GetKeyState(VK_CONTROL) & 0x8000)
			{
				HWND hwnd = GetDlgItem(IDC_TAB)->m_hWnd;
				const int i = TabCtrl_GetCurSel(hwnd);
				TabCtrl_SetCurSel(hwnd, 1 - i);
				LRESULT result;
				OnTcnSelchangeTab(nullptr, &result);
			}
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

}

void CTetrisDlg::OnTimerImpl()
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
		DropFullLines();

		m_State = State_t::Empty;
		break;
	}
}

void CTetrisDlg::OnTimer(UINT_PTR)
{
	OnTimerImpl();

	if (m_State != State_t::GameOver)
		m_Duration++;

	std::wstring summary = L"Score: " + std::to_wstring(m_Score * 100) + L"\r\n";

	std::wstring sec = std::to_wstring(m_Duration % 60);
	if (sec.length() < 2)
		sec = L'0' + sec;
	summary += L"Duration: " + std::to_wstring(m_Duration / 60) + L':' + sec + L"\r\n";

	if (m_State == State_t::GameOver)
		summary += L"Game Over!";

	GetDlgItem(IDC_SUMMARY)->SetWindowText(summary.c_str());
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTetrisDlg::OnTcnSelchangeTab(NMHDR*, LRESULT* pResult)
{
	auto tab = TabCtrl_GetCurSel(GetDlgItem(IDC_TAB)->m_hWnd);
	//MessageBoxA(NULL, "AAA", std::to_string(i).c_str(), MB_OK);

	GetDlgItem(IDC_GROUP_BOX)->ShowWindow(tab == 0);
	GetDlgItem(IDC_SUMMARY)->ShowWindow(tab == 0);
	GetDlgItem(IDC_HELP)->ShowWindow(tab == 0);

	*pResult = 0;
}
