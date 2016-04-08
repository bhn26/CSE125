#pragma once

// Scenegraph nodes defined here.
// Node: the toplevel interface
// Group: can have children
// MatrixTransform: specific type of group that transforms the children by a matrix
// Geode: a type of node that should be derived from to draw objects

#include <glm/glm.hpp>
#include <list>

namespace Scenegraph
{
    struct Node
    {
        virtual void Draw(glm::mat4 C) = 0;
        virtual void Update(glm::mat4 C) = 0;
    };

    /* Group node. This node can hold multiple other nodes
    */
    struct Group : public Node
    {
        std::list<Node*> children;

        Group() : children(std::list<Node*>()) {}
        ~Group();

        void Draw(glm::mat4 C) override;
        void Update(glm::mat4 C) override;

        bool AddChild(Node *);
        bool RemoveChild(Node *);
    };

    /* Matrix Transform node. Objects attached to this will be modified by the matrix in the node
    */
    struct MatrixTransform : public Group
    {
        glm::mat4 M;

        MatrixTransform(glm::mat4);

        void Draw(glm::mat4 C) override;
        void Update(glm::mat4 C) override;

        void SetM(glm::mat4 M2);
    };

    /* Geode Node. Things that are geometric objects should inherit from this
    */
    struct Geode : public Node
    {
        double dim;
        glm::mat4 model2world;
        glm::vec3 color;

        Geode(double d) : Geode(d, glm::vec3(1.0f)) {}
        Geode(double d, glm::vec3 col) : dim(d), color(col), model2world(glm::mat4(1.0f)) {}

        void Draw(glm::mat4 C) override;
        virtual void Render() = 0;
    };
}
