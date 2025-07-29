#ifndef PIPELINE_STATE_H
#define PIPELINE_STATE_H

#include <d3d11.h>
#include <wrl.h>

struct PipeLineState
{
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader = nullptr;
    Microsoft::WRL::ComPtr <ID3D11VertexShader> vertexShader = nullptr;
    Microsoft::WRL::ComPtr <ID3D11HullShader> hullShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DomainShader> domainShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> gemetryShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
};

#endif // !PIPELINE_STATE_H
