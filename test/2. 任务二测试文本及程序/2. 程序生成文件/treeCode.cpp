#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
map<string, int> grammarMap = { {"program->stmt-sequence" , 0},{"stmt-sequence->stmt-sequence SEMI statement" , 1},{"stmt-sequence->statement" , 2},{"statement->if-stmt" , 3},{"statement->repeat-stmt" , 4},{"statement->assign-stmt" , 5},{"statement->read-stmt" , 6},{"statement->write-stmt" , 7},{"if-stmt->if exp then stmt-sequence end" , 8},{"if-stmt->if exp then stmt-sequence else stmt-sequence end" , 9},{"repeat-stmt->repeat stmt-sequence until exp" , 10},{"assign-stmt->ID ASSIGN exp" , 11},{"read-stmt->read ID" , 12},{"write-stmt->write exp" , 13},{"exp->simple-exp comparison-op simple-exp" , 14},{"exp->simple-exp" , 15},{"comparison-op->LT" , 16},{"comparison-op->EQ" , 17},{"comparison-op->LTEQ" , 18},{"comparison-op->NE" , 19},{"comparison-op->RTEQ" , 20},{"comparison-op->RT" , 21},{"simple-exp->simple-exp addop term" , 22},{"simple-exp->term" , 23},{"addop->PLUS" , 24},{"addop->MINUS" , 25},{"term->term mulop factor" , 26},{"term->factor" , 27},{"mulop->MULTIPLY" , 28},{"mulop->DIVIDE" , 29},{"mulop->MOD" , 30},{"factor->LPAN exp RPAN" , 31},{"factor->NUMBER" , 32},{"factor->ID" , 33}};
// ����һ���ṹ������ʾÿһ�еļ�ֵ��
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

// ������ȡ���ָ�ÿһ�еļ�ֵ��
vector<KeyValue> readKeyValuePairs(const string& filename) {
    ifstream file(filename);
    vector<KeyValue> keyValuePairs;

    if (!file) {
        return keyValuePairs;
    }

    string line;
    while (getline(file, line)) {
        // ʹ�� stringstream ��ÿһ�зָ��ɼ���ֵ
        stringstream ss(line);
        string key, value;
        getline(ss, key, ':'); // �� ':' �ָ���
        getline(ss, value);    // ��ȡʣ�µ���Ϊֵ

        // ȥ������ֵ���˵Ŀո�
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // ����ֵ����ӵ�������
        keyValuePairs.push_back({ key, value });
    }

    file.close();
    return keyValuePairs;
}

// �����﷨���ڵ�ṹ
struct BTreeNode {
    string kind;
    string value;
    vector<BTreeNode*> nodeList;
    BTreeNode(string kind,string val) :kind(kind), value(val) {}
};

stack<BTreeNode*> treeStack;    // ���ڵ�ջ
stack<KeyValue> strStack; // ����ջ
stack<int> stateStack; // ״̬ջ

// �������ݽṹ
struct SLRUnit
{
    string index;
    map<string, string> m;
};

vector<SLRUnit> SLRVector;

// ���Զ����ʽ���ַ��������� SLRVector
vector<SLRUnit> StringToSLRVector(const string& str)
{
    vector<SLRUnit> vec;
    SLRUnit unit;

    istringstream iss(str); // ����һ��������
    string line;

    int index = 0;

    while (getline(iss, line))
    {
        if (line == "SLRUnit")
        {
            unit = SLRUnit(); // ����һ���µ� SLRUnit
            unit.index = to_string(index);  // ������
        }
        else if (line == "{")
        {
            // ����
        }
        else if (line == "}")
        {
            vec.push_back(unit); // ����ɵ� SLRUnit ��ӵ� vector ��
        }
        else if (line.size() > 0)
        {
            // ������ֵ��
            size_t pos = line.find(": ");
            if (pos != string::npos)
            {
                string key = line.substr(pos + 2); // ��ȡ��������
                getline(iss, line); // ��ȡ��һ�У���һ��Ӧ����ֵ
                pos = line.find(": ");
                if (pos != string::npos)
                {
                    string value = line.substr(pos + 2); // ��ȡֵ������
                    unit.m[key] = value; // ��ӵ���ǰ SLRUnit ��
                }
            }
        }
    }

    return vec;
}


// �� BTreeNode ת��Ϊ�Զ����ʽ���ַ���
string BTreeNodeToString(BTreeNode* node, int depth = 0)
{
    ostringstream oss; // ����һ�������
    string indent(depth * 4, ' '); // ����

    oss << indent << "BTreeNode\n";
    oss << indent << "{\n";
    oss << indent << "    kind: " << node->kind << "\n";
    oss << indent << "    value: " << node->value << "\n";

    if (!node->nodeList.empty())
    {
        oss << indent << "    nodeList:\n";
        for (const auto& child : node->nodeList)
        {
            oss << BTreeNodeToString(child, depth + 1); // �ݹ�ת���ӽڵ�
        }
    }

    oss << indent << "}\n";

    return oss.str(); // ����������е��ַ���
}
// program->stmt-sequence
string fun0() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "program";
}

