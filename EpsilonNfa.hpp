#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

namespace final_project
{
    template <typename TKey, typename TLabel>
    class EpsilonNfa
    {
        struct NfaNode
        {
            std::unordered_multimap<TKey, NfaNode *> edges = std::unordered_multimap<TKey, NfaNode *>();
            std::vector<std::pair<std::function<bool(TKey)>, NfaNode *>> predicateEdges = std::vector<std::pair<std::function<bool(TKey)>, NfaNode *>>();
            std::vector<NfaNode *> epsilonEdges = std::vector<NfaNode *>();
            TLabel label;
            bool accepting;

            NfaNode(TLabel label, bool accepting)
            {
                this->label = label;
                this->accepting = accepting;
            }

            void AddEdge(TKey key, NfaNode *node)
            {
                edges.insert(std::pair<TKey, NfaNode *>(key, node));
            }

            void AddEdge(std::function<bool(TKey)> predicate, NfaNode *node)
            {
                predicateEdges.push_back(std::make_pair(predicate, node));
            }

            void AddEpsilonEdge(NfaNode *node)
            {
                epsilonEdges.push_back(node);
            }

            void Access(TKey key, std::unordered_set<NfaNode *> *outputSet)
            {
                if (edges.count(key) > 0)
                {
                    auto matchingEdges = edges.equal_range(key);
                    for (auto i = matchingEdges.first; i != matchingEdges.second; i++)
                    {
                        outputSet->insert(i->second);
                    }
                }

                for (auto pair : predicateEdges)
                {
                    if (outputSet->find(pair.second) == outputSet->end() 
                        && pair.first(key))
                    {
                        outputSet->insert(pair.second);
                    }
                }
            }
        } *initialNode = nullptr;

        std::unordered_map<TLabel, NfaNode *> nodes;

    public:
        EpsilonNfa()
        {
            nodes = std::unordered_map<TLabel, NfaNode *>();
        }

        std::unordered_set<NfaNode *> *GetInitialNode()
        {
            auto newSet = new std::unordered_set<NfaNode *>();
            newSet->insert(initialNode);
            TraverseEpsilon(newSet);
            return newSet;
        }

        void AddInitialNode(TLabel label, bool accepting)
        {
            initialNode = new NfaNode(label, accepting);
            nodes.insert(std::pair<TLabel, NfaNode *>(label, initialNode));
        }

        void AddNode(TLabel label, bool accepting)
        {
            nodes.insert(std::pair<TLabel, NfaNode *>(label, new NfaNode(label, accepting)));
        }

        void AddEdge(TLabel startLabel, TKey key, TLabel endLabel)
        {
            auto pos = nodes.find(startLabel);
            pos->second->AddEdge(key, nodes.find(endLabel)->second);
        }

        void AddEdge(TLabel startLabel, std::function<bool(TKey)> predicate, TLabel endLabel)
        {
            auto pos = nodes.find(startLabel);
            pos->second->AddEdge(predicate, nodes.find(endLabel)->second);
        }

        void AddEpsilonEdge(TLabel startLabel, TLabel endLabel)
        {
            auto pos = nodes.find(startLabel);
            pos->second->AddEpsilonEdge(nodes.find(endLabel)->second);
        }

        bool IsAccepting(std::unordered_set<NfaNode *> *currentState)
        {
            for (auto node : *currentState)
            {
                if (node->accepting)
                {
                    return true;
                }
            }
            return false;
        }

        template <typename Iterator>
        bool TryAcceptString(Iterator begin, Iterator end)
        {
            auto current = GetInitialNode();
            for (auto it = begin; it != end; ++it)
            {
                current = Traverse(current, *it);
            }

            return IsAccepting(current);
        }

        template <typename Container>
        bool TryAcceptString(const Container &container)
        {
            return TryAcceptString(container.begin(), container.end());
        }

        std::unordered_set<NfaNode *> *Traverse(std::unordered_set<NfaNode *> *currentState, TKey key)
        {
            auto newSet = new std::unordered_set<NfaNode *>;
            for (auto node : *currentState)
            {
                node->Access(key, newSet);
            }

            TraverseEpsilon(newSet);

            return newSet;
        }

        void TraverseEpsilon(std::unordered_set<NfaNode *> *outputSet)
        {
            auto epsilonSet = std::unordered_set<NfaNode *>();
            bool sizeChanged = true;
            while (sizeChanged)
            {
                epsilonSet.clear();

                for (auto node : *outputSet)
                {
                    for (auto epsilonNode : node->epsilonEdges)
                    {
                        if (outputSet->find(epsilonNode) == outputSet->end())
                        {
                            epsilonSet.insert(epsilonNode);
                        }
                    }
                }

                int previousSize = outputSet->size();
                outputSet->insert(epsilonSet.begin(), epsilonSet.end());
                sizeChanged = previousSize != outputSet->size();
            }
        }
    };
}