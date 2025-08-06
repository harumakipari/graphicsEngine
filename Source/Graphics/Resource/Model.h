#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <memory>
#include <vector>
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
        _ASSERT_EXPR(models_.find(filename) != models_.end(), L"���̃t�@�C�����Ń��f���͓o�^����Ă��܂���B");
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

    // �}�e���A���� variants
    std::vector<std::string> variants;

    struct Scene
    {
        std::string name;
        std::vector<int> nodes;// ���[�g�m�[�h�̔z��
    };
    std::vector<Scene> scenes;
    int defaultScene = 0;

    struct Node
    {
        std::string name;
        int camera = -1;
        int skin = -1;
        int mesh = -1;

        std::vector<int> children;

        // ���[�J���̃g�����X�t�H�[��
        DirectX::XMFLOAT4 rotation = { 0,0,0,1 };
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 translation = { 0,0,0 };
        std::vector<double> weights;

        DirectX::XMFLOAT4X4 globalTransform =
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1,
        };

        DirectX::XMFLOAT3 minValue=
        {

        }
    };

};
#endif // !MODEL_H