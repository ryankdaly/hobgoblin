#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#include "hobgoblin/render/meshbin.hpp"
#include "meshpack_lib.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cmath>

using hb::MeshBinHeader;
using hb::MeshIndexFmt;

namespace {

    struct VtxPNCUV { float px, py, pz, nx, ny, nz, cr, cg, cb, u, v; };

    static bool write_bin(const std::string& path, const void* data, size_t sz) {
        std::ofstream f(path, std::ios::binary);
        if (!f) return false;
        f.write(reinterpret_cast<const char*>(data), std::streamsize(sz));
        return f.good();
    }
    static bool write_text(const std::string& path, const std::string& s) {
        std::ofstream f(path, std::ios::binary);
        if (!f) return false;
        f << s;
        return f.good();
    }
    static void ensure_normals(std::vector<VtxPNCUV>& v, const std::vector<uint32_t>& idx) {
        for (auto& x : v) { x.nx = x.ny = x.nz = 0.0f; }
        for (size_t i = 0;i + 2 < idx.size();i += 3) {
            VtxPNCUV& a = v[idx[i]], & b = v[idx[i + 1]], & c = v[idx[i + 2]];
            float ux = b.px - a.px, uy = b.py - a.py, uz = b.pz - a.pz;
            float vx = c.px - a.px, vy = c.py - a.py, vz = c.pz - a.pz;
            float nx = uy * vz - uz * vy, ny = uz * vx - ux * vz, nz = ux * vy - uy * vx;
            a.nx += nx; a.ny += ny; a.nz += nz;
            b.nx += nx; b.ny += ny; b.nz += nz;
            c.nx += nx; c.ny += ny; c.nz += nz;
        }
        for (auto& x : v) {
            float l = std::sqrt(x.nx * x.nx + x.ny * x.ny + x.nz * x.nz);
            if (l > 1e-6f) { x.nx /= l; x.ny /= l; x.nz /= l; }
            else { x.nx = 0; x.ny = 1; x.nz = 0; }
        }
    }

    // helper copies albedo file into output directory
    // returns a user-visible log message string (may be empty if nothing was done)
    static std::string copy_albedo_if_possible(
        const std::string& albedoRel,
        const std::string& inPath,
        const std::string& outMesh)
    {
        namespace fs = std::filesystem;

        if (albedoRel.empty()) return {}; // nothing to do
        if (albedoRel.rfind("data:", 0) == 0)
            return " (warning: embedded/data URI albedo not copied)";

        const fs::path outDir = fs::path(outMesh).parent_path();
        const fs::path inDir = fs::path(inPath).parent_path();

        fs::path src = albedoRel;
        if (!src.is_absolute())
            src = inDir / src;

        if (!fs::exists(src))
            return " (warning: albedo path not found to copy: " + src.string() + ")";

        fs::path dst = outDir / src.filename();
        std::error_code ec;
        fs::create_directories(outDir, ec);
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing, ec);

        if (!ec) {
            return " (albedo copied -> " + dst.string() + ")";
        }
        else {
            return " (warning: failed to copy albedo: " + ec.message() + ")";
        }
    }


} // anon

namespace hb::meshpack {

