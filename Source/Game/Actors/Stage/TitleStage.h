#ifndef TITLE_STAGE_H
#define TITLE_STAGE_H

#include "Core/Actor.h"
#include "Components/Render/MeshComponent.h" 
#include "Utils/EasingHandler.h"

class TitleStage :public Actor
{
public:
    //�����t���R���X�g���N�^
    TitleStage(std::string actorName) :Actor(actorName) {}
    virtual ~TitleStage() = default;

    std::shared_ptr<StaticMeshComponent> titleLogo;
    void Initialize(const Transform& transform)override
    {
        std::shared_ptr<SceneComponent> parent = this->NewSceneComponent<SceneComponent>("empty");
        SetPosition(transform.GetLocation());
        SetQuaternionRotation(transform.GetRotation());
        SetScale(transform.GetScale());

        std::shared_ptr<StaticMeshComponent> stage = this->NewSceneComponent<StaticMeshComponent>("stageComponent","empty");
        stage->SetModel("./Data/Models/Title/title_yuka_kabe.gltf");
        stage->model->modelCoordinateSystem = InterleavedGltfModel::CoordinateSystem::RH_Y_UP;
        stage->SetRelativeScaleDirect({ -1.0f,1.0f,-1.0f });

        titleLogo = this->NewSceneComponent<StaticMeshComponent>("logoComponent", "empty");
        titleLogo->SetModel("./Data/Models/Title/title_rogo.gltf");
        titleLogo->model->modelCoordinateSystem = InterleavedGltfModel::CoordinateSystem::RH_Y_UP;
        titleLogo->SetRelativeScaleDirect({ -1.0f,1.0f,-1.0f });        
        titleLogo->SetRelativeLocationDirect({ 0.0f,0.1f,-0.1f });      // y���W 1.9f �Ŕ͈͊O
        titleLogo->SetRelativeEulerRotationDirect({ 0.0f,-9.0f,0.0f });

        std::shared_ptr<StaticMeshComponent> build = this->NewSceneComponent<StaticMeshComponent>("buildComponent", "empty");
        build->SetModel("./Data/Models/Title/title_bill.gltf");
        build->model->modelCoordinateSystem = InterleavedGltfModel::CoordinateSystem::RH_Y_UP;
        build->SetRelativeScaleDirect({ -1.0f,1.0f,-1.0f });

        std::shared_ptr<SkeltalMeshComponent> trafficLight = this->NewSceneComponent<SkeltalMeshComponent>("trafficLight", "empty");
        trafficLight->SetModel("./Data/Models/Stage/Props/traffic_light.gltf");
        trafficLight->model->modelCoordinateSystem = InterleavedGltfModel::CoordinateSystem::RH_Y_UP;
        trafficLight->model->emission = 1.0f;
        trafficLight->SetRelativeScaleDirect({ 2.0f,2.0f,-2.0f });
        trafficLight->SetRelativeLocationDirect({ 4.25f,0.09f,7.27f });     
        trafficLight->SetRelativeEulerRotationDirect({ -12.4f,8.169f,53.431f });
        CreatePsFromCSO(Graphics::GetDevice(), "./Shader/GltfModelEmissionPS.cso", trafficLight->pipeLineState_.pixelShader.ReleaseAndGetAddressOf());
    }


    void Update(float deltaTime)override
    {
        if (isPushButton&&!isSetEasing)
        {
            yEasing.Clear();
            yEasing.SetEasing(EaseType::OutQuart, 0.1f, 1.9f, 1.0f);
            isSetEasing = true;
            isPushButton = false;
        }

        if (OnPushBack && !isSetEasing)
        {
            yEasing.Clear();
            yEasing.SetEasing(EaseType::OutQuart, 1.9f, 0.1f, 1.0f);
            isSetEasing = true;
            OnPushBack = false;
        }

        yEasing.Update(easeY, deltaTime);
        titleLogo->SetRelativeLocationDirect({ 0.0f,easeY,-0.1f });      // y���W 1.9f �Ŕ͈͊O
    }

    void OnPushButton()
    {
        isPushButton = true;
        isSetEasing = false;
    }

    void OnPushBackToTitle()
    {
        OnPushBack = true;
        isSetEasing = false;
    }
private:
    EasingHandler yEasing;
    float easeY = 0.0f;
    bool isPushButton = false;

    bool isSetEasing =false;

    bool OnPushBack = false;
};

#endif // !TITLE_STAGE_H
