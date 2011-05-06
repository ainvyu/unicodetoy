// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


LRESULT CMainDlg::OnEnChangeUserInput(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogImpl::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    ATL::CWindow wndUserInput = GetDlgItem(IDC_EDIT_USERINPUT);
    
    std::vector<TCHAR> arr;
    int nWndTxtSize = wndUserInput.GetWindowTextLengthW() + 1;
    arr.resize(nWndTxtSize);
    wndUserInput.GetWindowTextW(&arr[0], nWndTxtSize);
    arr[nWndTxtSize-1] = '\0';

    std::wstring strUTF16(&arr[0]);

    UpdateUTF16(strUTF16);
    UpdateUTF8(CStringEncode::UTF8FromUTF16(strUTF16));
    
    return 0;
}

void CMainDlg::UpdateUTF8(const std::string& strUTF8) const
{
    std::wstring strUTF8Bytes;
    std::for_each(strUTF8.begin(), strUTF8.end(), [&] (char byte) {
        strUTF8Bytes += _T("0x");
        strUTF8Bytes += CStringUtil::MakeUpper(CStringUtil::IntToStr((UCHAR)byte, 16));
        strUTF8Bytes += _T(", ");
    });

    ATL::CWindow wndUTF8 = GetDlgItem(IDC_EDIT_UTF8);
    wndUTF8.SetWindowText(strUTF8Bytes.c_str());
    wndUTF8.RedrawWindow();
}

void CMainDlg::UpdateUTF16(const std::wstring& strUTF16) const
{
    std::wstring strUTF16Bytes;
    for (int i = 0; i < strUTF16.size(); ++i) {
        char highbyte = (strUTF16[i] >> 8) & 0xff;
        char lowbyte  = strUTF16[i] & 0xff;

        strUTF16Bytes += _T("0x");
        strUTF16Bytes += CStringUtil::MakeUpper(CStringUtil::IntToStr((UCHAR)highbyte, 16));
        strUTF16Bytes += _T(", ");

        strUTF16Bytes += _T("0x");
        strUTF16Bytes += CStringUtil::MakeUpper(CStringUtil::IntToStr((UCHAR)lowbyte, 16));
        strUTF16Bytes += _T(", ");
    }

    ATL::CWindow wndUTF16 = GetDlgItem(IDC_EDIT_UTF16);
    wndUTF16.SetWindowText(strUTF16Bytes.c_str());
    wndUTF16.RedrawWindow();
}