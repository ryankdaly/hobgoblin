#pragma once
#include <d3d11.h>
#include <vector>
#include <cstdint>
#include <glm/vec3.hpp>

namespace hb {

    struct VertexPNCUV {
        float pos[3];
        float normal[3];
        float color[3];
        float uv[2];
    };

    class Mesh {
    public:
        // existing
        bool createCube(ID3D11Device* dev);
        bool createPlane(ID3D11Device* dev, float size = 20.0f, float y = 0.0f);
        void draw(ID3D11DeviceContext* ctx) const;


        // authoring space bounds (mesh/model space)
        glm::vec3 boundsMin{ 0.0f, 0.0f, 0.0f };
        glm::vec3 boundsMax{ 0.0f, 0.0f, 0.0f };
        bool hasBounds = false;

        // generic raw creation (uploads verbatim VB/IB)
        bool createRaw(ID3D11Device* dev,
            const void* vbBytes, UINT vbSizeBytes, UINT vertexStrideBytes,
            const void* ibBytes, UINT ibSizeBytes, DXGI_FORMAT indexFmt);

        // optional: free GPU buffers if you recreate meshes
        void destroy();


    private:
        ID3D11Buffer* m_vb = nullptr;
        ID3D11Buffer* m_ib = nullptr;
        UINT          m_indexCount = 0;
        UINT          m_vertexStride = sizeof(VertexPNCUV);
        DXGI_FORMAT   m_indexFormat = DXGI_FORMAT_R16_UINT;
    };

} // namespace hb
