﻿
// TetrisDlg.h: файл заголовка
//

#pragma once
#include <set>
#include "Tetramino.h"

// Диалоговое окно CTetrisDlg
class CTetrisDlg : public CDialogEx
{
// Создание
public:
	CTetrisDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TETRIS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	CStatic m_Static[10][20];
	CBitmap m_Bitmap;
	CButton m_GroupBox;

	Tetramino m_Tetramino;	

	enum class State_t
	{
		Empty,
		Fall,
		GameOver
	} m_State = State_t::Empty;


	void Draw(const Tetramino& tetramino, bool a_Show);

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	//HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	std::set<Point> m_Fallen;

	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};
