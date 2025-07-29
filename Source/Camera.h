#pragma once

#include <DirectXMath.h>

//カメラ
//class Camera
//{
//private:
//    Camera() {}
//    ~Camera() {}
//
//public:
//    // 唯一のインスタンス取得
//    static Camera& Instance()
//    {
//        static Camera camera;
//        return camera;
//    }
//    // 指定方向を向く
//    void SetLookAt(const DirectX::XMFLOAT4& cameraPosition, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);
//
//    // パースペクティブ設定
//    void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);
//
//    // ビュー行列取得
//    const DirectX::XMFLOAT4X4& GetView() const { return view; }
//
//    // プロジェクション行列取得
//    const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }
//
//    // 視点取得
//    const DirectX::XMFLOAT4& GetCameraPosition() const { return cameraPositon; }
//
//    //視点設定 
//    void SetCameraPosition(DirectX::XMFLOAT4 cameraPosition) { this->cameraPositon = cameraPosition; }
//
//    // 注視点取得
//    const DirectX::XMFLOAT4& GetFocus() const { return focus; }
//
//    // 上方向取得
//    const DirectX::XMFLOAT4& GetUp() const { return up; }
//
//    // 前方向取得
//    const DirectX::XMFLOAT3& GetFront() const { return front; }
//
//    // 右方向取得
//    const DirectX::XMFLOAT3& GetRight() const { return right; }
//
//    // 近クリップ距離取得
//    const float& GetNearZ() const { return nearZ; }
//
//    // 遠クリップ距離取得
//    const float& GetFarZ() const { return farZ; }
//
//    //注視点取得
//    const DirectX::XMFLOAT4& GetTarget() { return focus; }
//
//private:
//    DirectX::XMFLOAT4X4		view = { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 };
//    DirectX::XMFLOAT4X4		projection = { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 };
//
//    //DirectX::XMFLOAT4		cameraPositon{ 0, 0, 10,1 };
//    DirectX::XMFLOAT4		cameraPositon{ 0.0f, 0.0f, -10.0f, 1.0f };
//    DirectX::XMFLOAT4		focus{ 0, 0, 0,0 };
//
//    DirectX::XMFLOAT4		up{ 0, 1, 0 ,0 };
//    DirectX::XMFLOAT3		front{ 0, 0, 1 };
//    DirectX::XMFLOAT3		right{ 1, 0, 0 };
//
//    float					nearZ = {};
//    float					farZ = {};
//
//};