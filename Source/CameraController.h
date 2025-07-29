#pragma once
#include <DirectXMath.h>
#include "Engine/Input/GamePad.h"

class CollisionMesh;

// �J�����R���g���[���[
class CameraController
{
public:
    CameraController() {};
    ~CameraController() {}

    // �X�V����
    void Update(float elapsedTime, DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 forward);

    //�X�e�[�W�Ƃ̓����蔻��
    void Collider(const CollisionMesh* collisionMesh, DirectX::XMFLOAT4X4 transform);

    //�^�[�Q�b�g�ʒu�ݒ�
    void SetTarget(const DirectX::XMFLOAT3 target) { this->target = target; }

private:
    DirectX::XMFLOAT3 rotation{ 0.26f,-3.2f,0.0f };
    DirectX::XMFLOAT3 target{};

    //���[���h��Ԃł̎p��
    DirectX::XMFLOAT4 orientation{ 0,0,0,1 }; // x�� * sin��/2 ,y�� * sin��/2 ,z�� * sin��/2 , cos��/2

    //��]���x
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float range = 8.2f;
    float height = 0.7f;
    //float range = 1.5f;
    //float height = 1.6f;
    GamePad pad;

    bool isGround = false;
};
