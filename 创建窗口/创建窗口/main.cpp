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
	// 例 01：
	// 显示一个消息框                     内容                              title           按键
	//::MessageBox(nullptr,TEXT("Hello, win32 Application你好"),TEXT("First Win32 Window"),MB_OK);
	

	// 例 02：
	// 查找是否存在名为"无标题 - 记事本"的窗口
	//HWND hWndo = ::FindWindow(nullptr,TEXT("无标题 - 记事本"));
	//if (hWndo != nullptr){
	//	// 向窗口发送消息   WM_CLOSE 窗口收到该消息后会被关闭
	//	//::SendMessage(hWndo, WM_CLOSE, 0, 0);	// 发消息，等待消息处理结果
	//	::PostMessage(hWndo, WM_CLOSE, 0, 0);	// 发消息，消息发出后立即返回，不等待消息的执行结果
	//}

	// 例 03：
	// 创建一个窗口对象
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);								//对象的大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//窗口风格				水平 或 垂直 方向发生改变 重绘窗口
	wndClass.lpfnWndProc = MainWindProc;							//消息处理函数
	wndClass.cbClsExtra = 0;										//
	wndClass.cbWndExtra = 0;										//
	wndClass.hInstance = hInstance;									//本模块的实例句柄
	wndClass.hIcon = ::LoadIcon(nullptr,IDI_APPLICATION );			//窗口左上角图标的句柄
	wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);			//光标句柄
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);	//背景画刷颜色
	wndClass.lpszMenuName = nullptr;								//菜单名
	wndClass.lpszClassName = TEXT("MainWClass");					//该窗口类的名称
	wndClass.hIconSm = nullptr;										//小图标句柄



	// 注册窗口
	::RegisterClassEx(&wndClass);

	// 创建菜单
	HMENU hMenu = CreateMenu();	// 主菜单
	HMENU hMenuPop = CreateMenu();	// 下拉菜单
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


	// 创建窗口
	HWND hWnd = ::CreateWindowEx(
		0,								// 窗口的扩展风格
		TEXT("MainWClass"),				// 指向注册类名的指针
		TEXT("My First Window"),		// 指向窗口名的指针 
		WS_OVERLAPPEDWINDOW,			// 窗口风格
		CW_USEDEFAULT,					// 窗口的水平位置
		CW_USEDEFAULT,					// 窗口的垂直位置
		CW_USEDEFAULT,					// 窗口的宽度
		CW_USEDEFAULT,					// 窗口的高度
		nullptr,						// 父句柄的句柄
		hMenu,							// 菜单的句柄 或 子窗口的句柄
		hInstance,						// 应用程序实例的句柄
		nullptr							// 指向窗口的创建数据
		);
	if (nullptr == hWnd){
		std::cout << "Create Window Error!" << std::endl;
		return -1;
	}

	// 创建一个button
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

	// 显示窗口
	::ShowWindow(hWnd, nCmdShow);
	// 更新窗口
	::UpdateWindow(hWnd);

	/*
		消息传递过程
		1、系统或用户出发的某个动作
		2、系统将信息存储到MSG结构体中
		3、系统将该消息存储到相关应用程序的消息队列中
	*/
	MSG msg;
	// 消息循环
	while (::GetMessage(&msg, nullptr, 0, 0)){
		::TranslateMessage(&msg);		// 翻译消息
		::DispatchMessage(&msg);		// 不调用处理函数，而是转回给操作系统，消息加工完毕。
	}

	return msg.wParam;
}

// 消息处理函数
LRESULT CALLBACK MainWindProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	switch (message)
	{
	case WM_CREATE:
		// 工厂对象（Factory:factory object）    单线程
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
			// 获取客户区
			GetClientRect(hwnd, &rect);
			// 定义一个矩形范围
			D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
			// 创建渲染（RenderTarget:Direct2D Resources）
			hr = pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),			
				D2D1::HwndRenderTargetProperties(hwnd,size),
				&pRenderTarget
				);
			if (SUCCEEDED(hr)){
				const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
				// 创建画刷（Brush:Direct2D Resources）
				hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
			}

		}
		if (SUCCEEDED(hr)){
			if (pRenderTarget != NULL){
				// 获取客户区的大小
				D2D1_SIZE_F size = pRenderTarget->GetSize();

				const float x = size.width / 2;
				const float y = size.height / 2;
				// 圆的半径，取宽高一半的最小值
				const float radius = min(x, y);
				// 创建圆对象（Ellipse:Direct2D Resources）
				ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
			}
			PAINTSTRUCT ps;
			// （Direct2D Render Loop）
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
			// （Direct2D Render Loop）
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
			// 指定父句柄-->模态对话框  关闭子对话框后才可以对父对话框进行操作
		case 4001:
			::MessageBox(hwnd, TEXT("动态菜单File里的New"), TEXT("菜单"), MB_OK); break;
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
							// 释放
							pItem->Release();
						}
					}
					// 释放
					pFileOpen->Release();
				}
				// Uninitializing the COM Library
				CoUninitialize();
			}
			
		}
			break;
		case 4003:
			::MessageBox(hwnd, TEXT("动态菜单Edit里的Copy"), TEXT("菜单"), MB_OK); break;
		case 4004:
			::MessageBox(hwnd, TEXT("动态菜单Edit里的Cut"), TEXT("菜单"), MB_OK); break;
		case 4005:
			GetWindowRect(hwnd, &rect);
			// 移动窗口到指定位置
			MoveWindow(hwnd, 0, rect.top,  rect.right-rect.left,rect.bottom-rect.top, 1); break;
		case 4006:
			GetWindowRect(hwnd, &rect);
			// 移动窗口到指定位置
			MoveWindow(hwnd, rect.left + 200, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1); break;
		case 4007:
			GetWindowRect(hwnd, &rect);
			// 移动窗口到指定位置
			MoveWindow(hwnd, rect.left, 0, rect.right - rect.left, rect.bottom - rect.top, 1); break;
		case 4008:
			GetWindowRect(hwnd, &rect);
			// 移动窗口到指定位置
			MoveWindow(hwnd, rect.left, rect.top+200,  rect.right - rect.left, rect.bottom - rect.top, 1); break;
		case 4009:
			int cx = GetSystemMetrics(SM_CXFULLSCREEN);
			int cy = GetSystemMetrics(SM_CYFULLSCREEN);
			char str1[100];
			sprintf(str1,"桌面宽：%d\n",cx);
			char str2[100];
			sprintf(str2, "桌面高：%d", cy);
			strcat(str1, str2);
			wchar_t *wide = new wchar_t[sizeof(str1)];
			MultiByteToWideChar(0, 0, str1, -1, wide, sizeof(str1));
			::MessageBox(hwnd, wide, TEXT("桌面大小"), MB_OK);
			break;
		}
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}
