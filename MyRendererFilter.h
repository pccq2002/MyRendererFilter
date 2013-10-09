#pragma once

#define TEMPLATE_NAME	(L"My Renderer Filter")
#define FILTER_NAME		(TEMPLATE_NAME)
#define INPUT_PIN_NAME  (L"Input")

// {548DA892-F22C-408d-B759-CEF9487E4D81}
DEFINE_GUID(CLSID_MyRenderer, 
			0x548da892, 0xf22c, 0x408d, 0xb7, 0x59, 0xce, 0xf9, 0x48, 0x7e, 0x4d, 0x81);

// ピンタイプの定義
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
	&MEDIATYPE_Video,        // Major type
	&MEDIASUBTYPE_RGB32      // Minor type
};
// 入力ピンの情報
const AMOVIESETUP_PIN sudPins =
{
	INPUT_PIN_NAME,
	FALSE,                // Is it rendered
	FALSE,                // Is it an output (入力ピンのため FALSE を指定)
	FALSE,                // Allowed none
	FALSE,                // Allowed many
	&CLSID_NULL,
	NULL,
	1,                    // Number of types
	&sudPinTypes          // Pin information
};

// フィルタ情報
const AMOVIESETUP_FILTER afFilterInfo=
{
	&CLSID_MyRenderer,      // フィルタのCLSID
	FILTER_NAME,			// フィルタ名
	MERIT_DO_NOT_USE,       // メリット値
	1,                      // ピン数
	&sudPins                // ピン情報
};

// フィルタクラス
class CMyRenderer : public CBaseVideoRenderer {
public:
	CMyRenderer(LPUNKNOWN pUnk,HRESULT *phr);
	virtual						~CMyRenderer();
	static CUnknown * WINAPI	CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
	HRESULT CheckMediaType(const CMediaType *pMediaType);
	HRESULT DoRenderSample(IMediaSample *pMediaSample);
	HRESULT SetMediaType(const CMediaType *pmt);
	STDMETHODIMP JoinFilterGraph(IFilterGraph * pGraph, LPCWSTR pName);
protected:
private:
	VIDEOINFOHEADER	    m_Vih;
	HWND				m_Hwnd;
};
