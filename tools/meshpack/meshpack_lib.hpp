#pragma once
#include <string>

namespace hb::meshpack {

    // packs GLTF/GLB -> .meshbin and writes a tiny .meta.json with optional "albedo"
    // return true on success; put any human readable info in out_log (both errors and progress)
    bool pack_gltf_to_meshbin(const std::string& inPath,
        const std::string& outMeshPath,
        const std::string& albedoRelativeOrEmpty,
        std::string* out_log);

} // namespace hb::meshpack
