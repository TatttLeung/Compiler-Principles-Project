#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
map<string, int> grammarMap = { {"program->definition-list" , 0},{"definition-list->definition-list definition" , 1},{"definition-list->definition" , 2},{"definition->variable-definition" , 3},{"definition->function-definition" , 4},{"variable-definition->type-indicator ID SEMI" , 5},{"variable-definition->type-indicator ID LMM NUMBER RMM SEMI" , 6},{"type-indicator->int" , 7},{"type-indicator->float" , 8},{"type-indicator->double" , 9},{"type-indicator->void" , 10},{"function-definition->type-indicator ID LLM parameters RLM compound-stmt" , 11},{"parameters->parameter-list" , 12},{"parameters->void" , 13},{"parameter-list->parameter-list DOU parameter" , 14},{"parameter-list->parameter" , 15},{"parameter->type-indicator ID" , 16},{"parameter->type-indicator ID LMM RMM" , 17},{"compound-stmt->LBM local-definitions statement-list RBM" , 18},{"local-definitions->local-definitions variable-definition" , 19},{"local-definitions->@" , 20},{"statement-list->statement-list statement" , 21},{"statement-list->@" , 22},{"statement->expression-stmt" , 23},{"statement->compound-stmt" , 24},{"statement->condition-stmt" , 25},{"statement->dowhile-stmt" , 26},{"statement->return-stmt" , 27},{"expression-stmt->expression SEMI" , 28},{"expression-stmt->SEMI" , 29},{"condition-stmt->if LLM expression RLM statement" , 30},{"condition-stmt->if LLM expression RLM statement else statement" , 31},{"dowhile-stmt->do statement while LLM expression RLM SEMI" , 32},{"return-stmt->return SEMI" , 33},{"return-stmt->return expression SEMI" , 34},{"expression->variable ASSIGN expression" , 35},{"expression->simple-expression" , 36},{"variable->ID" , 37},{"variable->ID LMM expression RMM" , 38},{"simple-expression->additive-expression relop additive-expression" , 39},{"simple-expression->additive-expression" , 40},{"relop->LTEQ" , 41},{"relop->LT" , 42},{"relop->RT" , 43},{"relop->RTEQ" , 44},{"relop->EQ" , 45},{"relop->NE" , 46},{"additive-expression->additive-expression addop term" , 47},{"additive-expression->term" , 48},{"addop->PLUS" , 49},{"addop->MINUS" , 50},{"term->term mulop factor" , 51},{"term->factor" , 52},{"mulop->MULTIPLY" , 53},{"mulop->DIVIDE" , 54},{"mulop->MOD" , 55},{"factor->LLM expression RLM" , 56},{"factor->variable" , 57},{"factor->call" , 58},{"factor->NUMBER" , 59},{"call->ID LLM arguments RLM" , 60},{"arguments->argument-list" , 61},{"arguments->@" , 62},{"argument-list->argument-list DOU expression" , 63},{"argument-list->expression" , 64}};
// 定义一个结构体来表示每一行的键值对
struct KeyValue {
    string key;
    string value;
    KeyValue() {}
    KeyValue(string _key) {
        key = _key;
    }
    KeyValue(string _key, string _value) {
        key = _key;
        value = _value;
    }
};

// 函数读取并分隔每一行的键值对
vector<KeyValue> readKeyValuePairs(const string& filename) {
    ifstream file(filename);
    vector<KeyValue> keyValuePairs;

    if (!file) {
        return keyValuePairs;
    }

    string line;
    while (getline(file, line)) {
        // 使用 stringstream 将每一行分隔成键和值
        stringstream ss(line);
        string key, value;
        getline(ss, key, ':'); // 以 ':' 分隔键
        getline(ss, value);    // 获取剩下的作为值

        // 去除键和值两端的空格
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // 将键值对添加到向量中
        keyValuePairs.push_back({ key, value });
    }

    file.close();
    return keyValuePairs;
}

