/*
 * graph.cpp
 *
 *  Created on: Aug 5, 2020
 *      Author: JLAL
 */
#include "graph.h"
const string error_string = "Error: ";
exeptions::graphNotExist::graphNotExist(string graph_name):info(graph_name){}
exeptions::cannot_open_file::cannot_open_file(string file_name):info(file_name){}
exeptions::unrecognizedCommand::unrecognizedCommand(string command):info(command){}
exeptions::syntaxErrorWithLiteralGraph::syntaxErrorWithLiteralGraph(string name):info(name){}
exeptions::invalidGraphName::invalidGraphName(string name):info(name){}
exeptions::generalError::generalError(string error_name):info(error_name){}

Edge::Edge(string from_vertex, string to_vertex):from(from_vertex),to(to_vertex){}

	bool Edge::operator<(Edge edge)const{
		if (from == edge.from){
			return to < edge.to;
		}
		return from < edge.from;
	}
	bool Edge::operator>(Edge edge)const{
		if (from == edge.from){
			return to > edge.to;
		}
		return from > edge.from;
	}
	bool Edge::operator<=(Edge edge)const{
		return !(this->operator >(edge));
	}
	bool Edge::operator>=(Edge edge)const{
		return !(this->operator <(edge));
	}
	bool Edge::operator==(Edge edge)const{
		return from == edge.from && to == edge.to;
	}
    string Edge::getFrom()const{
    	return from;
    }
    string Edge::getTo()const{
    	return to;
    }
    ostream& operator<<(ostream& os,const Edge& edge){
    	return os<<edge.from<<" "<<edge.to<<endl;
    }



    Graph::Graph(const set<string>& vertexes_set, const set<Edge>& edges_set):vertexes(vertexes_set),edges(edges_set){}

    void Graph::addVertex(const string& vertex){
        if(!vertexIsValid(vertex)){
            throw badVertex();
        }
        if(vertexes.find(vertex)!= vertexes.end()){
            throw vertexExist();
        }
    	vertexes.insert(vertex);
    }
    void Graph::addEdge(const Edge& edge){
    	if(vertexes.find(edge.getFrom()) != vertexes.end() && vertexes.find(edge.getTo()) != vertexes.end() && edge.getFrom() != edge.getTo()){
    		if(edges.find(edge) != edges.end()){
                throw edgeExist();
    		}
    		edges.insert(edge);
    		return;
    	}
    	throw badEdge();
    }


    Graph Graph::operator!(){
        set<string> new_vertex_set = vertexes;
        set<Edge> new_edges_set;
        for(set<string>::iterator i = new_vertex_set.begin(); i!= new_vertex_set.end(); ++i){
            for(set<string>::iterator j = new_vertex_set.begin(); j!= new_vertex_set.end(); ++j){
                if (edges.find(Edge(*i,*j)) == edges.end() && *i != *j){
                    new_edges_set.insert(Edge(*i,*j));
                }
            }
        }
        return Graph(new_vertex_set, new_edges_set);


    }



    ostream& operator<<(ostream& os,const Graph& graph){
    	for(set<string>::iterator i = graph.vertexes.begin(); i!= graph.vertexes.end(); ++i){
    		os<<*i<<endl;
    	}
    	os<<"$"<<endl;
    	for(set<Edge>::iterator i = graph.edges.begin(); i!= graph.edges.end(); ++i){
    	    os<<*i;
    	}
    	return os;
    }

    Graph operator+(const Graph& graph1, const Graph& graph2){
    	set<string> new_vertex_set = graph1.vertexes;
    	new_vertex_set.insert(graph2.vertexes.begin(),graph2.vertexes.end());
    	set<Edge> new_edges_set = graph1.edges;
    	new_edges_set.insert(graph2.edges.begin(),graph2.edges.end());
    	return Graph(new_vertex_set,new_edges_set);
    }
    Graph operator-(const Graph& graph1, const Graph& graph2){
        set<string> new_vertex_set = graph1.vertexes;
        for(set<string>::iterator i = graph2.vertexes.begin(); i != graph2.vertexes.end(); ++i){
            new_vertex_set.erase(*i);
        }
        set<Edge> new_edges_set;
        for(set<Edge>::iterator i = graph1.edges.begin(); i!= graph1.edges.end();++i){
            if(new_vertex_set.find((*i).getFrom()) != new_vertex_set.end() && new_vertex_set.find((*i).getTo()) != new_vertex_set.end()){
                new_edges_set.insert(*i);
            }
        }
        return Graph(new_vertex_set,new_edges_set);

    }

    Graph operator^(const Graph& graph1, const Graph& graph2){
    	set<string> new_vertex_set = graph1.vertexes;
    	set<Edge> new_edges_set = graph1.edges;
    	const set<string>& vertex_set2 = graph2.vertexes;
    	const set<Edge>& edges_set2 = graph2.edges;
    	set<string>::iterator it1 = new_vertex_set.begin();
    	set<string>::iterator it2 = vertex_set2.begin();
    	while ( (it1 != new_vertex_set.end()) && (it2 != vertex_set2.end()) ) {
    	    if (*it1 < *it2) {
    	    	new_vertex_set.erase(it1++);
    	    } else if (*it2 < *it1) {
    	        ++it2;
    	    } else {
    	            ++it1;
    	            ++it2;
    	    }
    	}
    	new_vertex_set.erase(it1, new_vertex_set.end());
    	std::set<Edge>::iterator i1 = new_edges_set.begin();
    	std::set<Edge>::iterator i2 = edges_set2.begin();
    	while ( (i1 != new_edges_set.end()) && (i2 != edges_set2.end()) ) {
    	    if (*i1 < *i2) {
    	    	new_edges_set.erase(i1++);
    	    } else if (*i2 < *i1) {
    	        ++it2;
    	    } else {
    	            ++i1;
    	            ++i2;
    	    }
    	}

    	new_edges_set.erase(i1, new_edges_set.end());
    	return Graph(new_vertex_set,new_edges_set);
    }

