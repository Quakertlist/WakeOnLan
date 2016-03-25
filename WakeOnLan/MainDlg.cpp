// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

#include <WinSock2.h>
#include "Ini.h"
#include "HelpMacro.h"


#pragma comment(lib, "ws2_32.lib")


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

    CEdit(GetDlgItem(IDC_EDT_MAC_ADDRESS)).SetLimitText(255);
    CEdit(GetDlgItem(IDC_EDT_IP_ADDRESS)).SetLimitText(511);
    CEdit(GetDlgItem(IDC_EDT_SUBNET_MASK)).SetLimitText(127);
    CEdit(GetDlgItem(IDC_EDT_PASSWORD)).SetLimitText(6);

    _seriesControlData(false);
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

bool CMainDlg::wakeOnLan(const char aryMac[6], const char* pszIPAddress, unsigned int nPort, const char* pszSubnetMask, const char* pszPassword)
{
    bool bRet = false;
    char magicPacket[256];
    int nPacketSize = 6;
    memset(magicPacket, 0xFF, nPacketSize);

    // 16个mac地址重复
    for (int n = 0; n < 16; ++n)
    {
        memcpy(magicPacket + nPacketSize, aryMac, 6);
        nPacketSize += 6;
    }

    // SecureOn
    if (pszPassword && strlen(pszPassword)>0)
    {
        memcpy(magicPacket + nPacketSize, pszPassword, 6);
        nPacketSize += 6;
    }

    // 发送魔法数据
    SOCKET s = INVALID_SOCKET;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
    do
    {
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (INVALID_SOCKET == s)
        {
            break;
        }
        BOOL bOptBroadcast = TRUE;
        int nOptLen = sizeof(bOptBroadcast);
        if (SOCKET_ERROR == setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&bOptBroadcast, nOptLen))
        {
            break;
        }
        SOCKADDR_IN addrTo;
        addrTo.sin_family = AF_INET;
        addrTo.sin_port = htons(nPort);
        HOSTENT* pHost = NULL;
        if (NULL==pszIPAddress || strlen(pszIPAddress)<=0)
        {
            addrTo.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        }
        else
        {
            pHost = gethostbyname(pszIPAddress);
            if (NULL == pHost)
            {
                pHost = gethostbyaddr(pszIPAddress, 4, AF_INET);
            }

            if (NULL != pHost)
            {
                char szIP[128] = { 0 };
                _fillIPAddress(szIP, pszSubnetMask, pHost);
                addrTo.sin_addr.s_addr = inet_addr(szIP);
            }
        }
        

        int nSend = sendto(s, (const char*)magicPacket, nPacketSize, 0, (const sockaddr*)&addrTo, sizeof(addrTo));
        if (SOCKET_ERROR == nSend)
        {
            break;
        }
        bRet = true;
    } while (false);
    if (INVALID_SOCKET != s)
    {
        closesocket(s);
    }
    WSACleanup();
    return bRet;
}

void CMainDlg::_seriesControlData(bool bSave)
{
    CIni ini;
    ini.SetPathName(_T(".\\config.data"));

    static const TCHAR* STR_CONFIG_SECTION = _T("config");

    static const TCHAR* STR_MAC_ADDRESS_KEY = _T("mac_address");
    static const TCHAR* STR_IP_ADDRESS_KEY = _T("ip_address");
    static const TCHAR* STR_SUBNET_MASK_KEY = _T("subnet_mask");
    static const TCHAR* STR_PASSWORD_KEY = _T("password");
    static const TCHAR* STR_PORT_KEY = _T("port");

    TCHAR szText[1024];
    if (bSave)
    {
        CEdit(GetDlgItem(IDC_EDT_MAC_ADDRESS)).GetWindowText(szText, 255);
        ini.WriteString(STR_CONFIG_SECTION, STR_MAC_ADDRESS_KEY, szText);
        CEdit(GetDlgItem(IDC_EDT_IP_ADDRESS)).GetWindowText(szText, 511);
        ini.WriteString(STR_CONFIG_SECTION, STR_IP_ADDRESS_KEY, szText);
        CEdit(GetDlgItem(IDC_EDT_SUBNET_MASK)).GetWindowText(szText, 127);
        ini.WriteString(STR_CONFIG_SECTION, STR_SUBNET_MASK_KEY, szText);
        CEdit(GetDlgItem(IDC_EDT_PASSWORD)).GetWindowText(szText, 127);
        ini.WriteString(STR_CONFIG_SECTION, STR_PASSWORD_KEY, szText);
        CEdit(GetDlgItem(IDC_EDT_PORT)).GetWindowText(szText, 64);
        ini.WriteString(STR_CONFIG_SECTION, STR_PORT_KEY, szText);

    }
    else
    {
        ini.GetString(STR_CONFIG_SECTION, STR_MAC_ADDRESS_KEY, szText, 1023, _T(""));
        SetDlgItemText(IDC_EDT_MAC_ADDRESS, szText);
        ini.GetString(STR_CONFIG_SECTION, STR_IP_ADDRESS_KEY, szText, 1023, _T(""));
        SetDlgItemText(IDC_EDT_IP_ADDRESS, szText);
        ini.GetString(STR_CONFIG_SECTION, STR_SUBNET_MASK_KEY, szText, 1023, _T("255.255.255.255"));
        SetDlgItemText(IDC_EDT_SUBNET_MASK, szText);
        ini.GetString(STR_CONFIG_SECTION, STR_PASSWORD_KEY, szText, 1023, _T(""));
        SetDlgItemText(IDC_EDT_PASSWORD, szText);
        ini.GetString(STR_CONFIG_SECTION, STR_PORT_KEY, szText, 1023, _T("4343"));
        SetDlgItemText(IDC_EDT_PORT, szText);
    }
}

