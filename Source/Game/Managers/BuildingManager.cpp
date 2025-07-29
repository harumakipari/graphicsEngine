#include "BuildingManager.h"
#include "Core/ActorManager.h"
#include "Game/Actors/Player/Player.h"
#include "Game/Actors/Stage/Building.h"

void BuildingManager::SpawnItemArea(Area& area, int col, int row)
{
#if 0
    SpawnHelper::TrySpawnWithValidation<TestCollision>(3,
        [&area]() {return area.GetRandomSpawnPosition(); }, [&area](auto building) {area.currentItemCount++; });
    const int maxTries = 10;
    int tryCount = 0;

    while (tryCount < maxTries)
    {
        ++tryCount;

        DirectX::XMFLOAT3 spawnPos = area.GetRandomSpawnPosition();
        // ‚±‚±‚Å position ‚ðŽæ‚Á‚Ä‚«‚½‚ç¶¬‚Å‚«‚é

        Transform transform(spawnPos, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
        auto building = ActorManager::CreateAndRegisterActorWithTransform<Building>("building", transform);
        //building->SetTempPosition(spawnPos);
        //building->Initialize();
        //building->PostInitialize();

        auto shape = building->GetSceneComponentByName("boxComponent");
        if (auto box = std::dynamic_pointer_cast<BoxComponet>(shape))
        {
            if (SpawnValidator::IsAreaFree(box->GetAABB()))
            {
                SpawnValidator::Register(shared_from_this());

                //item->GetComponent<SkeltalMeshComponent>()->SetIsVisible(true);
                building->preSkeltalMeshComponent->SetIsVisible(true);
                area.currentItemCount++;
                return; //¬Œ÷‚µ‚½‚Ì‚Å‚±‚Ì area ‚Ìƒ‹[ƒv‚ð‚â‚ß‚é
            }
        }
        building->SetValid(false);
    }
#else
    DirectX::XMFLOAT3 spawnPos = area.GetGridSpawnPosition(col, row);


    //std::shared_ptr<InstancedStaticMeshComponent> instanceBuildMeshComponent=

    auto player = std::dynamic_pointer_cast<Player>(ActorManager::GetActorByName("actor"));
    DirectX::XMFLOAT3 pos = player->GetPosition();
    DirectX::XMVECTOR disVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pos), DirectX::XMLoadFloat3(&spawnPos));
    float x = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(disVec));
    static float r = 4.0f;
    if ((x - r * r) <= 0.1f)
    {
        return;
    }

    spawnPos.y = -5.0f;
    Transform transform(spawnPos, DirectX::XMFLOAT4(0, 0, 0, 1), DirectX::XMFLOAT3(1, 1, 1));



    auto building = ActorManager::CreateAndRegisterActorWithTransform<Building>("building", transform);

    //auto shape = building->GetSceneComponentByName("boxComponent");
    //if (auto box = std::dynamic_pointer_cast<BoxComponet>(shape))
    //{
    //    if (SpawnValidator::IsAreaFree(box->GetAABB()))
    //    {
    //        SpawnValidator::RegisterAABB(box->GetAABB());
    //        building->preSkeltalMeshComponent->SetIsVisible(true);
    //        area.currentItemCount++;
    //        return;
    //    }
    //}

    //building->SetValid(false);
#endif
}

void BuildingManager::SpawnBossBuildArea(Area& area, int col, int row)
{
    DirectX::XMFLOAT3 spawnPos = area.GetGridSpawnPosition(col, row);

    //std::shared_ptr<InstancedStaticMeshComponent> instanceBuildMeshComponent=

    auto player = std::dynamic_pointer_cast<Player>(ActorManager::GetActorByName("actor"));
    DirectX::XMFLOAT3 pos = player->GetPosition();
    DirectX::XMVECTOR disVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pos), DirectX::XMLoadFloat3(&spawnPos));
    float x = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(disVec));
    static float r = 4.0f;
    if ((x - r * r) <= 0.1f)
    {
        return;
    }

    spawnPos.y = -5.0f;
    Transform transform(spawnPos, DirectX::XMFLOAT4(0, 0, 0, 1), DirectX::XMFLOAT3(1, 1, 1));

    auto building = ActorManager::CreateAndRegisterActorWithTransform<BossBuilding>("bossbuilding", transform);

    //auto shape = building->GetSceneComponentByName("boxComponent");
    //if (auto box = std::dynamic_pointer_cast<BoxComponet>(shape))
    //{
    //    if (SpawnValidator::IsAreaFree(box->GetAABB()))
    //    {
    //        SpawnValidator::RegisterAABB(box->GetAABB());
    //        building->preSkeltalMeshComponent->SetIsVisible(true);
    //        area.currentItemCount++;
    //        return;
    //    }
    //}

    //building->SetValid(false);
}

