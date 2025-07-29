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
//    {//カメラが地上にないとき または　上向きの入力
//        //カメラの回転速度と入力値に合わせてX軸とY軸を回転
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
//    {//カメラが地上にないとき または　上向きの入力
//        Q = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(Q, DirectX::XMQuaternionRotationAxis(X, ay * elapsedTime)));
//    }
//    DirectX::XMStoreFloat4(&orientation, Q);
//#endif
//    //// 角度からクォータニオンを作成
//    //DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
//
//    //// 回転行列をクォータニオンから取得
//    //DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);
//
//    //目標のカメラ位置
//    DirectX::XMFLOAT3 targetPos =
//    {
//        playerPos.x - forward.x * range,
//        playerPos.y + height,
//        playerPos.z - forward.z * range,
//    };
//    Camera& camera = Camera::Instance();
//    DirectX::XMFLOAT4 cameraPosition = camera.GetCameraPosition();
//    target = playerPos;
//    //カメラの回転値を回転行列に変換
//#if 1
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
//#endif
//    //回転行列から前方向ベクトルを取り出す
//    DirectX::XMVECTOR Front = R.r[2]/*Z*/;
//    //DirectX::XMVECTOR Front = rotationMatrix.r[2];
//    DirectX::XMFLOAT3 front;
//    DirectX::XMStoreFloat3(&front, Front);
//
//
//    //注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
//    cameraPosition.x = target.x - front.x * range;
//    cameraPosition.y = target.y - front.y * range;
//    cameraPosition.z = target.z - front.z * range;
//
//    target.y += height;
//
//    //カメラの視点と注視点を設定
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
////ステージとの当たり判定
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
//    //XMFLOAT3に変換する
//    DirectX::XMFLOAT3 position = { eye.x,eye.y,eye.z };
//    //ステージ床へレイキャストを飛ばす
//    DirectX::XMFLOAT3 rayPosition{ position.x,position.y + 0.5f,position.z };
//    DirectX::XMFLOAT3 rayDirection{ 0, -1, 0 };   //レイの方向
//    DirectX::XMFLOAT3 intersectionPosition;
//    DirectX::XMFLOAT3 intersectionNormal;
//    std::string intersectionMesh;
//    std::string intersectionMaterial;
//    isGround = false;
//    if (collisionMesh->Raycast(rayPosition, rayDirection, transform, intersectionPosition, intersectionNormal, intersectionMesh, intersectionMaterial))
//    {
//        // カメラの現在位置と衝突地点の距離を計算
//        float cameraHeight = XMVectorGetX(XMVector3Length(XMLoadFloat3(&position) - XMLoadFloat3(&intersectionPosition)));
//
//        // 地面との最小距離（カメラがめり込まないようにする閾値）
//        const float minCameraHeight = 1.0f;  // 1.0f の高さを確保
//
//        if (cameraHeight < minCameraHeight)  // 地面と近すぎる場合
//        {
//
//            // カメラの `y` 座標を調整して最低限の高さを確保
//            position.y = intersectionPosition.y + minCameraHeight;
//
//            DirectX::XMFLOAT4 cameraPosition = { position.x,position.y,position.z,1.0f };
//            // カメラの位置を更新
//            Camera::Instance().SetCameraPosition(cameraPosition);
//            isGround = true;
//        }
//
//    }
//}
//
