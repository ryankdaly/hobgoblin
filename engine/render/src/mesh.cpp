#include "hobgoblin/render/mesh.hpp"
#include <cstring>

// WIP
// included as an example for the upcoming full release of the engine's codebase
// also as a linker .cpp so you can compile and use the given cmake files
namespace hb {

    static bool make_buffer(ID3D11Device* dev, const void* data, UINT byteSize,
        D3D11_BIND_FLAG bind, ID3D11Buffer** outBuf)
    {
        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = byteSize;
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.BindFlags = bind;
        D3D11_SUBRESOURCE_DATA srd{};
        srd.pSysMem = data;
        return SUCCEEDED(dev->CreateBuffer(&bd, &srd, outBuf));
    }

    void Mesh::destroy() {
        if (m_ib) { m_ib->Release(); m_ib = nullptr; }
        if (m_vb) { m_vb->Release(); m_vb = nullptr; }
        m_indexCount = 0;
        m_vertexStride = sizeof(VertexPNCUV);
        m_indexFormat = DXGI_FORMAT_R16_UINT;
    }

    bool Mesh::createRaw(ID3D11Device* dev,
        const void* vbBytes, UINT vbSizeBytes, UINT vertexStrideBytes,
        const void* ibBytes, UINT ibSizeBytes, DXGI_FORMAT indexFmt)
    {
        if (!dev || !vbBytes || !ibBytes || vbSizeBytes == 0 || ibSizeBytes == 0) return false;
        destroy();

        if (!make_buffer(dev, vbBytes, vbSizeBytes, D3D11_BIND_VERTEX_BUFFER, &m_vb))
            return false;
        if (!make_buffer(dev, ibBytes, ibSizeBytes, D3D11_BIND_INDEX_BUFFER, &m_ib)) {
            m_vb->Release(); m_vb = nullptr; return false;
        }

        m_vertexStride = vertexStrideBytes;
        m_indexFormat = indexFmt;
        m_indexCount = (indexFmt == DXGI_FORMAT_R16_UINT) ? (ibSizeBytes / 2u) : (ibSizeBytes / 4u);
        return true;
    }

    bool Mesh::createCube(ID3D11Device* dev) {
        destroy();

        const VertexPNCUV v[] = {
            {{+0.5f,-0.5f,-0.5f},{+1,0,0},{1,0,0},{0,1}},
            {{+0.5f,+0.5f,-0.5f},{+1,0,0},{1,0,0},{0,0}},
            {{+0.5f,+0.5f,+0.5f},{+1,0,0},{1,0,0},{1,0}},
            {{+0.5f,-0.5f,+0.5f},{+1,0,0},{1,0,0},{1,1}},
            {{-0.5f,-0.5f,+0.5f},{-1,0,0},{0,1,0},{0,1}},
            {{-0.5f,+0.5f,+0.5f},{-1,0,0},{0,1,0},{0,0}},
            {{-0.5f,+0.5f,-0.5f},{-1,0,0},{0,1,0},{1,0}},
            {{-0.5f,-0.5f,-0.5f},{-1,0,0},{0,1,0},{1,1}},
            {{-0.5f,+0.5f,-0.5f},{0,+1,0},{0,0,1},{0,1}},
            {{-0.5f,+0.5f,+0.5f},{0,+1,0},{0,0,1},{0,0}},
            {{+0.5f,+0.5f,+0.5f},{0,+1,0},{0,0,1},{1,0}},
            {{+0.5f,+0.5f,-0.5f},{0,+1,0},{0,0,1},{1,1}},
            {{-0.5f,-0.5f,+0.5f},{0,-1,0},{1,1,0},{0,1}},
            {{-0.5f,-0.5f,-0.5f},{0,-1,0},{1,1,0},{0,0}},
            {{+0.5f,-0.5f,-0.5f},{0,-1,0},{1,1,0},{1,0}},
            {{+0.5f,-0.5f,+0.5f},{0,-1,0},{1,1,0},{1,1}},
            {{-0.5f,-0.5f,+0.5f},{0,0,+1},{1,0,1},{0,1}},
            {{+0.5f,-0.5f,+0.5f},{0,0,+1},{1,0,1},{0,0}},
            {{+0.5f,+0.5f,+0.5f},{0,0,+1},{1,0,1},{1,0}},
            {{-0.5f,+0.5f,+0.5f},{0,0,+1},{1,0,1},{1,1}},
            {{+0.5f,-0.5f,-0.5f},{0,0,-1},{0,1,1},{0,1}},
            {{-0.5f,-0.5f,-0.5f},{0,0,-1},{0,1,1},{0,0}},
            {{-0.5f,+0.5f,-0.5f},{0,0,-1},{0,1,1},{1,0}},
            {{+0.5f,+0.5f,-0.5f},{0,0,-1},{0,1,1},{1,1}},
        };
        const uint16_t idx[] = {
            0,1,2, 0,2,3,  4,5,6, 4,6,7,
            8,9,10, 8,10,11, 12,13,14, 12,14,15,
            16,17,18, 16,18,19, 20,21,22, 20,22,23
        };

        return createRaw(dev, v, sizeof(v), sizeof(VertexPNCUV),
            idx, sizeof(idx), DXGI_FORMAT_R16_UINT);
    }

    bool Mesh::createPlane(ID3D11Device* dev, float size, float y) {
        destroy();
        float s = size * 0.5f;
        const VertexPNCUV v[] = {
            {{-s, y, -s},{0,1,0},{1,1,1},{0,1}},
            {{-s, y, +s},{0,1,0},{1,1,1},{0,0}},
            {{+s, y, +s},{0,1,0},{1,1,1},{1,0}},
            {{+s, y, -s},{0,1,0},{1,1,1},{1,1}},
        };
        const uint16_t idx[] = { 0,2,1, 0,3,2 };
        return createRaw(dev, v, sizeof(v), sizeof(VertexPNCUV),
            idx, sizeof(idx), DXGI_FORMAT_R16_UINT);
    }

    void Mesh::draw(ID3D11DeviceContext* ctx) const {
        UINT stride = m_vertexStride, offset = 0;
        ctx->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
        ctx->IASetIndexBuffer(m_ib, m_indexFormat, 0);
        ctx->DrawIndexed(m_indexCount, 0, 0);
    }

} // namespace hb
