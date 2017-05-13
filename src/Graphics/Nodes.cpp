#include "Nodes.h"

namespace Scenegraph
{
    /*** Group node
    */
    Group::~Group()
    {
        for (Node* node : m_children)
            delete node;
    }

    void Group::Draw(glm::mat4 C)
    {
        for (Node* node : m_children)
            node->Draw(C);
    }

    void Group::Update(glm::mat4 C)
    {
        for (Node* node : m_children)
            node->Update(C);
    }

    bool Group::AddChild(Node* node)
    {
        m_children.push_back(node);
        return true;
    }

    bool Group::RemoveChild(Node* node)
    {
        auto currNode = m_children.begin();
        auto end = m_children.end();

        for (; currNode != end; currNode++)
        {
            if (*currNode == node)      // Ptr comparison
            {
                m_children.erase(currNode);
                return true;
            }
        }

        return false;
    }

    /*** MatrixTransform
    */
    MatrixTransform::MatrixTransform(glm::mat4 matrix)
    {
        M = std::move(matrix);
    }

    void MatrixTransform::Draw(glm::mat4 C)
    {
        Group::Draw(C*M);
    }

    void MatrixTransform::Update(glm::mat4 C)
    {
        Group::Update(C*M);
    }

    void MatrixTransform::SetM(glm::mat4 M2)
    {
        M = std::move(M2);
    }

    /*** Geode Node
    */
    void Geode::Draw(glm::mat4 C)
    {
        //glm::mat4 glmatrix = C * model2world;
        Render();
    }
}
