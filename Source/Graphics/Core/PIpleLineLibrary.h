#ifndef PIPELINELIBRARY_H
#define PIPELINELIBRARY_H

#include <string>
#include <unordered_map>

#include "Graphics/Core/PipelineState.h"
#include "Graphics/Core/Shader.h"
#include "Engine/Utility/Win32Utils.h"

class PipeLineState
{
private:

};

class PipeLineStateSet
{
public:
    const PipeLineStateDesc& Get(const std::string& name)const
    {
        auto it = sets_.find(name);
        if (it == sets_.end())
        {// 見つからなかったら
            _ASSERT(L"このパイプラインステートは設定されていません。");
        }
        return it->second;
    }

    PipeLineStateSet()
    {
        sets_["opaque"].rasterState = RASTERRIZER_STATE::SOLID_CULL_BACK;
        sets_["opaque"].blendState = BLEND_STATE::MULTIPLY_RENDER_TARGET_NONE;
        sets_["opaque"].depthState = DEPTH_STATE::ZT_ON_ZW_ON;
        //sets_["opaque"].inputLayout=

    }

    void Initialize(ID3D11Device* device)
    {
        HRESULT hr = S_OK;
        // StaticMesh用
        {
            PipeLineStateDesc desc;
            
            D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            hr = CreateVsFromCSO(device, "./Shader/GltfModelStaticBatchingVS.cso", desc.vertexShader.ReleaseAndGetAddressOf(), desc.inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, _countof(inputElementDesc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = CreateVsFromCSO(device, "./Shader/GltfModelStaticBatchingCsmVS.cso", desc.cascadShadowVertexShader.ReleaseAndGetAddressOf(), NULL, NULL, 0);
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        }
    }
private:
    std::unordered_map<std::string, PipeLineStateDesc> sets_;
};




#endif // !PIPELINELIBRARY_H
