#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <d3d11.h>
#include <DirectXMath.h>
class Model
{
public:
    static inline std::unordered_map<std::string/*file name*/, std::shared_ptr<Model>> models_;

    static std::shared_ptr<Model> emplace(ID3D11Device* device, const std::string& filename, bool staticBatching, float toMetersScale = 1.0f, size_t coordinateSystem = 0)
    {
        auto cachedModel = models_.find(filename);
        if (cachedModel != models_.end())
        {
            return cachedModel->second;
        }
        return models_.emplace(filename, std::make_shared<Model>(device, filename, staticBatching, toMetersScale, coordinateSystem)).first->second;
    }

    static std::shared_ptr<Model> at(const std::string& filename)
    {
        _ASSERT_EXPR(models_.find(filename) != models_.end(), L"このファイル名でモデルは登録されていません。");
        return models_.at(filename);
    }

public:
    Model(ID3D11Device* device, const std::string& filename, bool staticBatching, float toMetersScale = 1.0f, size_t coordinateSystem = 0);
    Model(const Model& rhs) = delete;
    Model& operator=(const Model& rhs) = delete;
    Model(Model&&) noexcept = default;
    Model& operator=(Model&&) noexcept = delete;
    ~Model() = default;

    const bool staticBatching;
    const  DirectX::XMMATRIX axisConversionMatrix;

public:
    std::string filename;

    // マテリアルの variants
    std::vector<std::string> variants;
};
#endif // !MODEL_H