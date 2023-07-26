
class Renderer
{
    public:
        void Draw(VertexArray &va, IndexBuffer &ib, Shader &Shader);
    private:
};



void Renderer::Draw(VertexArray& va, IndexBuffer& ib, Shader& shader)
{
    glClearColor(0,0,0,1);
    shader.Bind();
    va.Bind();
    glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, 0);
}