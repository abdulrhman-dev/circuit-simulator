#pragma once
#include "raylib.h"
#include "Graph.h"
#include "UIConstants.h"

struct NodeObject
{
private:
    static inline int idCounter{ 0 };
public:
    Vector2 pos{};
    Graph::Node graphNode{};
    bool solved{ false };
    float value{};
    bool isGround{ false };

    NodeObject(Vector2 pos_, Graph::Node& graphNode_, bool solved_ = false, float value_ = 0)
        : pos(pos_)
        , graphNode(graphNode_)
        , solved(solved_)
        , value(value_)
    {
        graphNode.id = idCounter++;
    }

    NodeObject(Vector2 pos_)
        : pos(pos_)
    {
        graphNode.id = idCounter++;
    }

    void addEdge(NodeObject* node, DrawState& state) {
        graphNode.edges.push_back(Graph::Edge{ &(node->graphNode), state == DrawState::WIRE ? true : false });
    }

    void draw() const {
        DrawCircleV(pos, 5, isGround ? RED : UI::WIRE_COLOR);
    }

    bool isNeighbor(NodeObject& checkNode) {
        for (auto& graphNode : graphNode.edges) {
            if (checkNode.graphNode.id == graphNode.node->id)
                return true;
        }

        return false;
    }
};