Graph operator*(const Graph& graph1, const Graph& graph2){
    set<string> new_vertex_set;
    set<Edge> new_edges_set;
    for(set<string>::iterator i = graph1.vertexes.begin(); i != graph1.vertexes.end(); ++i){
        for(set<string>::iterator j = graph2.vertexes.begin(); j != graph2.vertexes.end(); ++j){
            new_vertex_set.insert("[" + *i + ";" + *j + "]");
        }
    }
    for(set<Edge>::iterator i = graph1.edges.begin(); i != graph1.edges.end(); ++i){
        for(set<Edge>::iterator j = graph2.edges.begin(); j != graph2.edges.end(); ++j){
           string v1((*i).getFrom());
           string w1((*j).getFrom());
           string v2((*i).getTo());
           string w2((*j).getTo());
           Edge edge("[" + v1 + ";" + w1 + "]","[" + v2 + ";" + w2 + "]");
           new_edges_set.insert(edge);
        }
    }
    return Graph(new_vertex_set,new_edges_set);

}

bool vertexIsValid(string vertex){
    if (vertex.size() == 0){
        return false;
    }
    for(string::iterator i = vertex.begin(); i!= vertex.end(); ++i){
        if (!(isalnum(*i) || *i == '[' || *i == ']'|| *i == ';')){
            return false;
        }
    }
    int right = 0;
    int lift = 0;
    for(string::iterator i = vertex.begin(); i!= vertex.end(); ++i){
        if (lift > right){
            return false;
        }
        if(*i == '['){
            ++right;
        }
        if(*i == ']'){
            ++lift;
        }
        if(*i == ';' && lift == right){
            return false;
        }
    }
    return lift == right;
}

