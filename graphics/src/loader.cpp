// BismIllahIRRahmaanIRRaheem
/* model loader */

#include <graphics/loader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <stb_image.h>
#include <string.h>

void falx::LoadModel(const char* aPath, std::vector<IndividualClump>& oClumps, std::vector<Brush>& oBrushes)
{
    Assimp::Importer h_Importer;
    const aiScene* p_Scene = h_Importer.ReadFile(aPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices | aiProcess_GenNormals);
	FLX_SMART_CHECK(p_Scene != NULL, "Failed to load model");
    oClumps.clear();
    oBrushes.clear();
    oClumps.reserve(p_Scene->mNumMeshes);
    // one brush per material, created on first use
    std::vector<int32> m_MaterialBrush(p_Scene->mNumMaterials, -1);
    for (uint32 i = 0; i < p_Scene->mNumMeshes; i++) {
        const aiMesh* p_Mesh = p_Scene->mMeshes[i];
        FLX_SMART_CHECK(p_Mesh->HasPositions() && p_Mesh->mNumVertices <= 0xFFFF, "Unsupported mesh in model");
        IndividualClump m_Clump = {};
        // pos(3) uv(2) normal(3)
        m_Clump.m_NumVerts = static_cast<uint16>(p_Mesh->mNumVertices);
        m_Clump.m_ModelVerts = static_cast<float32*>(malloc(static_cast<size_t>(p_Mesh->mNumVertices) * 8 * sizeof(float32)));
        FLX_SMART_CHECK(m_Clump.m_ModelVerts != NULL, "Failed to allocate model vertices");
		bit m_HasTexCoords = p_Mesh->HasTextureCoords(0);
        for (uint32 v = 0; v < p_Mesh->mNumVertices; v++) {
            float32* p_Out = &m_Clump.m_ModelVerts[v * 8];
            p_Out[0] = p_Mesh->mVertices[v].x;
            p_Out[1] = p_Mesh->mVertices[v].y;
            p_Out[2] = p_Mesh->mVertices[v].z;
            if (m_HasTexCoords) {
                p_Out[3] = p_Mesh->mTextureCoords[0][v].x;
                p_Out[4] = p_Mesh->mTextureCoords[0][v].y;
            }
            else {
                p_Out[3] = 0;
                p_Out[4] = 0;
            }
            p_Out[5] = p_Mesh->mNormals[v].x;
            p_Out[6] = p_Mesh->mNormals[v].y;
            p_Out[7] = p_Mesh->mNormals[v].z;
        }
        uint32 m_NumIndices = 0;
        for (uint32 f = 0; f < p_Mesh->mNumFaces; f++) {
            m_NumIndices += p_Mesh->mFaces[f].mNumIndices;
        }
        m_Clump.m_NumIndices = m_NumIndices;
        m_Clump.m_ModelIndices = static_cast<uint16*>(malloc(static_cast<size_t>(m_NumIndices) * sizeof(uint16)));
        FLX_SMART_CHECK(m_Clump.m_ModelIndices != NULL, "Failed to allocate model indices");
        uint32 m_Written = 0;
        for (uint32 f = 0; f < p_Mesh->mNumFaces; f++) {
            const aiFace& m_Face = p_Mesh->mFaces[f];
            for (uint32 ind = 0; ind < m_Face.mNumIndices; ind++) {
                m_Clump.m_ModelIndices[m_Written++] = static_cast<uint16>(m_Face.mIndices[ind]);
            }
        }
        uint32 m_MaterialIndex = p_Mesh->mMaterialIndex;
        if (m_MaterialBrush[m_MaterialIndex] < 0) {
            Brush m_Brush = {};
            const aiMaterial* p_Material = p_Scene->mMaterials[m_MaterialIndex];
            aiString m_TexturePath;
            aiColor4D m_Color;
            bool m_HasTexture = false;
            bool m_Loaded = false;
            if (p_Material->GetTextureCount(aiTextureType_BASE_COLOR) > 0) {
                m_HasTexture = p_Material->GetTexture(aiTextureType_BASE_COLOR, 0, &m_TexturePath) == AI_SUCCESS;
            }
            else if (p_Material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                m_HasTexture = p_Material->GetTexture(aiTextureType_DIFFUSE, 0, &m_TexturePath) == AI_SUCCESS;
            }
            if (m_HasTexture) {
                int m_ImgW = 0;
                int m_ImgH = 0;
                int m_ImgComp = 0;
                uint8* p_Pixels = nullptr;
                if (m_TexturePath.C_Str()[0] == '*') {
                    uint32 m_TexIndex = static_cast<uint32>(atoi(&m_TexturePath.C_Str()[1]));
                    const aiTexture* p_Texture = (m_TexIndex < p_Scene->mNumTextures) ? p_Scene->mTextures[m_TexIndex] : nullptr;
                    if (p_Texture != nullptr && p_Texture->mHeight == 0) {
                        // compressed embedded image (png, jpg, ...)
                        p_Pixels = stbi_load_from_memory(reinterpret_cast<const uint8*>(p_Texture->pcData), static_cast<int>(p_Texture->mWidth), &m_ImgW, &m_ImgH, &m_ImgComp, 4);
                    }
                    else if (p_Texture != nullptr) {
                        // raw embedded texels
                        m_ImgW = static_cast<int>(p_Texture->mWidth);
                        m_ImgH = static_cast<int>(p_Texture->mHeight);
                        p_Pixels = static_cast<uint8*>(malloc(static_cast<size_t>(m_ImgW) * m_ImgH * 4));
                        if (p_Pixels != nullptr) {
                            for (int t = 0; t < m_ImgW * m_ImgH; t++) {
                                p_Pixels[t * 4 + 0] = p_Texture->pcData[t].r;
                                p_Pixels[t * 4 + 1] = p_Texture->pcData[t].g;
                                p_Pixels[t * 4 + 2] = p_Texture->pcData[t].b;
                                p_Pixels[t * 4 + 3] = p_Texture->pcData[t].a;
                            }
                        }
                    }
                }
                else {
                    // external image file, relative to the model's directory
                    std::string m_FullPath(aPath);
                    size_t m_Slash = m_FullPath.find_last_of("/\\");
                    m_FullPath = (m_Slash == std::string::npos) ? std::string() : m_FullPath.substr(0, m_Slash + 1);
                    m_FullPath += m_TexturePath.C_Str();
                    p_Pixels = stbi_load(m_FullPath.c_str(), &m_ImgW, &m_ImgH, &m_ImgComp, 4);
                    if (p_Pixels == nullptr) {
                        p_Pixels = stbi_load(m_TexturePath.C_Str(), &m_ImgW, &m_ImgH, &m_ImgComp, 4);
                    }
                }
                if (p_Pixels != nullptr && m_ImgW > 0 && m_ImgH > 0 && m_ImgW <= 0xFFFF && m_ImgH <= 0xFFFF) {
                    size_t m_Count = static_cast<size_t>(m_ImgW) * m_ImgH;
                    m_Brush.m_Bitmap = static_cast<color*>(malloc(m_Count * sizeof(color)));
                    FLX_SMART_CHECK(m_Brush.m_Bitmap != NULL, "Failed to allocate brush bitmap");
                    memcpy(m_Brush.m_Bitmap, p_Pixels, m_Count * 4);
                    m_Brush.m_Width = static_cast<uint16>(m_ImgW);
                    m_Brush.m_Height = static_cast<uint16>(m_ImgH);
                    m_Loaded = true;
                }
                if (p_Pixels != nullptr) {
                    stbi_image_free(p_Pixels);
                }
            }
            else if ((p_Material->Get(AI_MATKEY_BASE_COLOR, m_Color) == AI_SUCCESS) ||
                (p_Material->Get(AI_MATKEY_COLOR_DIFFUSE, m_Color) == AI_SUCCESS)) {
                m_Brush.m_Bitmap = static_cast<color*>(malloc(sizeof(color)));
                FLX_SMART_CHECK(m_Brush.m_Bitmap != NULL, "Failed to allocate brush bitmap");
                m_Brush.m_Bitmap[0].c[0] = static_cast<uint8>(m_Color.r * 255);
                m_Brush.m_Bitmap[0].c[1] = static_cast<uint8>(m_Color.g * 255);
                m_Brush.m_Bitmap[0].c[2] = static_cast<uint8>(m_Color.b * 255);
                m_Brush.m_Bitmap[0].c[3] = static_cast<uint8>(m_Color.a * 255);
                m_Brush.m_Width = 1;
                m_Brush.m_Height = 1;
                m_Loaded = true;
            }
            if (!m_Loaded) {
                // fallback: grey 1x1
                m_Brush.m_Bitmap = static_cast<color*>(malloc(sizeof(color)));
                FLX_SMART_CHECK(m_Brush.m_Bitmap != NULL, "Failed to allocate brush bitmap");
                m_Brush.m_Bitmap[0].c[0] = 127;
                m_Brush.m_Bitmap[0].c[1] = 127;
                m_Brush.m_Bitmap[0].c[2] = 127;
                m_Brush.m_Bitmap[0].c[3] = 255;
                m_Brush.m_Width = 1;
                m_Brush.m_Height = 1;
            }
            m_MaterialBrush[m_MaterialIndex] = static_cast<int32>(oBrushes.size());
            oBrushes.push_back(m_Brush);
        }
        oBrushes[m_MaterialBrush[m_MaterialIndex]].m_ClumpAddresses.push_back(static_cast<uint32>(oClumps.size()));
        oClumps.push_back(m_Clump);
    }
}