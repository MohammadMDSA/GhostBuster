//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept
{
    m_directResource = std::make_unique<DirectResource>(D3D_FEATURE_LEVEL_9_2);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(::IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation)
{
    m_directResource->SetWindow(window);
    m_directResource->SetWindowSize(std::max(width, 1), std::max(height, 1));
    m_directResource->SetWindowRotation(rotation);

    m_directResource->CreateDevice();

    m_directResource->CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_directResource->GetDeviceContext()->ClearRenderTargetView(m_directResource->GetRenderTargetView().Get(), Colors::CornflowerBlue);
    m_directResource->GetDeviceContext()->ClearDepthStencilView(m_directResource->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_directResource->GetDeviceContext()->OMSetRenderTargets(1, m_directResource->GetRenderTargetView().GetAddressOf(), m_directResource->GetDepthStencilView().Get());

    // Set the viewport.
    m_directResource->SetViewPort();
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_directResource->GetSwapChain()->Present(1, 0);

    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    m_directResource->GetDeviceContext()->DiscardView(m_directResource->GetRenderTargetView().Get());

    // Discard the contents of the depth stencil.
    m_directResource->GetDeviceContext()->DiscardView(m_directResource->GetDepthStencilView().Get());

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        m_directResource->OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    m_directResource->GetDeviceContext()->ClearState();

    ComPtr<IDXGIDevice3> dxgiDevice;
    if (SUCCEEDED(m_directResource->GetDevice().As(&dxgiDevice)))
    {
        dxgiDevice->Trim();
    }

    // TODO: Game is being power-suspended.
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed.
}

void Game::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation)
{
    m_directResource->OnWindowSizeChanged(width, height, rotation);

    // TODO: Game window is being resized.
}

void Game::ValidateDevice()
{
    // The D3D Device is no longer valid if the default adapter changed since the device
    // was created or if the device has been removed.

    DXGI_ADAPTER_DESC previousDesc;
    {
        ComPtr<IDXGIDevice3> dxgiDevice;
        DX::ThrowIfFailed(m_directResource->GetDevice().As(&dxgiDevice));

        ComPtr<IDXGIAdapter> deviceAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(deviceAdapter.GetAddressOf()));

        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIAdapter1> previousDefaultAdapter;
        DX::ThrowIfFailed(dxgiFactory->EnumAdapters1(0, previousDefaultAdapter.GetAddressOf()));

        DX::ThrowIfFailed(previousDefaultAdapter->GetDesc(&previousDesc));
    }

    DXGI_ADAPTER_DESC currentDesc;
    {
        ComPtr<IDXGIFactory2> currentFactory;
        DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(currentFactory.GetAddressOf())));

        ComPtr<IDXGIAdapter1> currentDefaultAdapter;
        DX::ThrowIfFailed(currentFactory->EnumAdapters1(0, currentDefaultAdapter.GetAddressOf()));

        DX::ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));
    }

    // If the adapter LUIDs don't match, or if the device reports that it has been removed,
    // a new D3D device must be created.

    if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart
        || previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart
        || FAILED(m_directResource->GetDevice()->GetDeviceRemovedReason()))
    {
        // Create a new device and swap chain.
        m_directResource->OnDeviceLost();
    }
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
