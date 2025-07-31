#ifndef GBUFFER_H
#define GBUFFER_H

#include "MultipleRenderTargets.h"

class GBuffer :public MultipleRenderTargets
{
public:
    static constexpr UINT GBufferCount = 4;
    static constexpr DXGI_FORMAT GBufferFormats[GBufferCount] =
    {
        DXGI_FORMAT_R8G8B8A8_UNORM,  //  albedo Opacity + Mask??とか？
        DXGI_FORMAT_R16G16B16A16_FLOAT, // Normal
        DXGI_FORMAT_R32G32B32A32_FLOAT, // Position
        DXGI_FORMAT_R32G32B32A32_FLOAT, // マテリアルの情報？
    };

    GBuffer(ID3D11Device* device, UINT width, UINT height) :MultipleRenderTargets(device, width, height, GBufferCount, GBufferFormats)
    {

    }


};

#endif // !GBUFFER_H

