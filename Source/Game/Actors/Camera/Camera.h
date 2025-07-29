#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

#include "Core/Actor.h"
#include "Core/ActorManager.h"

#include "Components/Camera/CameraComponent.h"
#include "Components/CollisionShape/ShapeComponent.h"
#include "Physics/PhysicsUtility.h"

#include "Game/Actors/Stage/Building.h"
#include "Game/Actors/Stage/BossBuilding.h"
#include "Game/Utils/ShockWaveTargetRegistry.h"
#include "Game/Actors/Enemy/RiderEnemy.h"

#include "Game/Managers/GameManager.h"
class Camera :public Actor
{
public:
    //引数付きコンストラクタ
    Camera(std::string actorName) :Actor(actorName) {}

    virtual ~Camera() = default;
    std::shared_ptr<CameraComponent> mainCameraComponent;
    std::shared_ptr<SphereComponent> sphereComponent;
    void Initialize()override
    {
        std::shared_ptr<SpringArmComponent> springArmComponent = this->NewSceneComponent<class SpringArmComponent>("springArm");
        springArmComponent->SetRelativeEulerRotationDirect({ 40.0f, 0.0f, 0.0f });
        //springArmComponent->SetRelativeLocationDirect({ 1.5f,9.0f,-10.9f });
        mainCameraComponent = this->NewSceneComponent<class CameraComponent>("mainCamera", "springArm");
        mainCameraComponent->SetPerspective(DirectX::XMConvertToRadians(45), Graphics::GetScreenWidth() / Graphics::GetScreenHeight(), 0.1f, 1000.0f);

        // 当たり判定のコンポーネントを追加
        sphereComponent = this->NewSceneComponent<class SphereComponent>("sphereComponent", "springArm");
        sphereComponent->SetRadius(0.2f);
        //sphereComponent->SetMass(40.0f);
        sphereComponent->SetLayer(CollisionLayer::Camera);
        sphereComponent->SetResponseToLayer(CollisionLayer::Building, CollisionComponent::CollisionResponse::Trigger);
        sphereComponent->SetResponseToLayer(CollisionLayer::Camera, CollisionComponent::CollisionResponse::None);
        sphereComponent->Initialize();
        sphereComponent->SetIsVisibleDebugBox(false);
        sphereComponent->SetIsVisibleDebugShape(false);
        sphereComponent->DisableCollision();
    };

