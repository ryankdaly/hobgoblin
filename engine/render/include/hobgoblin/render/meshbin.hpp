#pragma once
#include <cstdint>

namespace hb {

    enum class MeshIndexFmt : uint8_t { U16 = 0, U32 = 1 };

#pragma pack(push,1)
    struct MeshBinHeader {
        uint32_t magic = 0x4842534D; // 'HBSM'
        uint32_t version = 2;          // bump to 2 for PNCUV
        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;

        // vertex layout = PNCUV (float3 pos, float3 normal, float3 color, float2 uv)
        uint32_t vertexStride = sizeof(float) * (3 + 3 + 3 + 2); // 44 bytes

        // indices: 0 = U16, 1 = U32
        MeshIndexFmt indexFmt = MeshIndexFmt::U32;
        uint8_t  hasNormals = 1;
        uint8_t  hasUVs = 1;
        uint8_t  reserved = 0;

        // offsets from file start
        uint32_t verticesOffset = 0;
        uint32_t indicesOffset = 0;
    };
#pragma pack(pop)

} // namespace hb
