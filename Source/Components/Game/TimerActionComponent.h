#ifndef TIMER_ACTION_COMPONENT_H
#define TIMER_ACTION_COMPONENT_H

#include <functional>

#include "Components/Base/Component.h"

class TimerActionComponent :public Component
{
public:
    TimerActionComponent(const std::string& name, std::shared_ptr<Actor> owner)
        : Component(name, owner)
    {
    }

    virtual ~TimerActionComponent() = default;

    void Initialize()override {};

    // 遅延時間と実行する関数を設定
    void SetTimer(float delaySeconds, std::function<void()> action)
    {
        delay_ = delaySeconds;
        elapsed_ = 0.0f;
        isActive_ = true;
        onComplete_ = action;
    }

    void Tick(float deltaTime) override
    {
        if (!isActive_)
            return;

        elapsed_ += deltaTime;

        if (elapsed_ >= delay_)
        {
            if (onComplete_)
            {
                onComplete_(); // 遅延後の処理を実行
            }

            isActive_ = false;
        }
    }

    virtual void UpdateComponentToWorld(UpdateTransformFlags update_transform_flags = UpdateTransformFlags::None, TeleportType teleport = TeleportType::None)override
    {
        // LogicComponentにはTransformがないので、何もしない
    }

private:
    float delay_ = 0.0f;
    float elapsed_ = 0.0f;
    bool isActive_ = false;
    std::function<void()> onComplete_;
};

#endif // !TIMER_ACTION_COMPONENT_H
