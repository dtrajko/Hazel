// Copyright (c) 2019  PardCode.
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Window.h"

#include <string>


Window* window = nullptr;

Window::Window()
{
	m_hwnd = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			// Event fired when the window is created
			window->setHWND(hwnd);
			window->onCreate();
			break;
		}
		case WM_SETFOCUS:
		{
			// Event fired when the window is in focus
			window->onFocus();
			break;
		}
		case WM_KILLFOCUS:
		{
			// Event fired when the window is in focus
			window->onKillFocus();
			break;
		}
		case WM_DESTROY:
		{
			// Event fired when the window is destroyed
			window->onDestroy();
			::PostQuitMessage(0);
			break;
		}
		default:
		{
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}

	return NULL;
}

bool Window::init()
{
	LPCWSTR className = L"MyWindowClass";
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = className;
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc)) // If the registration of class fails, the function returns false
	{
		return false;
	}

	if (!window)
	{
		window = this;
	}

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, className, L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1600, 900,
		NULL, NULL, NULL, NULL);

	if (!m_hwnd)
	{
		return false;
	}

	// Show up the window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	// Set this flag to true to indicate that the window is initialized and running
	m_is_run = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;

	window->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}

bool Window::release()
{
	// Destroy the window
	if (!::DestroyWindow(m_hwnd))
	{
		return false;
	}
	return true;
}

bool Window::isRunning()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rect;
	::GetClientRect(this->m_hwnd, &rect);
	return rect;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::onCreate()
{

}

void Window::onUpdate()
{

}

void Window::onDestroy()
{
	m_is_run = false;
}

void Window::onFocus()
{

}

void Window::onKillFocus()
{

}

Window::~Window()
{

}
