#pragma once
#include<d3d11.h>
#include <vector>
#include <memory>

#include "Core/Actor.h"
#include "Components/Render/MeshComponent.h"
#include "Components/CollisionShape/ShapeComponent.h"

class SceneRenderer
{
public:
    SceneRenderer() = default;

    virtual ~SceneRenderer() {}

    void RenderOpaque(ID3D11DeviceContext* immediateContext, std::vector<std::shared_ptr<Actor>> allActors)
    {
        for (auto actor : allActors)
        {
            if (!actor->rootComponent_)
            {
                continue;
            }

            if (!actor->isActive)
            {// actorが存在していなかったらスキップ
                continue;
            }

            // actor に付属している全ての meshComponent を取り出す
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                //  各 MeshComponent 自身の最新ワールド行列を取り出す
                const auto& worldMat = meshComponent->GetComponentWorldTransform().ToWorldTransform();

                auto* convexComponent = actor->GetComponent<ConvexCollisionComponent>();
                if (convexComponent = dynamic_cast<ConvexCollisionComponent*>(convexComponent))
                {
                    meshComponent->model->Render(immediateContext, worldMat, convexComponent->GetAnimatedNodes(), InterleavedGltfModel::RenderPass::Opaque);
                }

                if (!meshComponent->IsVisible())
                { // 描画フラグが false ならスキップ
                    continue;
                }
                //  描画呼び出し
                meshComponent->RenderOpaque(immediateContext, worldMat);
            }
        }
    }
    void RenderMask(ID3D11DeviceContext* immediateContext, std::vector<std::shared_ptr<Actor>> allActors)
    {
        for (auto actor : allActors)
        {
            if (!actor->rootComponent_)
            {
                continue;
            }

            if (!actor->isActive)
            {// actorが存在していなかったらスキップ
                continue;
            }

            // actor に付属している全ての meshComponent を取り出す
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                if (!meshComponent->IsVisible())
                { // 描画フラグが false ならスキップ
                    continue;
                }
                const auto& worldMat = meshComponent->GetComponentWorldTransform().ToWorldTransform();
                meshComponent->RenderMask(immediateContext, worldMat);
            }
        }
    }
    void RenderBlend(ID3D11DeviceContext* immediateContext, std::vector<std::shared_ptr<Actor>> allActors)
    {
        for (auto actor : allActors)
        {
            if (!actor->rootComponent_)
            {
                continue;
            }

            if (!actor->isActive)
            {// actorが存在していなかったらスキップ
                continue;
            }

            // actor に付属している全ての meshComponent を取り出す
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                if (!meshComponent->IsVisible())
                { // 描画フラグが false ならスキップ
                    continue;
                }
                const auto& worldMat = meshComponent->GetComponentWorldTransform().ToWorldTransform();
                meshComponent->RenderBlend(immediateContext, worldMat);
            }
        }
    }

    void CastShadowRender(ID3D11DeviceContext* immediateContext, std::vector<std::shared_ptr<Actor>> allActors)
    {
        for (auto actor : allActors)
        {
            if (!actor->rootComponent_)
            {
                continue;
            }

            if (!actor->isActive)
            {// actorが存在していなかったらスキップ
                continue;
            }

            // actor に付属している全ての meshComponent を取り出す
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                if (!meshComponent->IsVisible())
                { // 描画フラグが false ならスキップ
                    continue;
                }
                const auto& worldMat = meshComponent->GetComponentWorldTransform().ToWorldTransform();
                meshComponent->CastShadow(immediateContext, worldMat);
            }
        }
    }
};