    bool pack_gltf_to_meshbin(const std::string& inPath,
        const std::string& outMesh,
        const std::string& albedoRelArg,
        std::string* out_log)
    {
        auto log = [&](const std::string& s) { if (out_log) { out_log->append(s); out_log->push_back('\n'); } };

        std::string albedoRel = albedoRelArg;

        cgltf_options opt{}; cgltf_data* data = nullptr;
        auto res = cgltf_parse_file(&opt, inPath.c_str(), &data);
        if (res != cgltf_result_success) { log("cgltf_parse_file failed"); return false; }
        res = cgltf_load_buffers(&opt, data, inPath.c_str());
        if (res != cgltf_result_success) { log("cgltf_load_buffers failed"); cgltf_free(data); return false; }
        cgltf_validate(data);

        if (data->meshes_count == 0 || data->meshes[0].primitives_count == 0) {
            log("No mesh/primitive"); cgltf_free(data); return false;
        }
        const cgltf_primitive& prim = data->meshes[0].primitives[0];

        // indices
        std::vector<uint32_t> idx32;
        if (prim.indices) {
            const cgltf_accessor* acc = prim.indices;
            idx32.resize(acc->count);
            for (cgltf_size i = 0;i < acc->count;i++) {
                cgltf_uint vi = 0; cgltf_accessor_read_uint(acc, i, &vi, 1);
                idx32[i] = (uint32_t)vi;
            }
        }
        else {
            cgltf_size vcount = 0;
            for (cgltf_size i = 0;i < prim.attributes_count;i++)
                if (prim.attributes[i].type == cgltf_attribute_type_position)
                    vcount = prim.attributes[i].data->count;
            idx32.resize(vcount);
            for (uint32_t i = 0;i < vcount;i++) idx32[i] = i;
        }

        // vertex attribs
        const cgltf_accessor* posAcc = nullptr;
        const cgltf_accessor* nrmAcc = nullptr;
        const cgltf_accessor* uv0Acc = nullptr;
        for (cgltf_size i = 0;i < prim.attributes_count;i++) {
            const auto& a = prim.attributes[i];
            if (a.type == cgltf_attribute_type_position) posAcc = a.data;
            else if (a.type == cgltf_attribute_type_normal) nrmAcc = a.data;
            else if (a.type == cgltf_attribute_type_texcoord && a.index == 0) uv0Acc = a.data;
        }
        if (!posAcc) { log("No POSITION"); cgltf_free(data); return false; }

        const size_t vcount = posAcc->count;
        std::vector<VtxPNCUV> verts(vcount);
        for (size_t i = 0;i < vcount;i++) {
            float p[3]; cgltf_accessor_read_float(posAcc, i, p, 3);
            verts[i].px = p[0]; verts[i].py = p[1]; verts[i].pz = p[2];
            if (nrmAcc) {
                float n[3]; cgltf_accessor_read_float(nrmAcc, i, n, 3);
                verts[i].nx = n[0]; verts[i].ny = n[1]; verts[i].nz = n[2];
            }
            else { verts[i].nx = 0; verts[i].ny = 1; verts[i].nz = 0; }
            verts[i].cr = 1.0f; verts[i].cg = 1.0f; verts[i].cb = 1.0f;
            if (uv0Acc) {
                float t[2] = { 0,0 }; cgltf_accessor_read_float(uv0Acc, i, t, 2);
                verts[i].u = t[0]; verts[i].v = 1.0f - t[1];
            }
            else { verts[i].u = 0.0f; verts[i].v = 0.0f; }
        }
        if (!nrmAcc) ensure_normals(verts, idx32);

        // index size
        uint32_t maxIdx = 0; for (auto v : idx32) if (v > maxIdx) maxIdx = v;
        const bool use16 = (maxIdx <= 0xFFFFu);

        // build file
        MeshBinHeader hdr{};
        hdr.vertexCount = (uint32_t)verts.size();
        hdr.indexCount = (uint32_t)idx32.size();
        hdr.vertexStride = (uint32_t)sizeof(VtxPNCUV);
        hdr.indexFmt = use16 ? MeshIndexFmt::U16 : MeshIndexFmt::U32;
        hdr.hasNormals = 1; hdr.hasUVs = 1;

        const uint32_t vtxBytes = (uint32_t)(verts.size() * sizeof(VtxPNCUV));
        const uint32_t idxBytes = use16 ? (uint32_t)(idx32.size() * sizeof(uint16_t))
            : (uint32_t)(idx32.size() * sizeof(uint32_t));

        hdr.verticesOffset = sizeof(MeshBinHeader);
        hdr.indicesOffset = hdr.verticesOffset + vtxBytes;

        std::vector<uint8_t> file;
        file.resize(hdr.indicesOffset + idxBytes);
        std::memcpy(file.data(), &hdr, sizeof(hdr));
        std::memcpy(file.data() + hdr.verticesOffset, verts.data(), vtxBytes);

        if (use16) {
            std::vector<uint16_t> idx16(idx32.size());
            for (size_t i = 0;i < idx32.size();++i) idx16[i] = (uint16_t)idx32[i];
            std::memcpy(file.data() + hdr.indicesOffset, idx16.data(), idxBytes);
        }
        else {
            std::memcpy(file.data() + hdr.indicesOffset, idx32.data(), idxBytes);
        }

        if (!write_bin(outMesh, file.data(), file.size())) {
            log("Failed to write meshbin"); cgltf_free(data); return false;
        }

        // sidecar .meta.json (albedo hint)
        if (albedoRel.empty()) {
            if (prim.material && prim.material->pbr_metallic_roughness.base_color_texture.texture &&
                prim.material->pbr_metallic_roughness.base_color_texture.texture->image &&
                prim.material->pbr_metallic_roughness.base_color_texture.texture->image->uri) {
                albedoRel = prim.material->pbr_metallic_roughness.base_color_texture.texture->image->uri;
            }
        }
        std::filesystem::path metaPath = std::filesystem::path(outMesh).replace_extension(".meta.json");
        write_text(metaPath.string(), std::string("{\n  \"albedo\": \"") + albedoRel + "\"\n}\n");

        std::string copyMsg = copy_albedo_if_possible(albedoRel, inPath, outMesh);

        std::string albedoMsg;
        if (albedoRel.empty()) {
            albedoMsg = " (albedo not detected)";
        }
        else {
            albedoMsg = " (albedo=" + albedoRel + ")";
        }

        log("Packed " + inPath + " -> " + outMesh +
            std::string(" (") + (use16 ? "u16" : "u32") + " indices)" +
            albedoMsg + copyMsg);
        cgltf_free(data);
        return true;
    }

} // namespace hb::meshpack
