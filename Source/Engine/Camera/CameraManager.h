#pragma once
#include <memory>

class CameraComponent;

class CameraManager
{
public:
    static void ToggleCamera()
    {
        useDebugCamera = !useDebugCamera;
    }

    static CameraComponent* GetCurrentCamera()
    {
        return useDebugCamera ? debugCameraComponent.lock().get() : gameCameraComponent;
    }

    static bool IsUseDebug() { return useDebugCamera; }

    static void SetGameCamera(CameraComponent* gameCamera) { gameCameraComponent = gameCamera; }
    static void SetDebugCamera(std::shared_ptr<CameraComponent> debugCamera) { debugCameraComponent = debugCamera; }
private:
    static inline CameraComponent* gameCameraComponent;
    static inline std::weak_ptr<CameraComponent> debugCameraComponent;

    static inline bool useDebugCamera = false;
};