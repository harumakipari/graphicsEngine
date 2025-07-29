//#include "CameraController.h"
//
//#include <algorithm>
//#ifdef USE_IMGUI
//#include "..\\imgui\\imgui.h"
//#endif
//
//#include "Camera.h"
//#include "CollisionMesh.h"
//
//void CameraController::Update(float elapsedTime, DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 forward)
//{
//    //pad.Acquire();
//    float ax = pad.ThumbStateRx();  //[a][d]
//    float ay = pad.ThumbStateRy();  //[w][s]
//
//#if 1
//    if (!isGround || ay >= 0.0f)
//    {//�J�������n��ɂȂ��Ƃ� �܂��́@������̓���
//        //�J�����̉�]���x�Ɠ��͒l�ɍ��킹��X����Y������]
//        rotation.x += ay * rollSpeed * elapsedTime;
//    }
//    rotation.y += ax * rollSpeed * elapsedTime;
//    if (rotation.x > DirectX::XMConvertToRadians(80.0f))
//        rotation.x = DirectX::XMConvertToRadians(80.0f);
//    if (rotation.x < DirectX::XMConvertToRadians(-80.0f))
//        rotation.x = DirectX::XMConvertToRadians(-80.0f);
//#else
//    const float angularSpeed = 1.0f;
//    DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&orientation);
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
//    DirectX::XMVECTOR X = R.r[0];
//    DirectX::XMVECTOR Y = R.r[1];
//    DirectX::XMVECTOR Z = R.r[2];
//    Q = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(Q, DirectX::XMQuaternionRotationAxis(Y, ax*elapsedTime)));
//    if (!isGround || ay >= 0.0f)
//    {//�J�������n��ɂȂ��Ƃ� �܂��́@������̓���
//        Q = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(Q, DirectX::XMQuaternionRotationAxis(X, ay * elapsedTime)));
//    }
//    DirectX::XMStoreFloat4(&orientation, Q);
//#endif
//    //// �p�x����N�H�[�^�j�I�����쐬
//    //DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
//
//    //// ��]�s����N�H�[�^�j�I������擾
//    //DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);
//
//    //�ڕW�̃J�����ʒu
//    DirectX::XMFLOAT3 targetPos =
//    {
//        playerPos.x - forward.x * range,
//        playerPos.y + height,
//        playerPos.z - forward.z * range,
//    };
//    Camera& camera = Camera::Instance();
//    DirectX::XMFLOAT4 cameraPosition = camera.GetCameraPosition();
//    target = playerPos;
//    //�J�����̉�]�l����]�s��ɕϊ�
//#if 1
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
//#endif
//    //��]�s�񂩂�O�����x�N�g�������o��
//    DirectX::XMVECTOR Front = R.r[2]/*Z*/;
//    //DirectX::XMVECTOR Front = rotationMatrix.r[2];
//    DirectX::XMFLOAT3 front;
//    DirectX::XMStoreFloat3(&front, Front);
//
//
//    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
//    cameraPosition.x = target.x - front.x * range;
//    cameraPosition.y = target.y - front.y * range;
//    cameraPosition.z = target.z - front.z * range;
//
//    target.y += height;
//
//    //�J�����̎��_�ƒ����_��ݒ�
//    Camera::Instance().SetLookAt(cameraPosition, target, DirectX::XMFLOAT3(0, 1, 0));
//
//
//#ifdef USE_IMGUI
//    ImGui::Begin("Camera");
//    ImGui::DragFloat3("cameraPosition", &cameraPosition.x, 0.5f);
//
//    ImGui::DragFloat("height", &height, 0.1f);
//    ImGui::DragFloat("range", &range, 0.3f);
//    DirectX::XMFLOAT3 angle;
//    angle.x = DirectX::XMConvertToDegrees(rotation.x);
//    angle.y = DirectX::XMConvertToDegrees(rotation.y);
//    angle.z = DirectX::XMConvertToDegrees(rotation.z);
//    ImGui::DragFloat3("Angle", &angle.x, 0.5f);
//    rotation.x = DirectX::XMConvertToRadians(angle.x);
//    rotation.y = DirectX::XMConvertToRadians(angle.y);
//    rotation.z = DirectX::XMConvertToRadians(angle.z);
//    ImGui::DragFloat3("orientation", &orientation.x, 0.5f);
//    ImGui::DragFloat3("rotation", &rotation.x, 0.5f);
//
//    ImGui::End();
//#endif
//
//}
//
////�X�e�[�W�Ƃ̓����蔻��
//void CameraController::Collider(const CollisionMesh* collisionMesh, DirectX::XMFLOAT4X4 transform)
//{
//    using namespace DirectX;
//
//    //DirectX::XMFLOAT4 intersection;
//    std::string mesh;
//    std::string material;
//
//    DirectX::XMFLOAT4 eye =/*{ Camera::Instance().GetCameraPosition()*/
//    {};
//    //XMFLOAT3�ɕϊ�����
//    DirectX::XMFLOAT3 position = { eye.x,eye.y,eye.z };
//    //�X�e�[�W���փ��C�L���X�g���΂�
//    DirectX::XMFLOAT3 rayPosition{ position.x,position.y + 0.5f,position.z };
//    DirectX::XMFLOAT3 rayDirection{ 0, -1, 0 };   //���C�̕���
//    DirectX::XMFLOAT3 intersectionPosition;
//    DirectX::XMFLOAT3 intersectionNormal;
//    std::string intersectionMesh;
//    std::string intersectionMaterial;
//    isGround = false;
//    if (collisionMesh->Raycast(rayPosition, rayDirection, transform, intersectionPosition, intersectionNormal, intersectionMesh, intersectionMaterial))
//    {
//        // �J�����̌��݈ʒu�ƏՓ˒n�_�̋������v�Z
//        float cameraHeight = XMVectorGetX(XMVector3Length(XMLoadFloat3(&position) - XMLoadFloat3(&intersectionPosition)));
//
//        // �n�ʂƂ̍ŏ������i�J�������߂荞�܂Ȃ��悤�ɂ���臒l�j
//        const float minCameraHeight = 1.0f;  // 1.0f �̍������m��
//
//        if (cameraHeight < minCameraHeight)  // �n�ʂƋ߂�����ꍇ
//        {
//
//            // �J������ `y` ���W�𒲐����čŒ���̍������m��
//            position.y = intersectionPosition.y + minCameraHeight;
//
//            DirectX::XMFLOAT4 cameraPosition = { position.x,position.y,position.z,1.0f };
//            // �J�����̈ʒu���X�V
//            Camera::Instance().SetCameraPosition(cameraPosition);
//            isGround = true;
//        }
//
//    }
//}
//