void CMainDlg::_fillIPAddress(char szIP[128], const char szSubnetMask[128], const HOSTENT* pHost)
{// TODO: 子网掩码部分的算法还是不太明白
    ATLASSERT(NULL != pHost);
    long arySubnetMask[4] = { 0x00FF, 0x00FF, 0x00FF, 0x00FF };
    long aryIP[4] = { pHost->h_addr_list[0][0], pHost->h_addr_list[0][1], pHost->h_addr_list[0][2], pHost->h_addr_list[0][3]};
    if (szSubnetMask && strlen(szSubnetMask)>0)
    {// 处理子网掩码
        unsigned long aryTemp[4];
        if (4==sscanf(szSubnetMask, "%d.%d.%d.%d", &aryTemp[0], &aryTemp[1], &aryTemp[2], &aryTemp[3]) &&
            aryTemp[0]<=255 && aryTemp[1]<=255 && aryTemp[2]<=255 && aryTemp[3]<=255)
        {
            for (int n = 0; n < 4; ++n)
            {
                arySubnetMask[n] &= aryTemp[n];
                if (arySubnetMask[n] > 255)
                {
                    arySubnetMask[n] = 255;
                }
            }

        }
    }
    for (int n = 0; n < 4; ++n)
    {
        aryIP[n] &= arySubnetMask[n];
        if (aryIP[n] > 255 || 0==aryIP[3])
        {
            aryIP[n] = 255;
        }
    }
    snprintf(szIP, 127, "%d.%d.%d.%d", aryIP[0], aryIP[1], aryIP[2], aryIP[3]);
}

LRESULT CMainDlg::OnWakeUpClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    char szMacAddress[256], szIPAddress[512], szSubnetMask[128], szPassword[128];
    unsigned int nPort = 4343;
    ::GetWindowTextA(GetDlgItem(IDC_EDT_MAC_ADDRESS), szMacAddress, 255);
    ::GetWindowTextA(GetDlgItem(IDC_EDT_IP_ADDRESS), szIPAddress, 511);
    ::GetWindowTextA(GetDlgItem(IDC_EDT_SUBNET_MASK), szSubnetMask, 127);
    ::GetWindowTextA(GetDlgItem(IDC_EDT_PASSWORD), szPassword, 127);
    nPort = GetDlgItemInt(IDC_EDT_PORT);

    do 
    {
        char aryMac[6];
        if (!getMacAddress(szMacAddress, aryMac, 6))
        {
            break;
        }
        if (wakeOnLan(aryMac, szIPAddress, nPort, szSubnetMask, szPassword))
        {
            _seriesControlData(true);
            MessageBox(_T("Magic packet send!"), _T("Success"), MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            MessageBox(_T("Send magic packet failed!"), _T("Fail"), MB_OK | MB_ICONERROR);
        }
    } while (false);

    return 0;
}


bool CMainDlg::getMacAddress(const char* pszMac, char aryMac[6], int nAryCount) const
{
    bool bRet = false;
    do 
    {
        long aryTemp[6];
        int nCount = sscanf(pszMac, "%2x-%2x-%2x-%2x-%2x-%2x",
            &aryTemp[0], &aryTemp[1], &aryTemp[2],
            &aryTemp[3], &aryTemp[4], &aryTemp[5]);
        if (6 != nCount)
        {
            nCount = sscanf(pszMac, "%2x:%2x:%2x:%2x:%2x:%2x",
                &aryTemp[0], &aryTemp[1], &aryTemp[2],
                &aryTemp[3], &aryTemp[4], &aryTemp[5]);
        }
        if (6!=nCount)
        {
            nCount = sscanf(pszMac, "%2x%2x%2x%2x%2x%2x",
                &aryTemp[0], &aryTemp[1], &aryTemp[2],
                &aryTemp[3], &aryTemp[4], &aryTemp[5]);
        }
        if (6!=nCount || nAryCount!=nCount)
        {
            break;
        }
        for (int n = 0; n < nCount; ++n)
        {
            aryMac[n] = (char)aryTemp[n];
        }
        bRet = true;
    } while (false);
    return bRet;
}