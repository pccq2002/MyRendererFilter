#include"stdafx.h"

extern "C" LRESULT CALLBACK MyWndProc
	(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		return DefWindowProc(hWnd, message, wParam, lParam);
}

CMyRenderer::CMyRenderer(IUnknown *pUnk, HRESULT *phr) :
	CBaseVideoRenderer(CLSID_MyRenderer, FILTER_NAME , pUnk, phr)
	, m_Hwnd(NULL)
{
}

CMyRenderer::~CMyRenderer() {
}

CUnknown * WINAPI CMyRenderer::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr) {
	CMyRenderer *pNewFilter = new CMyRenderer(pUnk, phr);
	if (pNewFilter == NULL) {
		*phr = E_OUTOFMEMORY;
	}
	return pNewFilter;
}

HRESULT CMyRenderer::CheckMediaType(const CMediaType *pMediaType) {
	if(!IsEqualGUID(*pMediaType->FormatType(), FORMAT_VideoInfo)) {
		return S_FALSE;
	}
	if(IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Video)) {
		if(IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB32)) {
			return S_OK;
		}
	}
	return S_FALSE;
}

HRESULT CMyRenderer::SetMediaType(const CMediaType *pmt) {
	// データフォーマットを記憶しておく
	CopyMemory(&m_Vih,(const VIDEOINFOHEADER *)pmt->pbFormat,sizeof(VIDEOINFOHEADER));
	WNDCLASSEX wcex={sizeof(WNDCLASSEX)};
	//既にウィンドウが開いていたら、閉じる
	if(m_Hwnd!=NULL) {
		DestroyWindow(m_Hwnd);
	}
	// 表示ウィンドウの作成
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)MyWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= GetModuleHandle(NULL);
	wcex.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)CreateSolidBrush(0);
	wcex.lpszClassName	= TEXT("myRendererClass");
	ATOM a=RegisterClassEx(&wcex);

	int height=m_Vih.bmiHeader.biHeight;
	if (height<0) height = -height;

	m_Hwnd=CreateWindow(TEXT("myRendererClass"),TEXT("My Renderer Window"), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT
		,m_Vih.bmiHeader.biWidth, height,
		GetDesktopWindow(), NULL, GetModuleHandle(NULL), NULL );
	ShowWindow(m_Hwnd, SW_NORMAL);
	UpdateWindow(m_Hwnd);
	return S_OK;
}

HRESULT CMyRenderer::JoinFilterGraph(IFilterGraph *pGraph,LPCWSTR pName) {
	if(pGraph==NULL) {
		DestroyWindow(m_Hwnd);
	}
	return CBaseVideoRenderer::JoinFilterGraph(pGraph,pName);
}

HRESULT CMyRenderer::DoRenderSample(IMediaSample *pMediaSample) {
	HRESULT hr=S_OK;
	LPBYTE pbSrc=NULL;
	const long lActualSize=pMediaSample->GetActualDataLength();
	pMediaSample->GetPointer(&pbSrc);
	if(IsWindow(m_Hwnd)==TRUE) {
		BITMAPINFO bi={m_Vih.bmiHeader};
		const HDC hdc=GetDC(m_Hwnd);
		RECT rect;
		GetClientRect(m_Hwnd,&rect);
		int height=m_Vih.bmiHeader.biHeight;
		if (height<0) height = -height;
		StretchDIBits(hdc,0,0,rect.right,rect.bottom
			,0,0,m_Vih.bmiHeader.biWidth , height
			,pbSrc,&bi,DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(m_Hwnd,hdc);
	}
	return hr;
}
