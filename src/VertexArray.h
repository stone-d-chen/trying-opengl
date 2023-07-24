#include <glad/glad.h>
#include <vector>
#include "VertexBuffer.h"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return(0);
    }
};

class VertexBufferLayout
{
    public:
        VertexBufferLayout()
            : m_Stride(0) {}

        template<typename T>
        void Push(unsigned int count)
        {
            static_assert(false);
        }
        template<>
        void Push<float>(unsigned int count)
        {
            m_Elements.push_back(VertexBufferElement{GL_FLOAT, count, false});
            m_Stride += 4 * count;
        }
        template<>
        void Push<unsigned int>(unsigned int count)
        {
            m_Elements.push_back(VertexBufferElement{GL_UNSIGNED_INT, count, false});
            m_Stride += 4 * count;
        }
        template<>
        void Push<unsigned char>(unsigned int count)
        {
            m_Elements.push_back(VertexBufferElement{GL_UNSIGNED_BYTE, count, true});
            m_Stride += sizeof(GLbyte) * count;
        }

        inline const std::vector<VertexBufferElement> GetElements() const {return(m_Elements);}
        inline unsigned int GetStride() {return(m_Stride);}

    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;
};


class VertexArray
{
    public:
        VertexArray();
        // ~VertexArray();


        void AddBuffer(VertexBuffer &vb, VertexBufferLayout &layout);

        void Bind();
        void Unbind();
    private:
        unsigned int m_RendererId;
};

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererId);
}

void VertexArray::Bind()
{
     glBindVertexArray(m_RendererId);
}

void VertexArray::Unbind()
{
     glBindVertexArray(0);
}

void VertexArray::AddBuffer(VertexBuffer &vb, VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    unsigned int offset = 0;
    const auto &elements = layout.GetElements();
    for(unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized,
        layout.GetStride(), (const void *)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

    }
}