void saveGraphToFile(const Graph& graph, string file_name){
    std::ofstream outfile(file_name, std::ios::binary);
    if(!outfile){
        throw exeptions::cannot_open_file(file_name);
    }
    unsigned int num_vertex = graph.vertexes.size();
    unsigned int num_edges = graph.edges.size();
    outfile.write((const char*)&num_vertex, sizeof(unsigned int));
    outfile.write((const char*)&num_edges, sizeof(unsigned int));
    for(set<string>::iterator i = graph.vertexes.begin(); i != graph.vertexes.end();++i){
        string str = *i;
        unsigned int str_size = str.size();
        char* cstr = new char [str_size + 1];
        strcpy(cstr, str.c_str());
        outfile.write((const char*)&str_size, sizeof(unsigned int));
        outfile.write(cstr, str_size);
        delete[] cstr;
    }
    for(set<Edge>::iterator i = graph.edges.begin(); i != graph.edges.end();++i){
        string from = (*i).getFrom();
        unsigned int str_size = from.size();
        char* cstr_from = new char [str_size + 1];
        strcpy(cstr_from, from.c_str());
        outfile.write((const char*)&str_size, sizeof(unsigned int));
        outfile.write(cstr_from, str_size);
        string to = (*i).getTo();
        str_size = to.size();
        char* cstr_to = new char [str_size + 1];
        strcpy(cstr_to, to.c_str());
        outfile.write((const char*)&str_size, sizeof(unsigned int));
        outfile.write(cstr_to, str_size);
        delete[] cstr_from;
        delete[] cstr_to;
    }


}

Graph loadGraphFromFile(string file_name){
    std::ifstream infile (file_name, std::ios::binary);
    if(!infile){
        throw exeptions::cannot_open_file(file_name);
    }
    Graph graph;
    unsigned int num_vertex;
    unsigned int num_edges;
    infile.read((char*)&num_vertex, sizeof(unsigned int));
    infile.read((char*)&num_edges, sizeof(unsigned int));
    for (unsigned int i = 0; i < num_vertex; ++i){
        unsigned int str_size;
        infile.read((char*)&str_size, sizeof(unsigned int));
        char* cstr = new char [str_size + 1];
        infile.read(cstr, str_size);
        graph.addVertex(string(cstr,str_size));
        delete[] cstr;
    }
    for(unsigned int i = 0 ;i < num_edges; ++i){
        unsigned int str_from_size;
        infile.read((char*)&str_from_size, sizeof(unsigned int));
        char* cstr_from = new char [str_from_size + 1];
        infile.read(cstr_from, str_from_size);
        unsigned int str_to_size;
        infile.read((char*)&str_to_size, sizeof(unsigned int));
        char* cstr_to = new char [str_to_size + 1];
        infile.read(cstr_to, str_to_size);
        graph.addEdge(Edge(string(cstr_from, str_from_size),string(cstr_to,str_to_size)));
    }
    return graph;
}

string removeSpaceFromBeginingAndEnd(const string& str){
    string new_string(str);
    if(new_string.size() == 0){
        return new_string;
    }
    for(string :: iterator i = new_string.begin(); (i != new_string.end()) && (*i ==' ');){
            new_string.erase(i);
    }
    for(string :: iterator i = new_string.end() - 1; (i != new_string.begin()) && (*i ==' '); --i){
        new_string.erase(i);
    }
    return new_string;
}
Graph getGraphByName(string graph_name,map<string,Graph>& map_of_graphs){
    string new_name = removeSpaceFromBeginingAndEnd(graph_name);
    if(map_of_graphs.find(new_name) == map_of_graphs.end()){
        throw exeptions::graphNotExist(new_name);
    }
    return Graph(map_of_graphs[new_name]);
}


Graph loadCommand(string command){
    string new_command = removeSpaceFromBeginingAndEnd(command);
    string str = new_command.substr(4);
    str = removeSpaceFromBeginingAndEnd(str);
    if (str.size() == 0 ||(!(str[0] == '(' && str[str.size() - 1] == ')'))){
        throw exeptions::unrecognizedCommand(new_command);
    }
    str = str.substr(1,str.size() - 2);
    str = removeSpaceFromBeginingAndEnd(str);
    return loadGraphFromFile(str);
}

