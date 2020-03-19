#pragma once
class DirectResource
{

public:

	DirectResource(D3D_FEATURE_LEVEL fl);
	~DirectResource();


	D3D_FEATURE_LEVEL GetFeatureLevel() { return m_featureLevel; }
	Microsoft::WRL::ComPtr<ID3D11Device3> GetDevice() { return m_d3dDevice; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext3> GetDeviceContext() { return m_d3dContext; }
	Microsoft::WRL::ComPtr<IDXGISwapChain3> GetSwapChain() { return m_swapChain; }
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return m_renderTargetView; }
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return m_depthStencilView; }
	void SetWindowSize(int width, int heigth);
	void SetWindow(::IUnknown* window);
	void SetWindowRotation(DXGI_MODE_ROTATION rot);

	void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);

	void SetViewPort();

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	void ValidateDevice();


private:

	D3D_FEATURE_LEVEL                               m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device3>           m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext3>    m_d3dContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain3>         m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	IUnknown* m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;
	DXGI_MODE_ROTATION                              m_outputRotation;

};

