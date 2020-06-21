#include "Debug.h"
#include <libllist.h>
#include <gtest/gtest.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace llist;


static const size_t MAX_RANDOM_TRIALS   = 100u;
static const size_t MAX_RANDOM_DATA_VAL = 0x100u;
static const std::vector<size_t> node_data    =  {1,4,12,40,62,3};
static const std::vector<size_t> check_data   = {3,340,1,4,56,234};

class GreaterEqual {
  int _than;

public:
  GreaterEqual(int th):_than(th){
    
  }
  bool operator()(int data) const 
  {
    return data >= _than;
  }
};

static bool FindGreaterThan(LListNode<size_t>* iterate_node, size_t& data)
{return iterate_node->data_ >= data;}

static bool FindEqualTo(LListNode<size_t>* iterate_node, size_t& data)
{return iterate_node->data_ == data ;}

static void PrintNode(LListNode<size_t>* node)
{printf("Node: data=%lu ",node->data_);}

static LListNode<size_t>* InsertNewNodeHead(LList<size_t>& list, size_t data)
{
    LListNode<size_t> * node;
    node = new(LListNode<size_t>);
    node->data_ = data;
    list.InsertHead(node);
    return node;
}

static LListNode<size_t> * CheckForEqualData(LList<size_t>& list, size_t data)
{
    return  list.FindNode(FindEqualTo, data);
}

static LListNode<size_t> * CheckForGreaterData(LList<size_t>& list, size_t data)
{
    return list.FindNode(FindGreaterThan, data);
}

TEST(LListTest, InsertNodesHead) { 
    LList<size_t> list;
    std::vector<LListNode<size_t> *> added_nodes;
    
    /* Insert nodes with predefined data */
    for (auto& data : node_data)
    {
        LListNode<size_t> * temp_node;
        temp_node = InsertNewNodeHead(list, data);
        added_nodes.push_back(temp_node);
    }

    list.PrintList(PrintNode);

    /* Check for node data */
    for (auto& data : check_data)
    {
        LListNode<size_t> * temp_node;

        temp_node = CheckForEqualData(list, data);
        if (std::find(node_data.begin(), node_data.end(), data) == node_data.end())
        {
            ASSERT_EQ(temp_node, nullptr);
        }
        else
        {
            ASSERT_EQ(temp_node->data_, data);
        }

        temp_node = CheckForGreaterData(list, data);
        auto temp_iter = std::find_if(node_data.begin(), node_data.end(), GreaterEqual(data));
        if (temp_iter == node_data.end())
        {
            ASSERT_EQ(temp_node, nullptr);
        }
        else
        {
            ASSERT_GE(temp_node->data_, data);
        }
    }

    /* Ensure each added node is in list */
    for (auto& node : added_nodes)
    {
        LListNode<size_t> * find_node;
        find_node = list.FindNode(FindEqualTo, node->data_);
        ASSERT_EQ(find_node, node);
    }    
}

TEST(LListTest, InsertNodesRandomly) { 
    LList<size_t> list;
    srand( (unsigned)time(NULL) );
    std::vector<LListNode<size_t> *> added_nodes;
    std::vector<size_t> added_data;

    /* Insert nodes with random data */
    for (unsigned int i = 0; i < MAX_RANDOM_TRIALS; i++)
    {
        LListNode<size_t> * temp_node;
        size_t temp_data = static_cast<size_t>((rand() % MAX_RANDOM_DATA_VAL));
        temp_node = InsertNewNodeHead(list, temp_data);
        added_nodes.push_back(temp_node);
        added_data.push_back(temp_data);
    }

    list.PrintList(PrintNode);

    /* Check for random data values in list */
    for (unsigned int i = 0; i < MAX_RANDOM_TRIALS; i++)
    {
        LListNode<size_t> * temp_node;
        size_t temp_data = static_cast<size_t>((rand() % MAX_RANDOM_DATA_VAL));
        
        temp_node = CheckForEqualData(list, temp_data);
        if (std::find(added_data.begin(), added_data.end(), temp_data) == added_data.end())
        {
            ASSERT_EQ(temp_node, nullptr);
        }
        else
        {
            ASSERT_EQ(temp_node->data_, temp_data);
        }
     
        temp_node = CheckForGreaterData(list, temp_data);
        auto temp_iter = std::find_if(added_data.begin(), added_data.end(), GreaterEqual(temp_data));
        if (temp_iter == added_data.end())
        {
            ASSERT_EQ(temp_node, nullptr);
        }
        else
        {
            ASSERT_GE(temp_node->data_, temp_data);
        }
    }    

    /* Ensure each added node is in list */
    for (auto& node : added_nodes)
    {
        LListNode<size_t> * find_node;
        find_node = list.FindNode(FindEqualTo, node->data_);
        ASSERT_EQ(find_node->data_, node->data_);
    }    
}
 

