#include <windows.h>
#include <shobjidl.h>
#include <iostream>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

ID2D1Factory			*pFactory;
ID2D1HwndRenderTarget	*pRenderTarget;
ID2D1SolidColorBrush	*pBrush;
D2D1_ELLIPSE			ellipse;

LRESULT CALLBACK MainWindProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// �� 01��
	// ��ʾһ����Ϣ��                     ����                              title           ����
	//::MessageBox(nullptr,TEXT("Hello, win32 Application���"),TEXT("First Win32 Window"),MB_OK);
	

	// �� 02��
	// �����Ƿ������Ϊ"�ޱ��� - ���±�"�Ĵ���
	//HWND hWndo = ::FindWindow(nullptr,TEXT("�ޱ��� - ���±�"));
	//if (hWndo != nullptr){
	//	// �򴰿ڷ�����Ϣ   WM_CLOSE �����յ�����Ϣ��ᱻ�ر�
	//	//::SendMessage(hWndo, WM_CLOSE, 0, 0);	// ����Ϣ���ȴ���Ϣ������
	//	::PostMessage(hWndo, WM_CLOSE, 0, 0);	// ����Ϣ����Ϣ�������������أ����ȴ���Ϣ��ִ�н��
	//}

	// �� 03��
	// ����һ�����ڶ���
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);								//����Ĵ�С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//���ڷ��				ˮƽ �� ��ֱ �������ı� �ػ洰��
	wndClass.lpfnWndProc = MainWindProc;							//��Ϣ������
	wndClass.cbClsExtra = 0;										//
	wndClass.cbWndExtra = 0;										//
	wndClass.hInstance = hInstance;									//��ģ���ʵ�����
	wndClass.hIcon = ::LoadIcon(nullptr,IDI_APPLICATION );			//�������Ͻ�ͼ��ľ��
	wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);			//�����
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);	//������ˢ��ɫ
	wndClass.lpszMenuName = nullptr;								//�˵���
	wndClass.lpszClassName = TEXT("MainWClass");					//�ô����������
	wndClass.hIconSm = nullptr;										//Сͼ����



	// ע�ᴰ��
	::RegisterClassEx(&wndClass);

	// �����˵�
	HMENU hMenu = CreateMenu();	// ���˵�
	HMENU hMenuPop = CreateMenu();	// �����˵�
	AppendMenu(hMenuPop, MF_STRING, 4001, TEXT("New"));
	AppendMenu(hMenuPop, MF_STRING, 4002, TEXT("Open"));
	AppendMenu(hMenu, MF_POPUP, (unsigned int)hMenuPop, TEXT("File"));

	hMenuPop = CreateMenu();
	AppendMenu(hMenuPop, MF_STRING, 4003, TEXT("Copy"));
	AppendMenu(hMenuPop, MF_STRING, 4004, TEXT("Cut"));
	AppendMenu(hMenu, MF_POPUP, (unsigned int)hMenuPop, TEXT("Edit"));

	hMenuPop = CreateMenu();
	AppendMenu(hMenuPop, MF_STRING, 4005, TEXT("Left"));
	AppendMenu(hMenuPop, MF_STRING, 4006, TEXT("Right"));
	AppendMenu(hMenuPop, MF_STRING, 4007, TEXT("Top"));
	AppendMenu(hMenuPop, MF_STRING, 4008, TEXT("Bottom"));
	AppendMenu(hMenu, MF_POPUP, (unsigned int)hMenuPop, TEXT("Move"));

	hMenuPop = CreateMenu();
	AppendMenu(hMenuPop, MF_STRING, 4009, TEXT("Size"));
	AppendMenu(hMenu, MF_POPUP, (unsigned int)hMenuPop, TEXT("Get"));


	// ��������
	HWND hWnd = ::CreateWindowEx(
		0,								// ���ڵ���չ���
		TEXT("MainWClass"),				// ָ��ע��������ָ��
		TEXT("My First Window"),		// ָ�򴰿�����ָ�� 
		WS_OVERLAPPEDWINDOW,			// ���ڷ��
		CW_USEDEFAULT,					// ���ڵ�ˮƽλ��
		CW_USEDEFAULT,					// ���ڵĴ�ֱλ��
		CW_USEDEFAULT,					// ���ڵĿ��
		CW_USEDEFAULT,					// ���ڵĸ߶�
		nullptr,						// ������ľ��
		hMenu,							// �˵��ľ�� �� �Ӵ��ڵľ��
		hInstance,						// Ӧ�ó���ʵ���ľ��
		nullptr							// ָ�򴰿ڵĴ�������
		);
	if (nullptr == hWnd){
		std::cout << "Create Window Error!" << std::endl;
		return -1;
	}

	// ����һ��button
	HWND hButton = ::CreateWindowEx(
		0,
		TEXT("BUTTON"),
		TEXT("My BUTTON"),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10,
		10,
		100,
		100,
		hWnd,
		NULL,
		(HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),
		nullptr
		);

	// ��ʾ����
	::ShowWindow(hWnd, nCmdShow);
	// ���´���
	::UpdateWindow(hWnd);

	/*
		��Ϣ���ݹ���
		1��ϵͳ���û�������ĳ������
		2��ϵͳ����Ϣ�洢��MSG�ṹ����
		3��ϵͳ������Ϣ�洢�����Ӧ�ó������Ϣ������
	*/
	MSG msg;
	// ��Ϣѭ��
	while (::GetMessage(&msg, nullptr, 0, 0)){
		::TranslateMessage(&msg);		// ������Ϣ
		::DispatchMessage(&msg);		// �����ô�����������ת�ظ�����ϵͳ����Ϣ�ӹ���ϡ�
	}

	return msg.wParam;
}

