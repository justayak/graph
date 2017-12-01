#include <iostream>
#include <fstream>
#include <stdexcept>

#include "andres/graph/graph.hxx"
#include "andres/graph/complete-graph.hxx"
#include "andres/graph/multicut-lifted/greedy-additive.hxx"
#include "andres/graph/multicut-lifted/kernighan-lin.hxx"


inline void test(bool pred)
{
    if(!pred)
        throw std::runtime_error("Test failed.");
}

using namespace andres::graph;

void testMulticutLifted()
{
    std::cout << "HI SABAA" << std::endl;

    std::string url_edges = "/home/cabbage/prototyping/edges.txt";
    std::string url_lifted_edges = "/home/cabbage/prototyping/lifted_edges.txt";
    std::string url_config = "/home/cabbage/prototyping/config.txt";

    std::ifstream fconfig(url_config);
    if (!fconfig) {
      std::cout << "no file " << url_config << std::endl;
      exit(1);
    }

    int count = 0;
    fconfig >> count;

    std::cout << "Create Graph with size " << count << std::endl;

    Graph<> original_graph(count);
    CompleteGraph<> lifted_graph(count);

    std::vector<double> weights(lifted_graph.numberOfEdges());

    // --

    std::ifstream fedges(url_edges);
    if (!fedges) {
      std::cout << "no file " << url_edges << std::endl;
      exit(1);
    }

    int i, j;
    double ce;


    while (fedges >> i) {
      fedges >> j;
      fedges >> ce;

      // std::cout << "--------------" << std::endl;
      // std::cout << i << "," << j << "," << ce << std::endl;
      original_graph.insertEdge(i, j);
      weights[lifted_graph.findEdge(i, j).second] = ce;
    }

    std::ifstream flifted_edges(url_lifted_edges);
    if (!flifted_edges) {
      std::cout << "no file " << url_lifted_edges << std::endl;
      exit(1);
    }

    while (flifted_edges >> i) {
      flifted_edges >> j;
      fedges >> ce;
      weights[lifted_graph.findEdge(i, j).second] = ce;
    }

    std::cout << "graph is build... attempt to solve it" << std::endl;

    // std::vector<char> edge_labels(lifted_graph.numberOfEdges(), 1);
    // multicut_lifted::greedyAdditiveEdgeContraction(original_graph, lifted_graph, weights, edge_labels);

    std::vector<char> edge_labels(lifted_graph.numberOfEdges(), 1);
    multicut_lifted::kernighanLin(original_graph, lifted_graph, weights, edge_labels, edge_labels);

    std::cout << "done" << std::endl;

    auto url_output = "result.txt";
    std::ofstream foutput;
    foutput.open(url_output);

    for (i = 0; i < count; ++i) {
      for (j=(i+1); j < count; ++j) {
        foutput << i << " " << j << " ";
        auto qqq = edge_labels[lifted_graph.findEdge(i, j).second] ;
        if(qqq == 0) {
            foutput << "0" << std::endl;
        } else if (qqq == 1) {
          foutput << "1" << std::endl;
        } else {
          std::cout << "nope" << std::endl;
          exit(1);
        }

      }
    }

    foutput.close();

    // ===================

    // Graph<> original_graph(5);
    // original_graph.insertEdge(0, 1); // 0
    // original_graph.insertEdge(0, 3); // 1
    // original_graph.insertEdge(1, 2); // 2
    // original_graph.insertEdge(1, 4); // 3
    // original_graph.insertEdge(3, 4); // 4
    //
    // CompleteGraph<> lifted_graph(5);
    //
    // std::vector<double> weights(10);
    // weights[lifted_graph.findEdge(0, 1).second] = 10;
    // weights[lifted_graph.findEdge(0, 2).second] = -1;
    // weights[lifted_graph.findEdge(0, 3).second] = -1;
    // weights[lifted_graph.findEdge(0, 4).second] = -1;
    // weights[lifted_graph.findEdge(1, 2).second] = 10;
    // weights[lifted_graph.findEdge(1, 3).second] = -1;
    // weights[lifted_graph.findEdge(1, 4).second] = 4;
    // weights[lifted_graph.findEdge(2, 3).second] = -1;
    // weights[lifted_graph.findEdge(2, 4).second] = -1;
    // weights[lifted_graph.findEdge(3, 4).second] = 10;
    //
    // std::vector<char> edge_labels(lifted_graph.numberOfEdges(), 1);
    // multicut_lifted::greedyAdditiveEdgeContraction(original_graph, lifted_graph, weights, edge_labels);
    //
    // test(edge_labels[lifted_graph.findEdge(0, 1).second] == 0);
    // test(edge_labels[lifted_graph.findEdge(0, 2).second] == 0);
    // test(edge_labels[lifted_graph.findEdge(0, 3).second] == 1);
    // test(edge_labels[lifted_graph.findEdge(0, 4).second] == 1);
    // test(edge_labels[lifted_graph.findEdge(1, 2).second] == 0);
    // test(edge_labels[lifted_graph.findEdge(1, 3).second] == 1);
    // test(edge_labels[lifted_graph.findEdge(1, 4).second] == 1);
    // test(edge_labels[lifted_graph.findEdge(2, 3).second] == 1);
    // test(edge_labels[lifted_graph.findEdge(2, 4).second] == 1);
    // test(edge_labels[lifted_graph.findEdge(3, 4).second] == 0);
}

int main()
{
    testMulticutLifted();

    return 0;
}
