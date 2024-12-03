#pragma once

#include "Engine/Core/Log.hpp"

namespace Engine {

enum class ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};  // Define the type of the data pushed into the buffer

/* Return the size (int bytes) of the pushed data */
static size_t SizeOfShaderDateType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::None:
            return 0;
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 8;
        case ShaderDataType::Float3:
            return 12;
        case ShaderDataType::Float4:
            return 16;
        case ShaderDataType::Mat3:
            return 32;
        case ShaderDataType::Mat4:
            return 64;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 8;
        case ShaderDataType::Int3:
            return 12;
        case ShaderDataType::Int4:
            return 16;
        case ShaderDataType::Bool:
            return 1;
        default:
            break;
    }
    CORE_ERROR("Unkmown shader data type");
    return 0;
}

/* Represent the bundle of pushed data. Example: Vertices, UVs coordinates, Normal, Color, etc. */
struct ENGINE_API BufferElement {
    std::string Name;
    ShaderDataType Type;
    size_t Size;
    uint32_t Offset = 0;
    bool Normalized;

    BufferElement() : Normalized(false), Size(0), Type(ShaderDataType::None) {}
    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name),
          Type(type),
          Size(SizeOfShaderDateType(type)),
          Offset(0),
          Normalized(normalized) {}

    uint32_t GetComponentCount() const {
        switch (Type) {
            case ShaderDataType::None:
                return 0;
            case ShaderDataType::Float:
                return 1;
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Float4:
                return 4;
            case ShaderDataType::Mat3:
                return 9;
            case ShaderDataType::Mat4:
                return 16;
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Bool:
                return 1;
            default:
                break;
        }

        CORE_ERROR("Unkmown shader data type");
        return 0;
    }
};

/* The buffer layout defines the layout of stored data into the vertex buffer */
class ENGINE_API BufferLayout {
   public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    inline uint32_t GetStride() const { return m_Stride; }
    inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

   private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class ENGINE_API VertexBuffer {
   public:
    VertexBuffer() = default;
    virtual ~VertexBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static VertexBuffer* Create(uint32_t size);
    static VertexBuffer* Create(float* vertices, uint32_t size);
};

class ENGINE_API ElementBuffer {
   public:
    ElementBuffer() = default;
    virtual ~ElementBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    inline uint32_t GetCount() const { return m_Count; };

    static ElementBuffer* Create(uint32_t* indices, uint32_t count);

   protected:
    uint32_t m_Count = 0;
};

}  // namespace Engine