// stmt-sequence->stmt-sequence SEMI statement
string fun1() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("SEMI", symbol1);

	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	BTreeNode* rootNode = new BTreeNode("-1","stmt-sequence");
 	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode2);
	treeStack.push(rootNode);
	return "stmt-sequence";
}

// stmt-sequence->statement
string fun2() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "stmt-sequence";
}

// statement->if-stmt
string fun3() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->repeat-stmt
string fun4() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->assign-stmt
string fun5() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->read-stmt
string fun6() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// statement->write-stmt
string fun7() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "statement";
}

// if-stmt->if exp then stmt-sequence end
string fun8() {
	string symbol4 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode4 = new BTreeNode("end", symbol4);

	BTreeNode* newNode3 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("then", symbol2);

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("if", symbol0);

	newNode0->nodeList.push_back(newNode1);
	newNode0->nodeList.push_back(newNode3);
	treeStack.push(newNode0);
	return "if-stmt";
}

// if-stmt->if exp then stmt-sequence else stmt-sequence end
string fun9() {
	string symbol6 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode6 = new BTreeNode("end", symbol6);

	BTreeNode* newNode5 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol4 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode4 = new BTreeNode("else", symbol4);

	BTreeNode* newNode3 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("then", symbol2);

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("if", symbol0);

	newNode0->nodeList.push_back(newNode1);
	newNode0->nodeList.push_back(newNode3);
	newNode0->nodeList.push_back(newNode5);
	treeStack.push(newNode0);
	return "if-stmt";
}

// repeat-stmt->repeat stmt-sequence until exp
string fun10() {
	BTreeNode* newNode3 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("until", symbol2);

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("repeat", symbol0);

	newNode0->nodeList.push_back(newNode1);
	newNode0->nodeList.push_back(newNode3);
	treeStack.push(newNode0);
	return "repeat-stmt";
}

// assign-stmt->ID ASSIGN exp
string fun11() {
	BTreeNode* newNode2 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ASSIGN", symbol1);

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("ID", symbol0);

	newNode1->nodeList.push_back(newNode0);
	newNode1->nodeList.push_back(newNode2);
	treeStack.push(newNode1);
	return "assign-stmt";
}

// read-stmt->read ID
string fun12() {
	string symbol1 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode1 = new BTreeNode("ID", symbol1);

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("read", symbol0);

	newNode0->nodeList.push_back(newNode1);
	treeStack.push(newNode0);
	return "read-stmt";
}

// write-stmt->write exp
string fun13() {
	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("write", symbol0);

	newNode0->nodeList.push_back(newNode1);
	treeStack.push(newNode0);
	return "write-stmt";
}

// exp->simple-exp comparison-op simple-exp
string fun14() {
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
	return "exp";
}

// exp->simple-exp
string fun15() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "exp";
}

// comparison-op->LT
string fun16() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("LT", symbol0);

	treeStack.push(newNode0);
	return "comparison-op";
}

// comparison-op->EQ
string fun17() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("EQ", symbol0);

	treeStack.push(newNode0);
	return "comparison-op";
}

// comparison-op->LTEQ
string fun18() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("LTEQ", symbol0);

	treeStack.push(newNode0);
	return "comparison-op";
}

// comparison-op->NE
string fun19() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("NE", symbol0);

	treeStack.push(newNode0);
	return "comparison-op";
}

// comparison-op->RTEQ
string fun20() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("RTEQ", symbol0);

	treeStack.push(newNode0);
	return "comparison-op";
}

// comparison-op->RT
string fun21() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("RT", symbol0);

	treeStack.push(newNode0);
	return "comparison-op";
}

// simple-exp->simple-exp addop term
string fun22() {
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

	BTreeNode* rootNode = new BTreeNode("-1","simple-exp");
 	rootNode->nodeList.push_back(newNode1);
	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode2);
	treeStack.push(rootNode);
	return "simple-exp";
}

// simple-exp->term
string fun23() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "simple-exp";
}

// addop->PLUS
string fun24() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("PLUS", symbol0);

	treeStack.push(newNode0);
	return "addop";
}

// addop->MINUS
string fun25() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("MINUS", symbol0);

	treeStack.push(newNode0);
	return "addop";
}