// 定义语法树节点结构
struct BTreeNode {
    string kind;
    string value;
    vector<BTreeNode*> nodeList;
    BTreeNode(string kind,string val) :kind(kind), value(val) {}
};

stack<BTreeNode*> treeStack;    // 树节点栈
stack<KeyValue> strStack; // 分析栈
stack<int> stateStack; // 状态栈

// 定义数据结构
struct SLRUnit
{
    string index;
    map<string, string> m;
};

vector<SLRUnit> SLRVector;

// 将自定义格式的字符串解析成 SLRVector
vector<SLRUnit> StringToSLRVector(const string& str)
{
    vector<SLRUnit> vec;
    SLRUnit unit;

    istringstream iss(str); // 创建一个输入流
    string line;

    int index = 0;

    while (getline(iss, line))
    {
        if (line == "SLRUnit")
        {
            unit = SLRUnit(); // 创建一个新的 SLRUnit
            unit.index = to_string(index);  // 标记序号
        }
        else if (line == "{")
        {
            // 忽略
        }
        else if (line == "}")
        {
            vec.push_back(unit); // 将完成的 SLRUnit 添加到 vector 中
        }
        else if (line.size() > 0)
        {
            // 解析键值对
            size_t pos = line.find(": ");
            if (pos != string::npos)
            {
                string key = line.substr(pos + 2); // 获取键的内容
                getline(iss, line); // 读取下一行，这一行应该是值
                pos = line.find(": ");
                if (pos != string::npos)
                {
                    string value = line.substr(pos + 2); // 获取值的内容
                    unit.m[key] = value; // 添加到当前 SLRUnit 中
                }
            }
        }
    }

    return vec;
}


// 将 BTreeNode 转换为自定义格式的字符串
string BTreeNodeToString(BTreeNode* node, int depth = 0)
{
    ostringstream oss; // 创建一个输出流
    string indent(depth * 4, ' '); // 缩进

    oss << indent << "BTreeNode\n";
    oss << indent << "{\n";
    oss << indent << "    kind: " << node->kind << "\n";
    oss << indent << "    value: " << node->value << "\n";

    if (!node->nodeList.empty())
    {
        oss << indent << "    nodeList:\n";
        for (const auto& child : node->nodeList)
        {
            oss << BTreeNodeToString(child, depth + 1); // 递归转换子节点
        }
    }

    oss << indent << "}\n";

    return oss.str(); // 返回输出流中的字符串
}
// program->definition-list
string fun0() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "program";
}

// definition-list->definition-list definition
string fun1() {
	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","definition-list");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode1);
	treeStack.push(rootNode);
	return "definition-list";
}

// definition-list->definition
string fun2() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "definition-list";
}

// definition->variable-definition
string fun3() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "definition";
}

// definition->function-definition
string fun4() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "definition";
}

// variable-definition->type-indicator ID SEMI
string fun5() {
	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("SEMI", symbol2);

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ID", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	newNode1->nodeList.push_back(newNode0);
	treeStack.push(newNode1);
	return "variable-definition";
}

// variable-definition->type-indicator ID LMM NUMBER RMM SEMI
string fun6() {
	string symbol5 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode5 = new BTreeNode("SEMI", symbol5);

	string symbol4 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode4 = new BTreeNode("RMM", symbol4);

	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("NUMBER", symbol3);

	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("LMM", symbol2);

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ID", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	newNode1->nodeList.push_back(newNode0);
	newNode1->nodeList.push_back(newNode3);
	treeStack.push(newNode1);
	return "variable-definition";
}

// type-indicator->int
string fun7() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("int", symbol0);

	treeStack.push(newNode0);
	return "type-indicator";
}

// type-indicator->float
string fun8() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("float", symbol0);

	treeStack.push(newNode0);
	return "type-indicator";
}

// type-indicator->double
string fun9() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("double", symbol0);

	treeStack.push(newNode0);
	return "type-indicator";
}

// type-indicator->void
string fun10() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("void", symbol0);

	treeStack.push(newNode0);
	return "type-indicator";
}

