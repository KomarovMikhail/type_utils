#pragma once

#include<iostream>
#include<type_traits>
#include<type_info.h>


namespace type_utils
{

    struct Tree
    {
        // Pair

        template<typename TKey, typename TValue>
        struct Pair
        {
            using key = TKey;
            using value = TValue;
        };

    private:

        // EmptyNode
        struct EmptyNode {};


        // Print
        template<typename TNode>
        static void PrintHelper(std::ostream& ostream, size_t level, char side)
        {
            for(size_t i = 0; i < level; ++i) {
                ostream << "|";
            }

            ostream << "-" << side << "(" << Info::Name<typename TNode::key>() << ", " 
                << Info::Name<typename TNode::value>() << ")[" 
                << Info::Hash<typename TNode::key>() << "]" << std::endl;
            PrintHelper<typename TNode::leftChild>(ostream, level + 1, 'L');
            PrintHelper<typename TNode::rightChild>(ostream, level + 1, 'R');
        };


        // Node

        template<typename ...T>
        struct Node;


        template<typename TKey, typename TValue>
        struct Node<TKey, TValue>
        {
            using key = TKey;
            using value = TValue;
            using leftChild = EmptyNode;
            using rightChild = EmptyNode;
        };

        template<typename TKey, typename TValue, typename TLeftChild, typename TRightChild>
        struct Node<TKey, TValue, TLeftChild, TRightChild>
        {
            using key = TKey;
            using value = TValue;
            using leftChild = TLeftChild;
            using rightChild = TRightChild;
        };


        // NodeSetValue

        template<typename TNode, typename TNewValue>
        struct NodeSetValue
        {
            using result = Node<typename TNode::key, TNewValue, typename TNode::leftChild, typename TNode::rightChild>;
        };


        // InsertNodeHelper

        template<typename TNode, typename TKey, typename TValue>
        struct InsertNodeHelper
        {
            using result = typename std::conditional<
                Info::Hash<TKey>() == Info::Hash<typename TNode::key>(),
                typename NodeSetValue<TNode, TValue>::result,
                typename std::conditional<
                    Info::Hash<TKey>() >= Info::Hash<typename TNode::key>(),
                    Node<
                        typename TNode::key, 
                        typename TNode::value, 
                        typename TNode::leftChild, 
                        typename InsertNodeHelper<typename TNode::rightChild, TKey, TValue>::result
                    >,
                    Node<
                        typename TNode::key, 
                        typename TNode::value, 
                        typename InsertNodeHelper<typename TNode::leftChild, TKey, TValue>::result, 
                        typename TNode::rightChild
                    >
                >::type
            >::type;
        };

        template<typename TKey, typename TValue>
        struct InsertNodeHelper<EmptyNode, TKey, TValue>
        {
            using result = Node<TKey, TValue>;
        };


        // CompoundPairsHelper

        template<typename ...TPair>
        struct CompoundPairsHelper;

        template<typename TPair>
        struct CompoundPairsHelper<TPair> 
        {
            using result = Node<typename TPair::key, typename TPair::value>;
        };

        template<typename TPairHead, typename ...TPairTail>
        struct CompoundPairsHelper<TPairHead, TPairTail...> 
        {
            using result = typename InsertNodeHelper<
                typename CompoundPairsHelper<TPairTail...>::result,
                typename TPairHead::key,
                typename TPairHead::value
            >::result;
        };


        // GetHelper

        template<typename TNode, typename TKey>
        struct GetHelper
        {
            // static_assert(!std::is_same<TNode, EmptyNode>::value, "Get: key type not found in type tree");
            using result = typename std::conditional<
                Info::Hash<TKey>() == Info::Hash<typename TNode::key>(),
                typename TNode::value,
                typename std::conditional<
                    Info::Hash<TKey>() >= Info::Hash<typename TNode::key>(),
                    typename GetHelper<typename TNode::rightChild, TKey>::result,
                    typename GetHelper<typename TNode::leftChild, TKey>::result
                >::type
            >::type;
        };

        template<typename TKey>
        struct GetHelper<EmptyNode, TKey>
        {
            // static_assert(!std::is_same<TNode, EmptyNode>::value, "Get: key type not found in type tree");
            using result = void;
        };


    public:

        // Container

        template<typename ...TPair>
        struct Container
        {
            using root = typename CompoundPairsHelper<TPair...>::result;
        };


        // Get

        template<typename TTree, typename TKey>
        struct Get
        {
            using result = typename GetHelper<typename TTree::root, TKey>::result;
        };


        // Print

        template<typename TTree>
        static void Print(std::ostream& ostream)
        {
            PrintHelper<typename TTree::root>(ostream, 0, 'O');
        }

    }; // struct Tree

    template<>
    void Tree::PrintHelper<Tree::EmptyNode>(std::ostream& ostream, size_t level, char side)
    {
        return;
    }

    template<>
    struct Tree::CompoundPairsHelper<> 
    {
        using result = EmptyNode;
    };

} // namespace type_utils
