#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#include <vector>

#include "Engine/Utility/Win32Utils.h"
#include "Graphics/Resource/Texture.h"
#include "Graphics/Core/Shader.h"

#include <functional>

#include "Graphics/Core/RenderState.h"

class ComputeParticleSystem
{
public:
	BLEND_STATE blendState = BLEND_STATE::ADD;


	//パーティクルスレッド数
	static constexpr UINT NumParticleThread = 1024;

	//パーティクル生成用構造体
	struct EmitParticleData
	{
		DirectX::XMFLOAT4 parameter{ 1,1,0,-1 };	//x : パーティクル処理タイプ, y : 生存時間, z : 生存時間記録用（セット不要）, w : 生成遅延時間（CPU側で使う用）

		DirectX::XMFLOAT4 position{ 0,0,0,0 };		//生成座標
		DirectX::XMFLOAT4 rotation{ 0,0,0,1 };		//回転
		DirectX::XMFLOAT4 scale{ 1,1,1,1 };			//拡縮 ( xy : startSize, zw : endSize )

		DirectX::XMFLOAT4 velocity{ 0,0,0,0 };		//初速
		DirectX::XMFLOAT4 acceleration{ 0,0,0,0 };	//加速度

		DirectX::XMFLOAT4 color{ 1,1,1,1 };			//色情報

		DirectX::XMFLOAT4 customData;				//カスタムデータ
	};

	//パーティクル構造体
	//アプリケーション側では使用しないが、形式として必要なのでここで宣言しておく
	struct ParticleData
	{
		DirectX::XMFLOAT4 parameter{ 1,1,0,-1 };	//x : パーティクル処理タイプ, y : 生存時間, z : 生存時間記録用（セット不要）, w : 生成遅延時間（CPU側で使う用）

		DirectX::XMFLOAT4 position{ 0,0,0,0 };		//生成座標
		DirectX::XMFLOAT4 rotation{ 0,0,0,0 };		//回転
		DirectX::XMFLOAT4 scale{ 1,1,1,1 };			//拡縮 ( xy : startSize, zw : endSize )

		DirectX::XMFLOAT4 velocity{ 0,0,0,0 };		//初速
		DirectX::XMFLOAT4 acceleration{ 0,0,0,0 };	//加速度

		DirectX::XMFLOAT4 texcoord;					//UV情報
		DirectX::XMFLOAT4 color{ 1,1,1,1 };			//色情報

		DirectX::XMFLOAT4 customData;				//カスタムデータ
	};

	//パーティクルヘッダー構造体
	struct ParticleHeader
	{
		UINT alive;			//生存フラグ
		UINT particleIndex;	//パーティクル番号
		float depth;		//深度
		UINT dummy;
	};

	//汎用情報定義
	struct CommonConstants
	{
		float deltaTime;					//経過時間
		DirectX::XMUINT2 textureSplitCount;	//テクスチャの分割数
		UINT systemNumParticles;			//パーティクル総数
		UINT totalEmitCount;				//現在のフレームでのパーティクル総生成数
		UINT maxEmitParticles;				//現在のフレームでのパーティクル最大生成数
		UINT commonDummy[2];
	};

	//バイトニックソート情報定義
	struct BitonicSortConstants
	{
		UINT increment;
		UINT direction;
		UINT dummy[2];
	};
	static constexpr UINT BitonicSortB2Thread = 256;
	static constexpr UINT BitonicSortC2Thread = 512;

public:
	ComputeParticleSystem(ID3D11Device* device, UINT particlesCount, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView,
		DirectX::XMUINT2 splitCount = DirectX::XMUINT2(1, 1));
	~ComputeParticleSystem();

	void Emit(const EmitParticleData& data);
	void PixelEmitBegin(ID3D11DeviceContext* immediateContext, float deltaTime);
	void PixelEmitEnd(ID3D11DeviceContext* immediateContext);

	void Update(ID3D11DeviceContext* immediateContext, float deltaTime);
	void Render(ID3D11DeviceContext* immediateContext);

	void DrawGUI();

private:
	UINT numParticles;
	UINT numEmitParticles;
	bool oneShotInitialize;
	DirectX::XMUINT2 textureSplitCount;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	std::vector<EmitParticleData> pendingParticles;//エミット待ちパーティクル
	std::vector<EmitParticleData> emitParticles;//現在のフレームでエミットするパーティクル
	Microsoft::WRL::ComPtr<ID3D11Buffer> commonConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> bitonicSortConstantBuffer;

	//パーティクルバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> particleDataBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleDataShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particleDataUnordredAccessView;

	//未使用パーティクル番号を格納したAppend/Cosumeバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> particleAppendConsumeBuffer;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particleAppendConsumeUnordredAccessView;

	//パーティクル生成情報を格納したバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> particleEmitBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleEmitShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particleEmitUnordredAccessView;

	//DispatchIndirect用構造体
	using DispatchIndirect = DirectX::XMUINT3; //UINT3で十分

	//	00バイト目：現在のパーティクル総数
	//	04バイト目：1フレーム前のパーティクル総数
	//	08バイト目：パーティクル破棄数
	//	12バイト目：パーティクル生成用DispatchIndirect情報
	static constexpr UINT NumCurrentParticleOffset = 0;
	static constexpr UINT NumPreviousParticleOffset = NumCurrentParticleOffset + sizeof(UINT);
	static constexpr UINT NumDeadParticleOffset = NumPreviousParticleOffset + sizeof(UINT);
	static constexpr UINT EmitDispatchIndirectOffset = NumDeadParticleOffset + sizeof(UINT);
	//DrawInstanced用DrawIndirect用構造体
	struct DrawIndirect
	{
		UINT vertexCountPerInstance;
		UINT instanceCount;
		UINT startVertexLocation;
		UINT startInstanceLocation;
	};
	//	24バイト目：パーティクル更新用DispatchIndirect情報
	//	36バイト目：パーティクル生成時に使用するインデックス(Append/Consumeの代わり)
	//	40バイト目：DrawIndirect情報
	//	40バイト目：ピクセルパーティクル生成数カウンター
	//	44バイト目：DrawIndirect情報
	static constexpr UINT UpdateDispatchIndirectOffset = EmitDispatchIndirectOffset + sizeof(DispatchIndirect);
	static constexpr UINT NumEmitParticleIndexOffset = UpdateDispatchIndirectOffset + sizeof(DispatchIndirect);
	static constexpr UINT NumEmitPixelParticleIndirectOffset = NumEmitParticleIndexOffset + sizeof(UINT);
	static constexpr UINT DrawIndirectOffset = NumEmitPixelParticleIndirectOffset + sizeof(UINT);

	static constexpr UINT DrawIndirectSize = DrawIndirectOffset + sizeof(DrawIndirect);

	//DrawIndirectを用いるため、RWStructuredBufferを用いるものに変更
	Microsoft::WRL::ComPtr<ID3D11Buffer> indirectDataBuffer;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> indirectDataUnordredAccessView;

	//パーティクルヘッダーバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> particleHeaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleHeaderShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particleHeaderUnordredAccessView;

	//各種シェーダー
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> initShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> emitShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> updateShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> beginFrameShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> endFrameShader;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> sortB2Shader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> sortC2Shader;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
};