// function-definition->type-indicator ID LLM parameters RLM compound-stmt
string fun11() {
	BTreeNode* newNode5 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol4 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode4 = new BTreeNode("RLM", symbol4);

	BTreeNode* newNode3 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("LLM", symbol2);

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ID", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	newNode1->nodeList.push_back(newNode0);
	newNode1->nodeList.push_back(newNode3);
	newNode1->nodeList.push_back(newNode5);
	treeStack.push(newNode1);
	return "function-definition";
}

// parameters->parameter-list
string fun12() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "parameters";
}

// parameters->void
string fun13() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("void", symbol0);

	treeStack.push(newNode0);
	return "parameters";
}

// parameter-list->parameter-list DOU parameter
string fun14() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("DOU", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","parameter-list");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode2);
	treeStack.push(rootNode);
	return "parameter-list";
}

// parameter-list->parameter
string fun15() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "parameter-list";
}

// parameter->type-indicator ID
string fun16() {
	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ID", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	newNode1->nodeList.push_back(newNode0);
	treeStack.push(newNode1);
	return "parameter";
}

// parameter->type-indicator ID LMM RMM
string fun17() {
	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("RMM", symbol3);

	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("LMM", symbol2);

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ID", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	newNode1->nodeList.push_back(newNode0);
	treeStack.push(newNode1);
	return "parameter";
}

// compound-stmt->LBM local-definitions statement-list RBM
string fun18() {
	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("RBM", symbol3);

	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("LBM", symbol0);

	newNode1->nodeList.push_back(newNode2);
	treeStack.push(newNode1);
	return "compound-stmt";
}

// local-definitions->local-definitions variable-definition
string fun19() {
	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","local-definitions");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode1);
	treeStack.push(rootNode);
	return "local-definitions";
}

// local-definitions->@
string fun20() {

	BTreeNode* rootNode = new BTreeNode("-2","local-definitions");
 	treeStack.push(rootNode);
	return "local-definitions";
}

// statement-list->statement-list statement
string fun21() {
	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","statement-list");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode1);
	treeStack.push(rootNode);
	return "statement-list";
}

// statement-list->@
string fun22() {

	BTreeNode* rootNode = new BTreeNode("-2","statement-list");
 	treeStack.push(rootNode);
	return "statement-list";
}

// statement->expression-stmt
string fun23() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->compound-stmt
string fun24() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->condition-stmt
string fun25() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->dowhile-stmt
string fun26() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->return-stmt
string fun27() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// expression-stmt->expression SEMI
string fun28() {
	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("SEMI", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "expression-stmt";
}

// expression-stmt->SEMI
string fun29() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("SEMI", symbol0);

	treeStack.push(newNode0);
	return "expression-stmt";
}

// condition-stmt->if LLM expression RLM statement
string fun30() {
	BTreeNode* newNode4 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("RLM", symbol3);

	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("LLM", symbol1);

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("if", symbol0);

	newNode0->nodeList.push_back(newNode2);
	newNode0->nodeList.push_back(newNode4);
	treeStack.push(newNode0);
	return "condition-stmt";
}

// condition-stmt->if LLM expression RLM statement else statement
string fun31() {
	BTreeNode* newNode6 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol5 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode5 = new BTreeNode("else", symbol5);

	BTreeNode* newNode4 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("RLM", symbol3);

	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("LLM", symbol1);

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("if", symbol0);

	newNode0->nodeList.push_back(newNode2);
	newNode0->nodeList.push_back(newNode4);
	newNode0->nodeList.push_back(newNode6);
	treeStack.push(newNode0);
	return "condition-stmt";
}

// dowhile-stmt->do statement while LLM expression RLM SEMI
string fun32() {
	string symbol6 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode6 = new BTreeNode("SEMI", symbol6);

	string symbol5 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode5 = new BTreeNode("RLM", symbol5);

	BTreeNode* newNode4 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("LLM", symbol3);

	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("while", symbol2);

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("do", symbol0);

	newNode0->nodeList.push_back(newNode1);
	newNode0->nodeList.push_back(newNode4);
	treeStack.push(newNode0);
	return "dowhile-stmt";
}