    //更新処理
    void Update(float deltaTime)override
    {
        using namespace DirectX;
        switch (state)
        {
        case Camera::State::Normal:
        {
            mainCameraComponent->customTarget = true;

            DirectX::XMVECTOR vOld = XMLoadFloat3(&oldTarget);
            DirectX::XMVECTOR vNew = XMLoadFloat3(&target);

            float speed = 2.0f;
            float t = std::clamp(deltaTime * speed, 0.0f, 1.0f);

            XMVECTOR vInterp = XMVectorLerp(vOld, vNew, t);
            XMFLOAT3 interpTarget;
            DirectX::XMStoreFloat3(&interpTarget, vInterp);

            DirectX::XMFLOAT3 clampedTarget;
            clampedTarget.x = std::clamp(interpTarget.x, cameraMin.x, cameraMax.x);
            clampedTarget.y = interpTarget.y;
            clampedTarget.z = std::clamp(interpTarget.z, cameraMin.z, cameraMax.z);

            // 補間済み目標位置を使って、eye にオフセットを適用
            DirectX::XMFLOAT3 eye;
            eye.x = clampedTarget.x + offset.x;
            eye.y = clampedTarget.y + offset.y;
            eye.z = clampedTarget.z + offset.z;
            SetPosition(eye);

            oldTarget = clampedTarget;

            //DirectX::XMFLOAT3 eye = GetPosition();
            DirectX::XMVECTOR eyeVec = DirectX::XMLoadFloat3(&eye);
            DirectX::XMFLOAT3 tar = target;
            tar.y += 0.5f;// player の腰当たり
            DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&tar);
            DirectX::XMVECTOR dirVec = DirectX::XMVectorSubtract(targetVec, eyeVec);
            DirectX::XMVECTOR dirNormalized = DirectX::XMVector3Normalize(dirVec);
            // 前フレームのターゲット
            preTarget = tar;
            mainCameraComponent->_target = clampedTarget;
            // lerp 先の position を保存
            afterTarget = clampedTarget;
            afterEye = eye;
            RaycastHit result;
            DirectX::XMFLOAT3 origin = GetPosition();
            DirectX::XMFLOAT3 direction;
            direction.x = mainCameraComponent->GetView()._31;
            direction.y = -mainCameraComponent->GetView()._32;
            direction.z = mainCameraComponent->GetView()._33;
            DirectX::XMVECTOR distanceVec = DirectX::XMVector3Length(dirVec);
            float distance;
            DirectX::XMVECTOR DirVec = DirectX::XMLoadFloat3(&direction);
            DirVec = DirectX::XMVector3Normalize(DirVec);
            DirectX::XMStoreFloat3(&direction, DirVec);
            DirectX::XMStoreFloat(&distance, distanceVec);
            if (PhysicsTest::Instance().SphereCast(origin, direction, distance, 0.4f, result, CollisionHelper::ToBit(CollisionLayer::Camera),     // myLayer
                CollisionHelper::ToBit(CollisionLayer::Building)))   // wantHitRayer)
            {
                if (auto build = dynamic_cast<Building*>(result.actor))
                {
                    build->preSkeltalMeshComponent->model->SetAlpha(0.3f);
                }
                else if (auto bossBuild = dynamic_cast<BossBuilding*>(result.actor))
                {
                    bossBuild->preSkeltalMeshComponent->model->SetAlpha(0.3f);
                }
            }
            else
            {
                for (const auto& actor : ShockWaveTargetRegistry::GetTargets())
                {
                    if (auto build = std::dynamic_pointer_cast<Building>(actor))
                    {
                        build->preSkeltalMeshComponent->model->SetAlpha(1.0f);
                    }
                    else if (auto bossBuild = std::dynamic_pointer_cast<BossBuilding>(actor))
                    {
                        bossBuild->preSkeltalMeshComponent->model->SetAlpha(1.0f);
                    }
                }
            }
        }
        break;
        case Camera::State::BossTarget:
        {
            if (auto enemy = std::dynamic_pointer_cast<RiderEnemy>(ActorManager::GetActorByName("enemy")))
            {// 敵の位置を取る
                mainCameraComponent->customTarget = true;
                DirectX::XMFLOAT3 targetVec = enemy->bossJointComponent->GetComponentWorldTransform().GetLocation();
                mainCameraComponent->_target = targetVec;
                //DirectX::XMFLOAT3 eye;
                //eye.x = 1.285f;
                //eye.y = 12.183f;
                //eye.z = -22.4f;
                DirectX::XMFLOAT3 playerFirstPos = { 0.7f,0.8f,-9.5f };
                playerFirstPos.y += 1.5f;
                playerFirstPos.x += distanceX;
                playerFirstPos.y += distanceY;
                float enemyY = enemy->GetPosition().y;
                float startY = 2.3f;
                float endY = 0.8f;

                if (enemyY <= startY)
                {
                    float t = std::clamp((startY - enemyY) / (startY - endY), 0.0f, 1.0f);
                    distanceZ = 3.8f * t;

                    // Y が endY 付近に到達して、shakeしていないとき
                    if (!didShake && enemyY <= endY - 0.7f)
                    {
                        Shake(0.5f, 0.4f);
                        didShake = true;
                    }

                }
                playerFirstPos.z += distanceZ;
                SetPosition(playerFirstPos); // プレイヤーの位置をEyeにする
                if (isFinishFirstPerf)
                {// 最初の演出が終わったら
                    // その時のカメラの位置
                    preEye = playerFirstPos;
                    // その時の focus の位置
                    preTarget = targetVec;
                    state = State::Lerp;
                    elapsedTime = 0.0f;
                    isFinishFirstPerf = false;
                }
            }
        }
        break;
        case Camera::State::Lerp:
        {
            elapsedTime += deltaTime;
            float lerpTime = 1.5f;
            float t = std::clamp(elapsedTime / lerpTime, 0.0f, 1.0f);
            DirectX::XMVECTOR PreTargetVec = DirectX::XMLoadFloat3(&preTarget);
            DirectX::XMVECTOR PreEyeVec = DirectX::XMLoadFloat3(&preEye);

            // 最初の　target　はプレイヤーの初期位置
            DirectX::XMVECTOR AftTargetVec = DirectX::XMLoadFloat3(&afterTarget);
            DirectX::XMVECTOR AftEyeVec = DirectX::XMLoadFloat3(&afterEye);
            //DirectX::XMVECTOR AftTargetVec = DirectX::XMLoadFloat3(&clampedTarget);
            //DirectX::XMVECTOR AftEyeVec = DirectX::XMLoadFloat3(&eye);
            //DirectX::XMVECTOR AftEyeVec = DirectX::XMVectorSet(1.317f, 12.2f, -22.4f, 1.0f);
            //DirectX::XMVECTOR AftEyeVec = DirectX::XMVectorSet(1.297f, 12.197f, -22.4f, 1.0f);
            //DirectX::XMVECTOR AftEyeVec = DirectX::XMVectorSet(1.297f, 12.197f, -22.4f, 1.0f);
            DirectX::XMVECTOR NowTar = XMVectorLerp(PreTargetVec, AftTargetVec, t);
            DirectX::XMVECTOR NowEye = XMVectorLerp(PreEyeVec, AftEyeVec, t);
            DirectX::XMFLOAT3 nowTarget, nowEye;
            DirectX::XMStoreFloat3(&nowTarget, NowTar);
            DirectX::XMStoreFloat3(&nowEye, NowEye);
            mainCameraComponent->customTarget = true;
            mainCameraComponent->_target = nowTarget;
            SetPosition(nowEye);
            if (t >= 1.0f)
            {// lerp し終わったら
                state = State::Normal;
                // ここでカウントがスタートする
                GameManager::GameCountStart();
            }
        }
        break;
        default:
            break;
        }
#if 0
        if (isTargetBoss)
        {
#if 0
            if (auto enemy = std::dynamic_pointer_cast<RiderEnemy>(ActorManager::GetActorByName("enemy")))
            {// 敵の位置を取る
                XMFLOAT3 forward = enemy->GetForward();
                DirectX::XMFLOAT3 focus = enemy->bossJointComponent->GetComponentWorldTransform().GetLocation();
                XMVECTOR Focus = XMLoadFloat3(&focus);

                XMVECTOR Forward = XMVector3Normalize(XMLoadFloat3(&forward));
                XMVECTOR Up = XMVectorSet(0, 1, 0, 0);
                XMVECTOR Right = XMVector3Normalize(XMVector3Cross(Forward, Up));
                Up = XMVector3Normalize(XMVector3Cross(Forward, Right));

                XMVECTOR Eye = Focus + (Right * distanceX) + (Forward * distanceZ) + (Up * distanceY);
                XMFLOAT3 eye;
                XMStoreFloat3(&eye, Eye);

                mainCameraComponent->customTarget = true;
                mainCameraComponent->_target = focus;
                SetPosition(eye);

            }

#endif // 0
            if (auto enemy = std::dynamic_pointer_cast<RiderEnemy>(ActorManager::GetActorByName("enemy")))
            {// 敵の位置を取る
                mainCameraComponent->customTarget = true;
                DirectX::XMFLOAT3 targetVec = enemy->bossJointComponent->GetComponentWorldTransform().GetLocation();
                mainCameraComponent->_target = targetVec;
                DirectX::XMFLOAT3 eye;
                eye.x = 1.285f;
                eye.y = 12.183f;
                eye.z = -22.4f;
                DirectX::XMFLOAT3 playerFirstPos = { 0.7f,0.8f,-9.5f };
                playerFirstPos.y += 1.5f;
                SetPosition(playerFirstPos); // プレイヤーの位置をEyeにする
                if (isFinishFirstPerf)
                {// 最初の演出が終わったら
                    // その時のカメラの位置
                    preEye = playerFirstPos;
                    // その時の focus の位置
                    preTarget = targetVec;
                }
            }
        }
        else
        {
            if (isFinishFirstPerf)
            {// 敵の最初の演出が終わったら
                isFinishFirstPerf = false;
                elapsedTime = 0.0f;
            }
            elapsedTime += deltaTime;
            float lerpTime = 1.0f;
            elapsedTime += deltaTime;
            float t = std::clamp(elapsedTime / lerpTime, 0.0f, 1.0f);
            DirectX::XMVECTOR PreTargetVec = DirectX::XMLoadFloat3(&preTarget);
            DirectX::XMVECTOR PreEyeVec = DirectX::XMLoadFloat3(&preEye);
            // 最初の　target　はプレイヤーの初期位置
            DirectX::XMVECTOR AftTargetVec = DirectX::XMVectorSet(0.7f, 0.8f, -9.5f, 1.0f);
            // 最初の　Eye
            DirectX::XMVECTOR AftEyeVec = DirectX::XMVectorSet(1.285f, 12.183f, -22.4f, 1.0f);
            DirectX::XMVECTOR NowTar = XMVectorLerp(PreTargetVec, AftTargetVec, t);
            DirectX::XMVECTOR NowEye = XMVectorLerp(PreEyeVec, AftEyeVec, t);
            DirectX::XMFLOAT3 nowTarget, nowEye;
            DirectX::XMStoreFloat3(&nowTarget, NowTar);
            DirectX::XMStoreFloat3(&nowEye, NowEye);


            mainCameraComponent->customTarget = false;

            XMVECTOR vOld = XMLoadFloat3(&oldTarget);
            XMVECTOR vNew = XMLoadFloat3(&target);

            float speed = 2.0f;
            float t = std::clamp(deltaTime * speed, 0.0f, 1.0f);

            XMVECTOR vInterp = XMVectorLerp(vOld, vNew, t);
            XMFLOAT3 interpTarget;
            XMStoreFloat3(&interpTarget, vInterp);

            DirectX::XMFLOAT3 clampedTarget;
            clampedTarget.x = std::clamp(interpTarget.x, cameraMin.x, cameraMax.x);
            //clampedTarget.y = std::clamp(interpTarget.y, cameraMin.y, cameraMax.y);
            clampedTarget.y = interpTarget.y;
            clampedTarget.z = std::clamp(interpTarget.z, cameraMin.z, cameraMax.z);

            // 補間済み目標位置を使って、eye にオフセットを適用
            DirectX::XMFLOAT3 eye;
            eye.x = clampedTarget.x + offset.x;
            eye.y = clampedTarget.y + offset.y;
            eye.z = clampedTarget.z + offset.z;
            SetPosition(eye);

            oldTarget = clampedTarget;

            //DirectX::XMFLOAT3 eye = GetPosition();
            DirectX::XMVECTOR eyeVec = DirectX::XMLoadFloat3(&eye);
            DirectX::XMFLOAT3 tar = target;
            tar.y += 0.5f;// player の腰当たり
            DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&target);
            DirectX::XMVECTOR dirVec = DirectX::XMVectorSubtract(targetVec, eyeVec);
            DirectX::XMVECTOR dirNormalized = DirectX::XMVector3Normalize(dirVec);