int commaNumberInString(string str){
    int cnt = 0;
    for (unsigned int i = 0; i < str.size(); ++i){
        if(str[i] == ','){
            ++cnt;
        }
    }
    return cnt;
}

Graph literalToGraph(string literal_graph){
    string new_literal_graph = removeSpaceFromBeginingAndEnd(literal_graph);
    if(!(new_literal_graph[0] == '{' && new_literal_graph[new_literal_graph.size()-1] == '}')){
        throw exeptions::syntaxErrorWithLiteralGraph(new_literal_graph);
    }
    string vertexex_string = "";
    string edges_string = "";
    size_t i = new_literal_graph.find('|');
    if(i == string::npos){
        vertexex_string = new_literal_graph.substr(1,new_literal_graph.size() - 2);
    }
    else{
        vertexex_string = new_literal_graph.substr(1,i-1);
        edges_string = new_literal_graph.substr(i + 1);
        edges_string.erase(edges_string.end() - 1);
    }
    Graph graph;
    addLiteralVertexesToGraph(graph,vertexex_string);
    addLiteralEdgesToGraph(graph,edges_string);
    return graph;
}

void addLiteralVertexesToGraph(Graph& graph, string vertexes){
    if(removeSpaceFromBeginingAndEnd(vertexes) == ""){
        return;
    }
    int number_of_commas = commaNumberInString(vertexes);
    string vertexes_literal = removeSpaceFromBeginingAndEnd(vertexes);
    while(number_of_commas >= 0){
        int i = vertexes_literal.find(',');
        string vertex = vertexes_literal.substr(0,i);
        vertex = removeSpaceFromBeginingAndEnd(vertex);
        graph.addVertex(vertex);
        if(number_of_commas > 0){
            vertexes_literal = vertexes_literal.substr(i + 1);
        }
        number_of_commas--;
    }
}


void addLiteralEdgesToGraph(Graph& graph, string edges){
    if(removeSpaceFromBeginingAndEnd(edges) == ""){
        return;
    }
    string edges_literal = removeSpaceFromBeginingAndEnd(edges);
    int flag = 0;
    int cnt = 0;
    for(unsigned int i = 0;i < edges.size(); ++i){
        if(edges[i] == '<'){
            ++cnt;
            flag++;
        }
        if(edges[i] == '>'){
            flag--;
        }
        if(flag < 0 || flag > 1){
            throw exeptions::syntaxErrorWithLiteralGraph(edges_literal);
        }
    }
    if(flag != 0 || cnt == 0){
        throw exeptions::syntaxErrorWithLiteralGraph(edges_literal);
    }
    while(cnt > 0){
        int lift = edges_literal.find('<');
        int right = edges_literal.find('>');
        string str = edges_literal.substr(lift,right - lift + 1);
        str = parserEdge(str);
        edges_literal.replace(lift,right - lift + 1,str);
        --cnt;
    }
    int number_of_comma = commaNumberInString(edges_literal);
    if (number_of_comma % 2 == 0){
        throw exeptions::syntaxErrorWithLiteralGraph(removeSpaceFromBeginingAndEnd(edges));
    }
    while(number_of_comma > 0){
        int i = edges_literal.find(',');
        string from = edges_literal.substr(0,i);
        from = removeSpaceFromBeginingAndEnd(from);
        edges_literal = edges_literal.substr(i + 1);
        i = edges_literal.find(',');
        string to = edges_literal.substr(0,i);
        to = removeSpaceFromBeginingAndEnd(to);
        if(number_of_comma > 1){
            edges_literal = edges_literal.substr(i + 1);
        }
        graph.addEdge(Edge(from,to));
        number_of_comma-=2;
    }

}


string parserEdge(string edge){
    string str = removeSpaceFromBeginingAndEnd(edge);
    str = str.substr(1,str.size() - 2);
    size_t i = str.find(',');
    if(i == string::npos){
        throw exeptions::syntaxErrorWithLiteralGraph(edge);
    }
    string check_string = str.substr(i + 1);
    size_t k = check_string.find(',');
    if(k != string::npos){
        throw exeptions::syntaxErrorWithLiteralGraph(edge);
    }
    return str;
}


