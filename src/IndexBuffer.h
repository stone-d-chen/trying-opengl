#pragma once

class IndexBuffer
{
    public:
        IndexBuffer(const unsigned int *data, unsigned int count);
        ~IndexBuffer();

        void Bind();
        void Unbind();

        inline unsigned int GetCount() {return(m_Count);}
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;
};