// ��Ϣ������
LRESULT CALLBACK MainWindProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	switch (message)
	{
	case WM_CREATE:
		// ��������Factory:factory object��    ���߳�
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))){
			return -1;
		}
		return 0;
	case WM_PAINT:
	{
		/*PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hwnd, &ps);
		std::wstring str = L"This is Test Text!";
		::TextOut(hdc, 0, 0, str.c_str(), str.size());
		::EndPaint(hwnd, &ps);*/
		HRESULT hr = S_OK;
		if (NULL == pRenderTarget){
			RECT rect;
			// ��ȡ�ͻ���
			GetClientRect(hwnd, &rect);
			// ����һ�����η�Χ
			D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
			// ������Ⱦ��RenderTarget:Direct2D Resources��
			hr = pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),			
				D2D1::HwndRenderTargetProperties(hwnd,size),
				&pRenderTarget
				);
			if (SUCCEEDED(hr)){
				const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
				// ������ˢ��Brush:Direct2D Resources��
				hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
			}

		}
		if (SUCCEEDED(hr)){
			if (pRenderTarget != NULL){
				// ��ȡ�ͻ����Ĵ�С
				D2D1_SIZE_F size = pRenderTarget->GetSize();

				const float x = size.width / 2;
				const float y = size.height / 2;
				// Բ�İ뾶��ȡ���һ�����Сֵ
				const float radius = min(x, y);
				// ����Բ����Ellipse:Direct2D Resources��
				ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
			}
			PAINTSTRUCT ps;
			// ��Direct2D Render Loop��
			BeginPaint(hwnd, &ps);
			pRenderTarget->BeginDraw();
			pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
			pRenderTarget->FillEllipse(ellipse, pBrush);
			hr = pRenderTarget->EndDraw();
			if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET){
				if (pRenderTarget)
				{
					pRenderTarget->Release();
					pRenderTarget = NULL;
				}
				if (pBrush)
				{
					pBrush->Release();
					pBrush = NULL;
				}
			}
			// ��Direct2D Render Loop��
			EndPaint(hwnd, &ps);
		}
		return 0;
	}
	case WM_SIZE:
		if (pRenderTarget != NULL){
			RECT rect;
			GetClientRect(hwnd, &rect);
			D2D1_SIZE_U size_u = D2D1::SizeU(rect.right, rect.bottom);
			pRenderTarget->Resize(size_u);
			D2D1_SIZE_F size_f = pRenderTarget->GetSize();
			const float x = size_f.width / 2;
			const float y = size_f.height / 2;
			const float radius = min(x, y);
			ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

			InvalidateRect(hwnd, NULL, FALSE);
		}
		else
		{
			//MessageBox(hwnd,TEXT("connot found render target"),TEXT("render target"),MB_OK);
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		::MessageBox(hwnd, TEXT("left button down"), TEXT("notting"), MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			// ָ�������-->ģ̬�Ի���  �ر��ӶԻ����ſ��ԶԸ��Ի�����в���
		case 4001:
			::MessageBox(hwnd, TEXT("��̬�˵�File���New"), TEXT("�˵�"), MB_OK); break;
		case 4002:
		{
			// Initialize the COM Library
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

			if (SUCCEEDED(hr)){
				// Create an Object in COM 
				IFileOpenDialog *pFileOpen;
				hr = CoCreateInstance(CLSID_FileOpenDialog,
					NULL,
					CLSCTX_ALL,
					IID_IFileOpenDialog,
					reinterpret_cast<void**>(&pFileOpen));
				
				if (SUCCEEDED(hr)){
					hr = pFileOpen->Show(NULL);
					if (SUCCEEDED(hr)){
						IShellItem *pItem;
						hr = pFileOpen->GetResult(&pItem);
						if (SUCCEEDED(hr)){
							PWSTR pszFilePath;
							hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
							if (SUCCEEDED(hr)){
								::MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
								CoTaskMemFree(pszFilePath);
							}
							// �ͷ�
							pItem->Release();
						}
					}
					// �ͷ�
					pFileOpen->Release();
				}
				// Uninitializing the COM Library
				CoUninitialize();
			}
			
		}
			break;
		case 4003:
			::MessageBox(hwnd, TEXT("��̬�˵�Edit���Copy"), TEXT("�˵�"), MB_OK); break;
		case 4004:
			::MessageBox(hwnd, TEXT("��̬�˵�Edit���Cut"), TEXT("�˵�"), MB_OK); break;
		case 4005:
			GetWindowRect(hwnd, &rect);
			// �ƶ����ڵ�ָ��λ��
			MoveWindow(hwnd, 0, rect.top,  rect.right-rect.left,rect.bottom-rect.top, 1); break;
		case 4006:
			GetWindowRect(hwnd, &rect);
			// �ƶ����ڵ�ָ��λ��
			MoveWindow(hwnd, rect.left + 200, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1); break;
		case 4007:
			GetWindowRect(hwnd, &rect);
			// �ƶ����ڵ�ָ��λ��
			MoveWindow(hwnd, rect.left, 0, rect.right - rect.left, rect.bottom - rect.top, 1); break;
		case 4008:
			GetWindowRect(hwnd, &rect);
			// �ƶ����ڵ�ָ��λ��
			MoveWindow(hwnd, rect.left, rect.top+200,  rect.right - rect.left, rect.bottom - rect.top, 1); break;
		case 4009:
			int cx = GetSystemMetrics(SM_CXFULLSCREEN);
			int cy = GetSystemMetrics(SM_CYFULLSCREEN);
			char str1[100];
			sprintf(str1,"�����%d\n",cx);
			char str2[100];
			sprintf(str2, "����ߣ�%d", cy);
			strcat(str1, str2);
			wchar_t *wide = new wchar_t[sizeof(str1)];
			MultiByteToWideChar(0, 0, str1, -1, wide, sizeof(str1));
			::MessageBox(hwnd, wide, TEXT("�����С"), MB_OK);
			break;
		}
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}