string graphToLiteral(Graph graph){
    string literal_graph = "{";
    for(set<string>::iterator i = graph.vertexes.begin(); i != graph.vertexes.end(); ++i){
        literal_graph += *i;
        if( (++i) == graph.vertexes.end()){
           break;
        }
        --i;
        literal_graph += ",";
    }
    literal_graph += "|";
    for(set<Edge>::iterator i = graph.edges.begin(); i != graph.edges.end(); ++i){
        string from = (*i).getFrom();
        string to = (*i).getTo();
        literal_graph += "<";
        literal_graph += from;
        literal_graph += ",";
        literal_graph += to;
        literal_graph += ">";
        if ((++i) == graph.edges.end()){
            break;
        }
        --i;
        literal_graph += ",";
    }
    literal_graph += "}";
    return literal_graph;
}


Graph calculateOnariCommand(string command, map<string,Graph>& map_of_graphs){
    int not_sign_num = 0;
    unsigned int i = 0;
    for(i = 0; i < command.size(); ++i){
        if(command[i] == '!'){
            not_sign_num++;
            continue;
        }
        if(command[i] != ' '){
           break;
        }
    }
    string new_command = command.substr(i);
    new_command = removeSpaceFromBeginingAndEnd(new_command);
    if (new_command.size() == 0){
        throw exeptions::unrecognizedCommand();
    }
    Graph graph;
    if (new_command[0] == '{' || new_command[new_command.size() - 1] == '}'){
        graph = literalToGraph(new_command);
    }
    else if(new_command.substr(0,4) == "load"){
        graph = loadCommand(new_command);
    }
    else {
        graph = getGraphByName(new_command, map_of_graphs);
    }
    if(not_sign_num % 2 == 1){
        return !graph;
    }
    return graph;
}


string replaceLoadCommand(string command, map<string,Graph>& map_op_graphs){
    string new_command = command;
    while(1){
       size_t i = new_command.find("load ");
       if (i == string::npos){
            i = new_command.find("load(");
            if(i == string::npos){
                break;
            }
       }
       size_t j = new_command.find(')',i);
       if(j == string::npos){
            throw exeptions::unrecognizedCommand(new_command.substr(i));
       }
       Graph graph = calculateOnariCommand(new_command.substr(i,j- i + 1), map_op_graphs);
       string literal_graph = graphToLiteral(graph);
       new_command.replace(i, j - i + 1, literal_graph);
    }
    return new_command;
}

Graph calculateBinaryGraph(Graph g1,Graph g2,char op){
    if (op =='+'){
        return g1 + g2;
    }
    if (op =='-'){
        return g1 - g2;
    }
    if (op =='*'){
        return g1 * g2;
    }
    if (op =='^'){
        return g1 ^ g2;
    }
    return g1;
}


string calculateSimpleGraph(string command, map<string,Graph>& map_of_graphs){
    vector<char> vector_operations(command.size(),'a');
    unsigned int op = command.size();
    int cnt = 0;
    for (unsigned int i = 0; i < command.size(); ++i){
        if(command[i] == '+' || command[i] == '-' || command[i] == '*' || command[i] == '^' ){
            vector_operations[i] = command[i];
            if (cnt == 0){
                op = i;
            }
            ++cnt;
        }
    }
    Graph result = calculateOnariCommand(command.substr(0,op),map_of_graphs);
    for (unsigned int i = op + 1; i < command.size(); ++i){
        if(vector_operations[i] != 'a'){
            result = calculateBinaryGraph(result,calculateOnariCommand(command.substr(op + 1,i - op - 1),map_of_graphs),vector_operations[op]);
            op = i;
        }
    }
    if(op != command.size()){
        result = calculateBinaryGraph(result,calculateOnariCommand(command.substr(op + 1),map_of_graphs),vector_operations[op]);
    }
    return graphToLiteral(result);
}



