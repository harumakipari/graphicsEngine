#pragma once
#include <memory>

class CameraComponent;
class Camera;

class CameraManager
{
public:
    static void ToggleCamera()
    {
        useDebugCamera = !useDebugCamera;
    }

    static Camera* GetCurrentCamera()
    {
        return useDebugCamera ? debugCamera.lock().get() : gameCamera;
    }

    static bool IsUseDebug() { return useDebugCamera; }

    static void SetGameCamera(Camera* camera) { gameCamera = camera; }
    static void SetDebugCamera(std::shared_ptr<Camera> camera) { debugCamera = camera; }
private:
    static inline Camera* gameCamera;
    static inline std::weak_ptr<Camera> debugCamera;

    static inline bool useDebugCamera = false;
};
//class CameraManager
//{
//public:
//    static void ToggleCamera()
//    {
//        useDebugCamera = !useDebugCamera;
//    }
//
//    static CameraComponent* GetCurrentCamera()
//    {
//        return useDebugCamera ? debugCameraComponent.lock().get() : gameCameraComponent;
//    }
//
//    static bool IsUseDebug() { return useDebugCamera; }
//
//    static void SetGameCamera(CameraComponent* gameCamera) { gameCameraComponent = gameCamera; }
//    static void SetDebugCamera(std::shared_ptr<CameraComponent> debugCamera) { debugCameraComponent = debugCamera; }
//private:
//    static inline CameraComponent* gameCameraComponent;
//    static inline std::weak_ptr<CameraComponent> debugCameraComponent;
//
//    static inline bool useDebugCamera = false;
//};