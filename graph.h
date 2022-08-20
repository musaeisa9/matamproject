/*
 * graph.h
 *
 *  Created on: Aug 5, 2020
 *      Author: JLAL
 */

#ifndef GRAPH_H_
#define GRAPH_H_
#include <string>
#include <iostream>
#include <ostream>
#include <istream>
#include<fstream>
#include<sstream>
#include<cstdint>
#include<cstdio>
#include<cstring>
#include<cctype>
#include<vector>
#include<map>
#include <set>
#include <algorithm>
using std::string;
using std::set;
using std::vector;
using std::map;
using std::ostream;
using std::istream;
using std::endl;
using std::cout;
using std::cin;
using std::cerr;


namespace exeptions{
    class generalError{
    public:
        string info;
        generalError(string error_name);
    };
    class invalidGraphName{
    public:
        string info;
        invalidGraphName(string name);
    };

    class cannot_open_file{
    public:
        string info;
        cannot_open_file(string file_name);
    };
    class graphNotExist{
    public:
        string info;
        graphNotExist(string graph_name);
    };

    class unrecognizedCommand{
    public:
        string info;
        unrecognizedCommand(string command = "");
    };

    class syntaxErrorWithLiteralGraph{
public:
    string info;
    syntaxErrorWithLiteralGraph(string name);
    };
}
class Edge{
	const string from;
	const string to;
public:
	Edge(string from_vertex, string to_vertex);
	Edge(const Edge&)= default;
	bool operator<(Edge)const;
	bool operator>(Edge)const;
	bool operator<=(Edge)const;
	bool operator>=(Edge)const;
	bool operator==(Edge)const;
	string getFrom()const;
	string getTo()const;
	friend ostream& operator<<(ostream& os,const Edge& edge);
};
/*template<class T>
T& operator<<(T& os,const Edge& edge){
    return os<<edge.from<<" "<<edge.to<<endl;

}*/


class Graph{
	set<string> vertexes;
	set<Edge> edges;
public:
	Graph()=default;
	Graph(const set<string>& vertexes_set, const set<Edge>& edges_set);
	Graph(const Graph&)=default;
	Graph& operator=(const Graph&)=default;
	void addVertex(const string& vertex);
	void addEdge(const Edge& edge);
	Graph operator!();
	friend Graph operator+(const Graph& graph1, const Graph& graph2);
	friend Graph operator-(const Graph& graph1, const Graph& graph2);
	friend Graph operator^(const Graph& graph1, const Graph& graph2);
	friend Graph operator*(const Graph& graph1, const Graph& graph2);
	friend void saveGraphToFile(const Graph&, string);
	friend ostream& operator<<(ostream& os,const Graph& graph);
	friend string graphToLiteral(Graph);
	class badEdge{};
	class badVertex{};
    class vertexExist{};
    class edgeExist{};


};
/*template<class T>
T& operator<<(T& os,const Graph& graph){
    for(set<string>::iterator i = graph.vertexes.begin(); i!= graph.vertexes.end(); ++i){
        os<<*i<<endl;
    }
    os<<"$"<<endl;
    for(set<Edge>::iterator i = graph.edges.begin(); i!= graph.edges.end(); ++i){
        os<<*i;
    }
    return os;
}
*/

bool vertexIsValid(string);

Graph loadGraphFromFile(string);

string removeSpaceFromBeginingAndEnd(const string&);

Graph getGraphByName(string graph_name,map<string,Graph>& map_of_graphs);

Graph loadCommand(string);

Graph literalToGraph(string);

int commaNumberInString(string);

void addLiteralVertexesToGraph(Graph&, string);


void addLiteralEdgesToGraph(Graph&, string);

string parserEdge(string);


Graph calculateOnariCommand(string command, map<string,Graph>& map_of_graphs);


string replaceLoadCommand(string command, map<string,Graph>& map_op_graphs);

Graph calculateBinaryGraph(Graph g1,Graph g2,char op);

string calculateSimpleGraph(string command, map<string,Graph>& map_of_graphs);


unsigned int bracketsNum(string str, unsigned int& last_lift,unsigned int & last_right);

Graph calculateGraph(string command, map<string,Graph>& map_of_graphs);

bool graphNameIsValid(string name);

void assignmentCommand(string command, map<string,Graph>& map_of_graphs);

void printGraphsNameInMap(map<string,Graph>& map_of_graphs, ostream& os);

void deleteCommand(string command, map<string,Graph>& map_of_graphs);

void printCommand(string command, map<string,Graph>& map_of_graphs, ostream& os);

void saveCommand(string command, map<string,Graph>& map_of_graphs);

bool doCommand(string command, map<string,Graph>& map_of_graphs, ostream& os);

void calculator(ostream& os, istream& is, string prompt);

#endif /* GRAPH_H_ */