unsigned int bracketsNum(string str, unsigned int& last_lift,unsigned int & last_right){
    unsigned int lift = 0;
    unsigned int right = 0;
    for(unsigned int i = 0; i < str.size(); ++i){
        if(right > lift){
            throw exeptions::unrecognizedCommand();
        }
        if(str[i] == '('){
            last_lift = i;
            lift++;
        }
        if(str[i] == ')'){
            right++;
        }

    }
    if(lift != right){
        throw exeptions::unrecognizedCommand();
    }
    if(lift == 0){
        return 0;
    }
    last_right = str.find(')',last_lift);
    return lift;
}




Graph calculateGraph(string command, map<string,Graph>& map_of_graphs){
    unsigned int lift = 0;
    unsigned int right = 0;
    string new_command = replaceLoadCommand(command,map_of_graphs);
    while(bracketsNum(new_command,lift,right)){
        string simple_command = new_command.substr(lift + 1,right - lift - 1);
        string literal_graph = calculateSimpleGraph(simple_command, map_of_graphs);
        new_command.replace(lift,right - lift + 1, literal_graph);
    }
    return literalToGraph(calculateSimpleGraph(new_command,map_of_graphs));
}

bool graphNameIsValid(string name){
    if(name.size() == 0){
        return false;
    }
    if(name == "load" || name == "save" || name == "print" || name == "who" || name == "delete" || name == "reset" || name == "quit"){
        return false;
    }
    if(!isalpha(name[0])){
        return false;
    }
    for(unsigned int i = 1; i < name.size(); ++i){
        if(!isalnum(name[i])){
            return false;
        }
    }
    return true;
}

void assignmentCommand(string command, map<string,Graph>& map_of_graphs){
    size_t i = command.find('=');
    string graph_name = command.substr(0,i);
    graph_name = removeSpaceFromBeginingAndEnd(graph_name);
    if(!graphNameIsValid(graph_name)){
        throw exeptions::invalidGraphName(graph_name);
    }
    map_of_graphs[graph_name] = calculateGraph(command.substr(i + 1),map_of_graphs);
}

void printGraphsNameInMap(map<string,Graph>& map_of_graphs, ostream& os){
    for(map<string,Graph>::iterator i = map_of_graphs.begin(); i != map_of_graphs.end(); ++i){
        os<<i->first<<endl;
    }
}

void deleteCommand(string command, map<string,Graph>& map_of_graphs){
    string new_command = command.substr(6);
    new_command = removeSpaceFromBeginingAndEnd(new_command);
    if(new_command.size() == 0){
        throw exeptions::generalError("there are no graphs to delete");
    }
    if(!(new_command[0] == '(' && new_command[new_command.size() - 1] == ')')){
        throw exeptions::unrecognizedCommand();
    }
    new_command = new_command.substr(1,new_command.size() - 2);
    new_command = removeSpaceFromBeginingAndEnd(new_command);
    if(new_command.size() == 0){
        throw exeptions::generalError("there are no graphs to delete");
    }
    if(map_of_graphs.find(new_command) == map_of_graphs.end()){
        throw exeptions::graphNotExist(new_command);
    }
    map_of_graphs.erase(new_command);
}

void printCommand(string command, map<string,Graph>& map_of_graphs, ostream& os){
    string new_command = command.substr(5);
    new_command = removeSpaceFromBeginingAndEnd(new_command);
    if(new_command.size() == 0){
        throw exeptions::generalError("there are no graphs to print");
    }
    if(!(new_command[0] == '(' && new_command[new_command.size() - 1] == ')')){
        throw exeptions::unrecognizedCommand();
    }
    new_command = new_command.substr(1,new_command.size() - 2);
    new_command = removeSpaceFromBeginingAndEnd(new_command);
    if(new_command.size() == 0){
        throw exeptions::generalError("there are no graphs to print");
    }
    os<<calculateGraph(new_command, map_of_graphs);
}