// return-stmt->return SEMI
string fun33() {
	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("SEMI", symbol1);

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("return", symbol0);

	treeStack.push(newNode0);
	return "return-stmt";
}

// return-stmt->return expression SEMI
string fun34() {
	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("SEMI", symbol2);

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("return", symbol0);

	newNode0->nodeList.push_back(newNode1);
	treeStack.push(newNode0);
	return "return-stmt";
}

// expression->variable ASSIGN expression
string fun35() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ASSIGN", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	newNode1->nodeList.push_back(newNode0);
	newNode1->nodeList.push_back(newNode2);
	treeStack.push(newNode1);
	return "expression";
}

// expression->simple-expression
string fun36() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "expression";
}

// variable->ID
string fun37() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("ID", symbol0);

	treeStack.push(newNode0);
	return "variable";
}

// variable->ID LMM expression RMM
string fun38() {
	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("RMM", symbol3);

	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("LMM", symbol1);

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("ID", symbol0);

	newNode0->nodeList.push_back(newNode2);
	treeStack.push(newNode0);
	return "variable";
}

// simple-expression->additive-expression relop additive-expression
string fun39() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	newNode1->nodeList.push_back(newNode0);
	newNode1->nodeList.push_back(newNode2);
	treeStack.push(newNode1);
	return "simple-expression";
}

// simple-expression->additive-expression
string fun40() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "simple-expression";
}

// relop->LTEQ
string fun41() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("LTEQ", symbol0);

	treeStack.push(newNode0);
	return "relop";
}

// relop->LT
string fun42() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("LT", symbol0);

	treeStack.push(newNode0);
	return "relop";
}

// relop->RT
string fun43() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("RT", symbol0);

	treeStack.push(newNode0);
	return "relop";
}

// relop->RTEQ
string fun44() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("RTEQ", symbol0);

	treeStack.push(newNode0);
	return "relop";
}

// relop->EQ
string fun45() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("EQ", symbol0);

	treeStack.push(newNode0);
	return "relop";
}

// relop->NE
string fun46() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("NE", symbol0);

	treeStack.push(newNode0);
	return "relop";
}

// additive-expression->additive-expression addop term
string fun47() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","additive-expression");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode1);
	rootNode->nodeList.push_back(newNode2);
	treeStack.push(rootNode);
	return "additive-expression";
}

// additive-expression->term
string fun48() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "additive-expression";
}

// addop->PLUS
string fun49() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("PLUS", symbol0);

	treeStack.push(newNode0);
	return "addop";
}

// addop->MINUS
string fun50() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("MINUS", symbol0);

	treeStack.push(newNode0);
	return "addop";
}

// term->term mulop factor
string fun51() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","term");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode1);
	rootNode->nodeList.push_back(newNode2);
	treeStack.push(rootNode);
	return "term";
}

// term->factor
string fun52() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "term";
}

// mulop->MULTIPLY
string fun53() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("MULTIPLY", symbol0);

	treeStack.push(newNode0);
	return "mulop";
}

// mulop->DIVIDE
string fun54() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("DIVIDE", symbol0);

	treeStack.push(newNode0);
	return "mulop";
}

// mulop->MOD
string fun55() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("MOD", symbol0);

	treeStack.push(newNode0);
	return "mulop";
}

// factor->LLM expression RLM
string fun56() {
	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("RLM", symbol2);

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("LLM", symbol0);

	treeStack.push(newNode1);
	return "factor";
}

// factor->variable
string fun57() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "factor";
}

// factor->call
string fun58() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "factor";
}

// factor->NUMBER
string fun59() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("NUMBER", symbol0);

	treeStack.push(newNode0);
	return "factor";
}

// call->ID LLM arguments RLM
string fun60() {
	string symbol3 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode3 = new BTreeNode("RLM", symbol3);

	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("LLM", symbol1);

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("ID", symbol0);

	newNode0->nodeList.push_back(newNode2);
	treeStack.push(newNode0);
	return "call";
}

