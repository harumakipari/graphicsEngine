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
            {// actor�����݂��Ă��Ȃ�������X�L�b�v
                continue;
            }

            // actor �ɕt�����Ă���S�Ă� meshComponent �����o��
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                //  �e MeshComponent ���g�̍ŐV���[���h�s������o��
                const auto& worldMat = meshComponent->GetComponentWorldTransform().ToWorldTransform();

                auto* convexComponent = actor->GetComponent<ConvexCollisionComponent>();
                if (convexComponent = dynamic_cast<ConvexCollisionComponent*>(convexComponent))
                {
                    meshComponent->model->Render(immediateContext, worldMat, convexComponent->GetAnimatedNodes(), InterleavedGltfModel::RenderPass::Opaque);
                }

                if (!meshComponent->IsVisible())
                { // �`��t���O�� false �Ȃ�X�L�b�v
                    continue;
                }
                //  �`��Ăяo��
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
            {// actor�����݂��Ă��Ȃ�������X�L�b�v
                continue;
            }

            // actor �ɕt�����Ă���S�Ă� meshComponent �����o��
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                if (!meshComponent->IsVisible())
                { // �`��t���O�� false �Ȃ�X�L�b�v
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
            {// actor�����݂��Ă��Ȃ�������X�L�b�v
                continue;
            }

            // actor �ɕt�����Ă���S�Ă� meshComponent �����o��
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                if (!meshComponent->IsVisible())
                { // �`��t���O�� false �Ȃ�X�L�b�v
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
            {// actor�����݂��Ă��Ȃ�������X�L�b�v
                continue;
            }

            // actor �ɕt�����Ă���S�Ă� meshComponent �����o��
            std::vector<MeshComponent*> meshComponents;
            actor->GetComponents<MeshComponent>(meshComponents);

            for (const MeshComponent* meshComponent : meshComponents)
            {
                if (!meshComponent->IsVisible())
                { // �`��t���O�� false �Ȃ�X�L�b�v
                    continue;
                }
                const auto& worldMat = meshComponent->GetComponentWorldTransform().ToWorldTransform();
                meshComponent->CastShadow(immediateContext, worldMat);
            }
        }
    }
};