            // 前フレームのターゲット
            preTarget = tar;

            RaycastHit result;
            DirectX::XMFLOAT3 origin = GetPosition();
            DirectX::XMFLOAT3 direction;
            direction.x = mainCameraComponent->GetView()._31;
            direction.y = -mainCameraComponent->GetView()._32;
            direction.z = mainCameraComponent->GetView()._33;
            DirectX::XMVECTOR distanceVec = DirectX::XMVector3Length(dirVec);
            float distance;
            DirectX::XMVECTOR DirVec = DirectX::XMLoadFloat3(&direction);
            DirVec = DirectX::XMVector3Normalize(DirVec);
            DirectX::XMStoreFloat3(&direction, DirVec);
            DirectX::XMStoreFloat(&distance, distanceVec);
            if (PhysicsTest::Instance().SphereCast(origin, direction, distance, 0.4f, result, CollisionHelper::ToBit(CollisionLayer::Camera),     // myLayer
                CollisionHelper::ToBit(CollisionLayer::Building)))   // wantHitRayer)
            {
                if (auto build = dynamic_cast<Building*>(result.actor))
                {
                    build->preSkeltalMeshComponent->model->SetAlpha(0.3f);
                }
                else if (auto bossBuild = dynamic_cast<BossBuilding*>(result.actor))
                {
                    bossBuild->preSkeltalMeshComponent->model->SetAlpha(0.3f);
                }
            }
            else
            {
                for (const auto& actor : ShockWaveTargetRegistry::GetTargets())
                {
                    if (auto build = std::dynamic_pointer_cast<Building>(actor))
                    {
                        build->preSkeltalMeshComponent->model->SetAlpha(1.0f);
                    }
                    else if (auto bossBuild = std::dynamic_pointer_cast<BossBuilding>(actor))
                    {
                        bossBuild->preSkeltalMeshComponent->model->SetAlpha(1.0f);
                    }
                }
            }
        }