// arguments->argument-list
string fun61() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "arguments";
}

// arguments->@
string fun62() {

	BTreeNode* rootNode = new BTreeNode("-2","arguments");
 	treeStack.push(rootNode);
	return "arguments";
}

// argument-list->argument-list DOU expression
string fun63() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("DOU", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","argument-list");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode2);
	treeStack.push(rootNode);
	return "argument-list";
}

// argument-list->expression
string fun64() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "argument-list";
}

// 定义一个存储函数指针的数组
string (*funcArray[])() = { fun0,fun1,fun2,fun3,fun4,fun5,fun6,fun7,fun8,fun9,fun10,fun11,fun12,fun13,fun14,fun15,fun16,fun17,fun18,fun19,fun20,fun21,fun22,fun23,fun24,fun25,fun26,fun27,fun28,fun29,fun30,fun31,fun32,fun33,fun34,fun35,fun36,fun37,fun38,fun39,fun40,fun41,fun42,fun43,fun44,fun45,fun46,fun47,fun48,fun49,fun50,fun51,fun52,fun53,fun54,fun55,fun56,fun57,fun58,fun59,fun60,fun61,fun62,fun63,fun64 };

void process(const KeyValue& line) {
    string key = line.key;
    string value = line.value;

    // 拿到当前栈的状态
    int state = stateStack.top();

    

    // 找到下一个状态字符串
    string nextStateStr = SLRVector[state].m[key == "EOF" ? "$" : key];
    int nextState;
    switch (nextStateStr[0]) {
    case 's':   // 下一步
        // 放入字符栈
        strStack.push(line);
        nextState = stoi(nextStateStr.substr(1));
        stateStack.push(nextState);
        break;
    case 'r':   // 要规约了
    {
        string res;
        size_t startPos = nextStateStr.find("("); // 查找左括号的位置
		size_t endPos = nextStateStr.find(")");   // 查找右括号的位置

        // 如果找到了左右括号
        if (startPos != string::npos && endPos != string::npos) {
            startPos++; // 从左括号的下一个位置开始提取内容
            res = nextStateStr.substr(startPos, endPos - startPos);
        }
        int i = grammarMap[res];
        string left = funcArray[i]();   // 调用对应的函数
        state = stateStack.top();
        nextStateStr = SLRVector[state].m[left];
        nextState = stoi(nextStateStr);
        stateStack.push(nextState);
        strStack.push(KeyValue(left));
        // 继续放入当前字符
        process(line);
        break;
    }
    case 'A':   // ACCEPT
        // 生成语法树
        cout << "成功！";
        break;
    default:
        cout << "状态表出错！";
        
    }

}

int main() {

    ifstream file("F:/个人资料/学习/计算机大三/2024 编译原理课程项目/上交材料/2. 测试文件夹/3. 任务三测试文本及程序/项目任务2/2. 测试生成的程序/SLR1Str.txt");
    stringstream buffer;
    buffer << file.rdbuf();
    string SLR1Str = buffer.str();

    SLRVector = StringToSLRVector(SLR1Str);

    // 初始状态为0
    int state = 0;
    stateStack.push(state);
    vector<KeyValue> keyValuePairs = readKeyValuePairs("F:/个人资料/学习/计算机大三/2024 编译原理课程项目/上交材料/2. 测试文件夹/3. 任务三测试文本及程序/项目任务2/2. 测试生成的程序/output.lex");
    for (const auto& pair : keyValuePairs) {
        process(pair);
    }
    
    string str = BTreeNodeToString(treeStack.top());
    // 将自定义格式的字符串解析为 BTreeNode
    istringstream iss(str);
    ofstream outFile("F:/个人资料/学习/计算机大三/2024 编译原理课程项目/上交材料/2. 测试文件夹/3. 任务三测试文本及程序/项目任务2/2. 测试生成的程序/tree.out");
    if (outFile.is_open())
    {
        outFile << str;
        outFile.close();
    }

    return 0;
}