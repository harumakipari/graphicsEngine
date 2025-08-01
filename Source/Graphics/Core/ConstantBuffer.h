#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <assert.h>
#include "Engine/Utility/Win32Utils.h"


/*
	// framework.h
	struct constants_t
	{
		float params[4];
	};
	std::unique_ptr <constant_buffer<constants_t>> constants;

	// framework::initialize
	constants = std::make_unique<constant_buffer<constants_t>>(device.Get());

	// framework::update/render
	constants->data.params[0] = 0;
	constants->activate(immediate_context.Get(), 0, true, true);
*/

#define USAGE_DYNAMIC

template <class T>
struct constant_buffer
{
	T data;
	constant_buffer(ID3D11Device* device)
	{
		assert(sizeof(T) % 16 == 0 && L"constant buffer's need to be 16 byte aligned");

		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(T);
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
#ifdef USAGE_DYNAMIC
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = device->CreateBuffer(&buffer_desc, 0, buffer_object.GetAddressOf());
		assert(SUCCEEDED(hr) && hr_trace(hr));
#else
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA subresource_data;
		subresource_data.pSysMem = &data;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, buffer_object.GetAddressOf());
		assert(SUCCEEDED(hr) && hr_trace(hr));
#endif
	}
	virtual ~constant_buffer() = default;
	constant_buffer(constant_buffer&) = delete;
	constant_buffer& operator =(constant_buffer&) = delete;

	void activate(ID3D11DeviceContext* immediate_context, int slot, bool vs_on = true, bool ps_on = true)
	{
		HRESULT hr = S_OK;
#ifdef USAGE_DYNAMIC
		D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mapped_buffer;

		hr = immediate_context->Map(buffer_object.Get(), 0, map, 0, &mapped_buffer);
		assert(SUCCEEDED(hr) && hr_trace(hr));
		memcpy_s(mapped_buffer.pData, sizeof(T), &data, sizeof(T));
		immediate_context->Unmap(buffer_object.Get(), 0);
#else
		immediate_context->UpdateSubresource(buffer_object.Get(), 0, 0, &data, 0, 0);
#endif
		if (vs_on)
		{
			immediate_context->VSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
		if (ps_on)
		{
			immediate_context->PSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
	}
	void deactivate(ID3D11DeviceContext* immediate_context)
	{
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_object;

};