#endif // 0
    }

    void Shake(float power = 0.02f, float time = 0.2f)
    {
        mainCameraComponent->Shake(power, time);
    }

    void SetOldTarget(const DirectX::XMFLOAT3& oldTarget)
    {
        this->oldTarget = oldTarget;
    }

    void DrawImGuiDetails()override
    {
#ifdef USE_IMGUI
        ImGui::DragFloat("distanceX", &distanceX, 0.1f);
        ImGui::DragFloat("distanceY", &distanceY, 0.1f);
        ImGui::DragFloat("distanceZ", &distanceZ, 0.1f);
        ImGui::DragFloat3("cameraOffset", &offset.x, 0.3f);
        ImGui::DragFloat3("cameraMin", &cameraMin.x, 1.0f);
        ImGui::DragFloat3("cameraMax", &cameraMax.x, 1.0f);
#endif
    }
    // ボスに注視点を合わせるかどうか
    void IsTargetBoss(bool isTargetBoss)
    {
        this->isTargetBoss = isTargetBoss;
        elapsedTime = 0.0f;
        if (isTargetBoss)
        {
            state = State::BossTarget;
        }
    }

    void SetTarget(DirectX::XMFLOAT3 target)
    {
        this->target = target;
    }

    void OnFinishFirstPerf()
    {
        isFinishFirstPerf = true;
    }
private:
    DirectX::XMFLOAT3 target = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 offset = { 0.6f,11.4f,-15.4f };
    DirectX::XMFLOAT3 oldTarget = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 cameraMin = { -11.0f,0.0f,-8.0f };
    DirectX::XMFLOAT3 cameraMax = { 11.0f,0.0f,7.0f };
    bool isTargetBoss = false;

    float distanceX = 0.0f;
    float distanceY = 0.0f;
    float distanceZ = 0.0f;
    //float distanceX = 0.0f;
    //float distanceY = -1.7f;
    //float distanceZ = 5.5f;
    // 最初の演出後の focus の位置
    DirectX::XMFLOAT3 preTarget = { 0.0f,0.0f,0.0f };
    float elapsedTime = 0.0f;

    bool isFinishFirstPerf = false;

    // 最初の演出後のカメラの位置
    DirectX::XMFLOAT3 preEye = { 0.0f,0.0f,0.0f };

    // lerp 先の
    DirectX::XMFLOAT3 afterTarget = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 afterEye = { 0.0f,0.0f,0.0f };

    enum class State
    {
        Normal,
        BossTarget,
        Lerp,
    };

    State state = State::Normal;

    bool didShake = false;
};


#endif //CAMERA_H