// term->term mulop factor
string fun26() {
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
 	rootNode->nodeList.push_back(newNode1);
	rootNode->nodeList.push_back(newNode0);
	rootNode->nodeList.push_back(newNode2);
	treeStack.push(rootNode);
	return "term";
}

// term->factor
string fun27() {
	BTreeNode* newNode0 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	treeStack.push(newNode0);
	return "term";
}

// mulop->MULTIPLY
string fun28() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("MULTIPLY", symbol0);

	treeStack.push(newNode0);
	return "mulop";
}

// mulop->DIVIDE
string fun29() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("DIVIDE", symbol0);

	treeStack.push(newNode0);
	return "mulop";
}

// mulop->MOD
string fun30() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("MOD", symbol0);

	treeStack.push(newNode0);
	return "mulop";
}

// factor->LPAN exp RPAN
string fun31() {
	string symbol2 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode2 = new BTreeNode("RPAN", symbol2);

	BTreeNode* newNode1 = treeStack.top();
    strStack.pop();
    stateStack.pop();
    treeStack.pop();

	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("LPAN", symbol0);

	treeStack.push(newNode1);
	return "factor";
}

// factor->NUMBER
string fun32() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("NUMBER", symbol0);

	treeStack.push(newNode0);
	return "factor";
}

// factor->ID
string fun33() {
	string symbol0 = strStack.top().value;
    strStack.pop();
    stateStack.pop();
	BTreeNode* newNode0 = new BTreeNode("ID", symbol0);

	treeStack.push(newNode0);
	return "factor";
}

// ����һ���洢����ָ�������
string (*funcArray[])() = { fun0,fun1,fun2,fun3,fun4,fun5,fun6,fun7,fun8,fun9,fun10,fun11,fun12,fun13,fun14,fun15,fun16,fun17,fun18,fun19,fun20,fun21,fun22,fun23,fun24,fun25,fun26,fun27,fun28,fun29,fun30,fun31,fun32,fun33 };

void process(const KeyValue& line) {
    string key = line.key;
    string value = line.value;

    // �õ���ǰջ��״̬
    int state = stateStack.top();

    

    // �ҵ���һ��״̬�ַ���
    string nextStateStr = SLRVector[state].m[key == "EOF" ? "$" : key];
    int nextState;
    switch (nextStateStr[0]) {
    case 's':   // ��һ��
        // �����ַ�ջ
        strStack.push(line);
        nextState = stoi(nextStateStr.substr(1));
        stateStack.push(nextState);
        break;
    case 'r':   // Ҫ��Լ��
    {
        string res;
        size_t startPos = nextStateStr.find("("); // ���������ŵ�λ��
		size_t endPos = nextStateStr.find(")");   // ���������ŵ�λ��

        // ����ҵ�����������
        if (startPos != string::npos && endPos != string::npos) {
            startPos++; // �������ŵ���һ��λ�ÿ�ʼ��ȡ����
            res = nextStateStr.substr(startPos, endPos - startPos);
        }
        int i = grammarMap[res];
        string left = funcArray[i]();   // ���ö�Ӧ�ĺ���
        state = stateStack.top();
        nextStateStr = SLRVector[state].m[left];
        nextState = stoi(nextStateStr);
        stateStack.push(nextState);
        strStack.push(KeyValue(left));
        // �������뵱ǰ�ַ�
        process(line);
        break;
    }
    case 'A':   // ACCEPT
        // �����﷨��
        cout << "�ɹ���";
        break;
    default:
        cout << "״̬�����";
        
    }

}

int main() {

    ifstream file("F:/��������/ѧϰ/���������/2024 ����ԭ��γ���Ŀ/�Ͻ�����/2. �����ļ���/2. ����������ı�������/2. ���������ļ�/SLR1Str.txt");
    stringstream buffer;
    buffer << file.rdbuf();
    string SLR1Str = buffer.str();

    SLRVector = StringToSLRVector(SLR1Str);

    // ��ʼ״̬Ϊ0
    int state = 0;
    stateStack.push(state);
    vector<KeyValue> keyValuePairs = readKeyValuePairs("F:/��������/ѧϰ/���������/2024 ����ԭ��γ���Ŀ/�Ͻ�����/2. �����ļ���/2. ����������ı�������/2. ���������ļ�/output.lex");
    for (const auto& pair : keyValuePairs) {
        process(pair);
    }
    
    string str = BTreeNodeToString(treeStack.top());
    // ���Զ����ʽ���ַ�������Ϊ BTreeNode
    istringstream iss(str);
    ofstream outFile("F:/��������/ѧϰ/���������/2024 ����ԭ��γ���Ŀ/�Ͻ�����/2. �����ļ���/2. ����������ı�������/2. ���������ļ�/tree.out");
    if (outFile.is_open())
    {
        outFile << str;
        outFile.close();
    }

    return 0;
}