void saveCommand(string command, map<string,Graph>& map_of_graphs){
    string new_command = command.substr(4);
    new_command = removeSpaceFromBeginingAndEnd(new_command);
    if(new_command.size() == 0){
        throw exeptions::generalError("there are no graphs to save into file");
    }
    if(!(new_command[0] == '(' && new_command[new_command.size() - 1] == ')')){
        throw exeptions::unrecognizedCommand();
    }
    new_command = new_command.substr(1,new_command.size() - 2);
    new_command = removeSpaceFromBeginingAndEnd(new_command);
    if(new_command.size() == 0){
        throw exeptions::generalError("there is no graph to save into file");
    }
    size_t i = new_command.rfind(',');
    if (i == string::npos){
        throw exeptions::generalError("invalid arguments to save");
    }
    string graph_to_save = new_command.substr(0,i);
    graph_to_save = removeSpaceFromBeginingAndEnd(graph_to_save);
    if(graph_to_save.size() == 0){
        throw exeptions::generalError("there is no graph to save into file");
    }
    string file_name = new_command.substr(i+1);
    file_name = removeSpaceFromBeginingAndEnd(file_name);
    if(file_name.size() == 0){
        throw exeptions::generalError("there are no file name provided to save command ");
    }
    saveGraphToFile(calculateGraph(graph_to_save, map_of_graphs), file_name);
}


bool doCommand(string command, map<string,Graph>& map_of_graphs, ostream& os){
    string new_command = removeSpaceFromBeginingAndEnd(command);
    if(new_command == ""){
        return false;
    }
    if(new_command.find('=') != string ::npos){
        assignmentCommand(new_command, map_of_graphs);
        return false;
    }
    if (new_command == "quit"){
        return true;
    }
    if (new_command == "reset"){
        map_of_graphs.clear();
        return false;
    }
    if(new_command == "who"){
        printGraphsNameInMap(map_of_graphs,os);
        return false;
    }
    if(new_command.find("delete") == 0){
        deleteCommand(new_command, map_of_graphs);
        return false;
    }
    if(new_command.find("print") == 0){
        printCommand(new_command, map_of_graphs, os);
        return false;
    }
    if(new_command.find("save") == 0){
        saveCommand(new_command, map_of_graphs);
        return false;
    }
    throw exeptions::unrecognizedCommand(new_command);
    return false;
}


void calculator(ostream& os, istream& is, string prompt){
    map<string,Graph> map_of_graphs;
    while(!is.eof()){
       string full_command ="";
       try{
        os<<prompt;
        string command;
        std::getline(is,command);
        full_command = command;
        if(doCommand(command, map_of_graphs,os)){
            return;
        }
        }
    catch(exeptions::generalError& e){
        os<<error_string<<e.info<<endl;
	}
	catch(exeptions::invalidGraphName& e){
        os<<error_string<<"invalid graph name: "<<e.info<<endl;
	}
    catch(Graph::badEdge&){
	    os<<error_string<<"error with edge name"<<endl;
	}
	catch(Graph::badVertex&){
	    os<<error_string<<"error with vertex name"<<endl;
	}
	catch(Graph::vertexExist&){
	    os<<error_string<<"you wrote the same vertex name twice"<<endl;
	}
	catch(Graph::edgeExist&){
	    os<<error_string<<"you wrote the same edge name twice"<<endl;
	}
	catch(exeptions::graphNotExist& e){
	    os<<error_string<<"graph not exist : "<<e.info<<endl;
	}
    catch(exeptions::cannot_open_file& e){
        os<<error_string<<"cannot open file  "<<e.info<<endl;
    }
    catch(exeptions::unrecognizedCommand& e){
        if(e.info == ""){
           os<<error_string<<"unrecognized command:  "<<full_command<<endl;
        }
        else{
           os<<error_string<<"unrecognized command:  "<<e.info<<endl;
        }
    }
    catch(exeptions::syntaxErrorWithLiteralGraph& e){
        os<<error_string<<"syntax error with literal graph"<<e.info<<endl;
    }
    catch(std::bad_alloc& e){
        cerr<<"error with memory";
        return;
    }
    catch(...){
        os<<error_string<<"unrecognized command:  "<<full_command<<endl;
    }
    }
}