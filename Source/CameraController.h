#pragma once
#include <DirectXMath.h>
#include "Engine/Input/GamePad.h"

class CollisionMesh;

// カメラコントローラー
class CameraController
{
public:
    CameraController() {};
    ~CameraController() {}

    // 更新処理
    void Update(float elapsedTime, DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 forward);

    //ステージとの当たり判定
    void Collider(const CollisionMesh* collisionMesh, DirectX::XMFLOAT4X4 transform);

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3 target) { this->target = target; }

private:
    DirectX::XMFLOAT3 rotation{ 0.26f,-3.2f,0.0f };
    DirectX::XMFLOAT3 target{};

    //ワールド空間での姿勢
    DirectX::XMFLOAT4 orientation{ 0,0,0,1 }; // x軸 * sinΘ/2 ,y軸 * sinΘ/2 ,z軸 * sinΘ/2 , cosΘ/2

    //回転速度
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float range = 8.2f;
    float height = 0.7f;
    //float range = 1.5f;
    //float height = 1.6f;
    GamePad pad;

    bool isGround = false;
};