TEST(LListTest, RemoveNodes) { 
    LList<size_t> list;
    std::vector<LListNode<size_t> *> added_nodes;
    Status_t status;

    for (auto& data : node_data)
    {
        LListNode<size_t> * temp_node;
        temp_node = InsertNewNodeHead(list, data);
        added_nodes.push_back(temp_node);
    }

    list.PrintList(PrintNode);

    /* Remove all added nodes */
    for (auto& node : added_nodes)
    {
        status = list.RemoveNode(node);
        ASSERT_EQ(status, kStatusSuccess);
    }

    /* Ensure none of the added nodes are in list */
    for (auto& node : added_nodes)
    {
        LListNode<size_t> * find_node;
        find_node = list.FindNode(FindEqualTo, node->data_);
        ASSERT_EQ(find_node, nullptr);
    }    
}


TEST(LListTest, ClearNodes) { 
    LList<size_t> list;
    std::vector<LListNode<size_t> *> added_nodes;
    
    for (auto& data : node_data)
    {
        LListNode<size_t> * temp_node;
        temp_node = InsertNewNodeHead(list, data);
        added_nodes.push_back(temp_node);
    }

    list.PrintList(PrintNode);

    /* Remove all added nodes */
    list.Clear();

    /* Ensure none of the added nodes are in list */
    for (auto& node : added_nodes)
    {
        LListNode<size_t> * find_node;
        find_node = list.FindNode(FindEqualTo, node->data_);
        ASSERT_EQ(find_node, nullptr);
    }    
}
 

TEST(LListTest, RemoveNodesRandomly) { 
    LList<size_t> list;
    srand( (unsigned)time(NULL) );
    Status_t status;
    std::vector<LListNode<size_t> *> added_nodes;

    /* Insert nodes with random data */
    for (unsigned int i = 0; i < MAX_RANDOM_TRIALS; i++)
    {
        LListNode<size_t> * temp_node;
        size_t temp_data = static_cast<size_t>((rand() % MAX_RANDOM_DATA_VAL));
        temp_node = InsertNewNodeHead(list, temp_data);
        added_nodes.push_back(temp_node);
    }

    list.PrintList(PrintNode);

    /* Remove all added nodes */
    for (auto& node : added_nodes)
    {
        status = list.RemoveNode(node);
        ASSERT_EQ(status, kStatusSuccess);
    }

    /* Ensure none of the added nodes are in list */
    for (auto& node : added_nodes)
    {
        LListNode<size_t> * find_node;
        find_node = list.FindNode(FindEqualTo, node->data_);
        ASSERT_EQ(find_node, nullptr);
    }    
}
 
TEST(LListTest, InsertNodes) { 
    LList<size_t> list;
    Status_t status;
    LListNode<size_t> * node_ptr;
    std::vector<LListNode<size_t>> nodes(5);

    list.InsertTail(&nodes[0]);
    list.InsertTail(&nodes[1]);
    list.InsertTail(&nodes[3]);

    node_ptr = list.GetHead();
    ASSERT_EQ(node_ptr, &nodes[0]);
    
    node_ptr = list.GetTail();
    ASSERT_EQ(node_ptr, &nodes[3]);

    status = list.InsertNodeBefore(&nodes[3], &nodes[2]);
    ASSERT_EQ(status, kStatusSuccess);


    node_ptr = list.GetTail();
    ASSERT_EQ(node_ptr, &nodes[3]);
    
    list.RemoveTail();
    node_ptr = list.GetTail();
    ASSERT_EQ(node_ptr, &nodes[2]);

    
    list.RemoveHead();
    node_ptr = list.GetHead();
    ASSERT_EQ(node_ptr, &nodes[1]);
 
    status = list.InsertNodeAfter(&nodes[1], &nodes[4]);
    ASSERT_EQ(status, kStatusSuccess);
    list.RemoveHead();
    node_ptr = list.GetHead();
    ASSERT_EQ(node_ptr, &nodes[4]);
 
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}