/****************************************************
 * @Copyright © 2023-2024 LDL. All rights reserved.
 *
 * @FileName: widget.cpp
 * @Brief: 主程序
 * @Module Function:
 *
 * @Current Version: 1.0
 * @Author: Lidaliang
 * @Modifier: Lidaliang
 * @Finished Date: 2024/3/21
 *
 * @Version History: 1.0 初始化              
 *
 ****************************************************/
#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <unordered_map>
#include <queue>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#pragma execution_character_set("utf-8")
using namespace std;

// EPSILON定义
const char EPSILON = '#';

// 下面map防止字符冲突
// 符号->字符串map
map<char, string> m1 = { {(char)1,"num"}, {(char)2, "letter"},{(char)3, "\\+\\"},{(char)4, "\\|\\"},{(char)5, "\\(\\"},{(char)6, "\\)\\"},{(char)7, "\\*\\"},{(char)16, "\\?\\"},{(char)17, "\\[\\"},{(char)18, "\\]\\"},{ (char)19,"\\~\\"}, {(char)20,"\\n"}};
// 字符串->符号map
map<string, char> m2 = { {"num",(char)1}, {"letter", (char)2 },{"\\+\\", (char)3 },{"\\|\\", (char)4 },{ "\\(\\", (char)5},{ "\\)\\" ,(char)6},{"\\*\\" ,(char)7},{ "\\?\\",(char)16},{"\\[\\", (char)17},{"\\]\\", (char)18 }, {"\\~\\", (char)19},{"\\n",(char)20} };

// 正则表达式行合集
QString regexLine[5];
// 生成NFA和DFA的正则表达式
string finalRegex;

// 关键词合集
set<string> keyWords;
// 操作符号map
map<string,string> opMap;

// 注释符号集合，0为开始符号，1为结束符号
string commentSymbol[2];

// 全局结点计数器
int nodeCount = 0;

// 是否忽略大小写（默认不忽略）
bool isLowerCase = false;

// 全局字符统计
set<char> nfaCharSet;
set<char> dfaCharSet;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

/*
* @brief 模拟trim函数
* 用于结果展示
*/
string trim(const string& str)
{
    if (str.empty() || str == "\\n")
    {
        return str;
    }
    string ret(str);
    ret.erase(0, ret.find_first_not_of("\\"));
    ret.erase(ret.find_last_not_of("\\") + 1);
    return ret;
}

/*
* @brief set转string
* 用于结果展示
*/
string set2string(set<int> s)
{
    string result;

    for (int i : s) {
        result.append(to_string(i));
        result.append(",");
    }

    if (result.size() != 0)
        result.pop_back(); //弹出最后一个逗号

    return result;
}

/*
* @brief 获取关键词列表
*/
void getKeyWords(QString regex) {
    string r = regex.toStdString();
    QStringList t = regex.split("|");
    for (QString s : t) {
        keyWords.insert(s.toStdString());
    }
}

/*
* @brief 获取操作符号的名称
*/
string getOpName(QString regex1, QString regex2) {
    QStringList op = regex1.split("|");
    QStringList opName = regex2.split("|");
    if (op.size() != opName.size()) {
        return "操作符和操作符名称个数不一致！";
    }
    for (int i = 0; i < op.size(); i++) {
        opMap[op[i].toStdString()] = opName[i].toStdString();
    }
    return "";
}

/*
* @brief 获取注释符号
*/
string getCommentSymbol(QString& regex) {
    QStringList t = regex.split("~");
    if (t.size() != 2) return "注释输入格式错误";
    commentSymbol[0] = t[0].toStdString();
    commentSymbol[1] = t[1].toStdString();
    regex = t[0] + "~*" + t[1];
    return "";
}

/*
* @brief 判断是不是字符
* 由于前面已经对出现在下面的字符进行了希腊字母转换，所以一定不会出现下面的字符
*/
bool isChar(char c)
{
    if (!(c == '+' || c == '[' || c == ']' || c == '|' || c == '*' || c == '(' || c == ')' || c == '?' || c == '@'))
        return true;
    return false;
}

/*
* @brief 进一步处理正则表达式
* 处理[] + 添加连接符
*/
QString afterHandleRegex(QString regex)
{
    string regexStd = regex.toStdString();
    qDebug() << "enter handleRegex: " << regex;

    // 处理中括号
    string result;
    bool insideBrackets = false;
    string currentString;

    for (char c : regexStd) {
        if (c == ' ') continue;
        if (c == '[') {
            insideBrackets = true;
            currentString.push_back('(');
        }
        else if (c == ']') {
            insideBrackets = false;
            currentString.push_back(')');
            result += currentString;
            currentString.clear();
        }
        else if (insideBrackets) {
            if (currentString.length() > 1) {
                currentString.push_back('|');
            }
            currentString.push_back(c);
        }
        else {
            result.push_back(c);
        }
    }

    regexStd = result;

    //先处理+号
    for (int i = 0; i < regexStd.size(); i++)
    {
        if (regexStd[i] == '+')
        {
            int kcount = 0;
            int j = i;
            do
            {
                j--;
                if (regexStd[j] == ')')
                {
                    kcount++;
                }
                else if (regexStd[j] == '(')
                {
                    kcount--;
                }
            } while (kcount != 0);
            string str1 = regexStd.substr(0, j);
            string kstr = regexStd.substr(j, i - j);
            string str2 = regexStd.substr(i + 1, (regexStd.size() - i));
            regexStd = str1 + kstr + kstr + "*" + str2;
        }
    }

    for (int i = 0; i < regexStd.size() - 1; i++)
    {
        if (isChar(regexStd[i]) && isChar(regexStd[i + 1])
            || isChar(regexStd[i]) && regexStd[i + 1] == '('
            || regexStd[i] == ')' && isChar(regexStd[i + 1])
            || regexStd[i] == ')' && regexStd[i + 1] == '('
            || regexStd[i] == '*' && regexStd[i + 1] != ')' && regexStd[i + 1] != '|' && regexStd[i + 1] != '?'
            || regexStd[i] == '?' && regexStd[i + 1] != ')'
            || regexStd[i] == '+' && regexStd[i + 1] != ')')
        {
            string str1 = regexStd.substr(0, i + 1);
            string str2 = regexStd.substr(i + 1, (regexStd.size() - i));
            str1 += "@";
            regexStd = str1 + str2;
        }

    }
    return QString::fromStdString(regexStd);
}

/*
* @brief 对正则表达式进行处理
* 格式：
* 第一行：keyword，多个用 | 进行拼接成正则表达式
* 第二行：除了注释符号的专用符号的正则表达式，用|连接，如果与正则表达式符号冲突（包括：+、|、（、）、*、？、[、]、~），用\进行包围，如：\+\
* 第三行：第二行符号对应的名称，用|分隔
* 第四行：标识符的正则表达式，可以用\letter\表示所有字母，\num\表示所有数字
* 第五行：数字的正则表达式
* 第六行：注释的正则表达式，用~表示除结束符号的任意字符，如{~}

*/
string handleAllRegex(QString allRegex, bool isLowerCase) {

    // 不区分大小写的话，全部转为小写
    if (isLowerCase) {
        allRegex = allRegex.toLower();
    }
    // 去除空格
    allRegex.replace(" ","");
    // 使用map映射关系替换正则表达式中的符号
    for (const auto& pair : m2) {
        allRegex.replace(QString::fromStdString(pair.first), QString(pair.second));
    }

    // 将文本内容按行分割成一个字符串列表
    QStringList lines = allRegex.split("\n");

    if (lines.size() != 6) {
        return "输入的正则表达式不符合规范，不为6行，请检查！";
    }
    getKeyWords(lines[0]);  // 获取关键词
    string res = getCommentSymbol(lines[5]);  // 获取注释开始符号和结束符号后
    res = getOpName(lines[1], lines[2]);


    // 拼接生成NFA和DFA图的正则表达式
    QString output;
    for (int i = 1; i < 6; ++i) {
        if (i == 2) continue;   // 第三行不做处理
        output += "(" + lines[i] + ")";
        if (i < 5) {
            output += "|";
        }
    }
    finalRegex = output.toStdString();
    qDebug() <<("拼接后的正则表达式：") << QString::fromStdString(finalRegex);

    finalRegex = afterHandleRegex(output).toStdString();
    qDebug() << ("处理后的正则表达式：") << QString::fromStdString(finalRegex);

    return res;
}

/*============================正则表达式转NFA==================================*/

struct nfaNode; // 声明一下，不然报错

/*
* @brief 结构体，NFA图的边
*/
struct nfaEdge
{
    char c;
    nfaNode* next;
};
/*
* @brief 结构体，NFA图的结点
*/
struct nfaNode
{
    int id; // 结点唯一编号
    bool isStart;   // 初态标识
    bool isEnd; // 终态标识
    vector<nfaEdge> edges;  // 边，用vector因为有可能一个结点有多条边可走
    nfaNode() {
        id = nodeCount++;
        isStart = false;
        isEnd = false;
    }
};

/*
* @brief 结构体，NFA图
*/
struct NFA
{
    nfaNode* start;
    nfaNode* end;
    NFA() {}
    NFA(nfaNode* s, nfaNode* e)
    {
        start = s;
        end = e;
    }
};

/*
* @brief 创建基本字符NFA
* 只包含一个字符的NFA图
*/
NFA CreateBasicNFA(char character) {
    nfaNode* start = new nfaNode();
    nfaNode* end = new nfaNode();

    start->isStart = true;
    end->isEnd = true;

    nfaEdge edge;
    edge.c = character;
    edge.next = end;
    start->edges.push_back(edge);

    NFA nfa(start, end);

    // 存入全局nfa字符set
    nfaCharSet.insert(character);
    // 存入全局dfa字符set
    dfaCharSet.insert(character);

    return nfa;
}

/*
* @brief 创建连接运算符的NFA图
*/ 
NFA CreateConcatenationNFA(NFA nfa1, NFA nfa2) {
    // 把nfa1的终止状态与nfa2的起始状态连接起来
    nfa1.end->isEnd = false;
    nfa2.start->isStart = false;

    nfaEdge edge;
    edge.c = EPSILON; // 这里用EPSILON表示空边
    edge.next = nfa2.start;
    nfa1.end->edges.push_back(edge);

    NFA nfa;
    nfa.start = nfa1.start;
    nfa.end = nfa2.end;

    return nfa;
}

/*
* @brief 创建选择运算符的NFA图
*/
NFA CreateUnionNFA(NFA nfa1, NFA nfa2) {
    nfaNode* start = new nfaNode();
    nfaNode* end = new nfaNode();

    start->isStart = true;
    end->isEnd = true;

    // 把新的初态与nfa1和nfa2的初态连接起来
    nfaEdge edge1;
    edge1.c = EPSILON;
    edge1.next = nfa1.start;
    start->edges.push_back(edge1);
    nfa1.start->isStart = false;    // 初态结束

    nfaEdge edge2;
    edge2.c = EPSILON;
    edge2.next = nfa2.start;
    start->edges.push_back(edge2);
    nfa2.start->isStart = false;    // 初态结束

    // 把nfa1和nfa2的终止状态与新的终止状态连接起来
    nfa1.end->isEnd = false;
    nfa2.end->isEnd = false;

    nfaEdge edge3;
    edge3.c = EPSILON;
    edge3.next = end;
    nfa1.end->edges.push_back(edge3);

    nfaEdge edge4;
    edge4.c = EPSILON;
    edge4.next = end;
    nfa2.end->edges.push_back(edge4);

    NFA nfa{ start , end };

    return nfa;
}

/*
* @brief 创建*运算符的NFA图
*/
NFA CreateZeroOrMoreNFA(NFA nfa1) {
    nfaNode* start = new nfaNode();
    nfaNode* end = new nfaNode();

    start->isStart = true;
    end->isEnd = true;

    // 把新的初态与nfa1的初态连接起来
    nfaEdge edge1;
    edge1.c = EPSILON;
    edge1.next = nfa1.start;
    start->edges.push_back(edge1);
    nfa1.start->isStart = false;    // 初态结束

    // 把新的初态与新的终止状态连接起来
    nfaEdge edge2;
    edge2.c = EPSILON;
    edge2.next = end;
    start->edges.push_back(edge2);

    // 把nfa1的终止状态与新的终止状态连接起来
    nfa1.end->isEnd = false;

    nfaEdge edge3;
    edge3.c = EPSILON;
    edge3.next = nfa1.start;
    nfa1.end->edges.push_back(edge3);

    nfaEdge edge4;
    edge4.c = EPSILON;
    edge4.next = end;
    nfa1.end->edges.push_back(edge4);

    NFA nfa{ start,end };

    return nfa;
}

/*
* @brief 创建？运算符的NFA图
*/
NFA CreateOptionalNFA(NFA nfa1) {
    nfaNode* start = new nfaNode();
    nfaNode* end = new nfaNode();

    start->isStart = true;
    end->isEnd = true;

    // 把新的初态与nfa1的初态连接起来
    nfaEdge edge1;
    edge1.c = EPSILON;
    edge1.next = nfa1.start;
    start->edges.push_back(edge1);
    nfa1.start->isStart = false;    // 初态结束

    // 把新的初态与新的终止状态连接起来
    nfaEdge edge2;
    edge2.c = EPSILON;
    edge2.next = end;
    start->edges.push_back(edge2);

    // 把nfa1的终止状态与新的终止状态连接起来
    nfa1.end->isEnd = false;

    nfaEdge edge3;
    edge3.c = EPSILON;
    edge3.next = end;
    nfa1.end->edges.push_back(edge3);

    NFA nfa(start, end);

    return nfa;
}

/*
* @brief 判断优先级
*/
int Precedence(char op) {
    if (op == '|') {
        return 1;  // 选择运算符 "|" 的优先级
    }
    else if (op == '@') {
        return 2;  // 连接运算符 "@" 的优先级
    }
    else if (op == '*' || op == '?') {
        return 3;  // 闭包运算符 "*"和 "?" 的优先级
    }
    else {
        return 0;  // 默认情况下，将其它字符的优先级设为0
    }
}

/*
* @brief 结构体，状态转换表单个结点
*/
struct statusTableNode
{
    string flag;  // 标记初态还是终态
    int id; // 唯一id值
    map<char, set<int>> m;  // 对应字符能到达的状态
    statusTableNode()
    {
        flag = ""; // 默认为空
    }
};

// 状态转换表
unordered_map<int, statusTableNode> statusTable;
// statusTable插入顺序记录，方便后续输出
vector<int> insertionOrder;
set<int> startNFAstatus;
set<int> endNFAstatus;

/*
* @brief 生成状态转换表
* 使用DFS算法
*/
void createNFAStatusTable(NFA& nfa)
{
    stack<nfaNode*> nfaStack;
    set<nfaNode*> visitedNodes;

    // 初态
    nfaNode* startNode = nfa.start;
    statusTableNode startStatusNode;
    startStatusNode.flag = '-'; // -表示初态
    startStatusNode.id = startNode->id;
    statusTable[startNode->id] = startStatusNode;
    insertionOrder.push_back(startNode->id);
    startNFAstatus.insert(startNode->id);

    nfaStack.push(startNode);

    while (!nfaStack.empty()) {
        nfaNode* currentNode = nfaStack.top();
        nfaStack.pop();
        visitedNodes.insert(currentNode);

        for (nfaEdge edge : currentNode->edges) {
            char transitionChar = edge.c;
            nfaNode* nextNode = edge.next;

            // 记录状态转换信息
            statusTable[currentNode->id].m[transitionChar].insert(nextNode->id);

            // 如果下一个状态未被访问，将其加入堆栈
            if (visitedNodes.find(nextNode) == visitedNodes.end()) {
                nfaStack.push(nextNode);

                // 记录状态信息
                statusTableNode nextStatus;
                nextStatus.id = nextNode->id;
                if (nextNode->isStart) {
                    nextStatus.flag += '-'; // -表示初态
                    startNFAstatus.insert(nextStatus.id);
                }
                else if (nextNode->isEnd) {
                    nextStatus.flag += '+'; // +表示终态
                    endNFAstatus.insert(nextStatus.id);
                }
                statusTable[nextNode->id] = nextStatus;
                // 记录插入顺序（排除终态）
                if (!nextNode->isEnd)
                {
                    insertionOrder.push_back(nextNode->id);
                }
            }
        }
    }

    // 顺序表才插入终态
    nfaNode* endNode = nfa.end;
    insertionOrder.push_back(endNode->id);
}

// 测试输出NFA状态转换表程序（debug使用）
void printStatusTable() {
    // 打印状态表按照插入顺序
    for (int id : insertionOrder) {
        const statusTableNode& node = statusTable[id];
        qDebug() << "Node ID: " << node.id << ", Flag: " << QString::fromStdString(node.flag) << "\n";

        for (const auto& entry : node.m) {
            char transitionChar = entry.first;
            const std::set<int>& targetStates = entry.second;

            qDebug() << "  Transition: " << transitionChar << " -> {";
            for (int targetState : targetStates) {
                qDebug() << targetState << " ";
            }
            qDebug() << "}\n";
        }
    }
}


/*
* @brief 正则表达式转NFA入口
*/
NFA regex2NFA(string regex)
{
    // 双栈法，创建两个栈opStack（运算符栈）,nfaStack（nfa图栈）
    stack<char> opStack;
    stack<NFA> nfaStack;

    // 对表达式进行类似于逆波兰表达式处理
    // 运算符：| @（） ？ +  *
    for (char c : regex)
    {
        switch (c)
        {
        case ' ': // 空格跳过
            break;
        case '(':
            opStack.push(c);
            break;
        case ')':
            while (!opStack.empty() && opStack.top() != '(')
            {
                // 处理栈顶运算符，构建NFA图，并将结果入栈
                char op = opStack.top();
                opStack.pop();

                if (op == '|') {
                    // 处理并构建"|"运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA resultNFA = CreateUnionNFA(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
                else if (op == '@') {
                    // 处理并构建"."运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 . nfa2
                    NFA resultNFA = CreateConcatenationNFA(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
            }
            if (opStack.empty())
            {
                qDebug() << "括号未闭合，请检查正则表达式！";
                exit(-1);
            }
            else
            {
                opStack.pop(); // 弹出(
            }
            break;
        case '|':
        case '@':
            // 处理运算符 | 和 @
            while (!opStack.empty() && (opStack.top() == '|' || opStack.top() == '@') &&
                Precedence(opStack.top()) >= Precedence(c))
            {
                char op = opStack.top();
                opStack.pop();

                // 处理栈顶运算符，构建NFA图，并将结果入栈
                if (op == '|') {
                    // 处理并构建"|"运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    NFA resultNFA = CreateUnionNFA(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
                else if (op == '@') {
                    // 处理并构建"."运算符
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的 NFA，表示 nfa1 . nfa2
                    NFA resultNFA = CreateConcatenationNFA(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
            }
            opStack.push(c);
            break;
        case '?':
        case '*':
            // 处理闭包运算符 ? + *
            // 从nfaStack弹出NFA，应用相应的闭包操作，并将结果入栈
            if (!nfaStack.empty()) {
                NFA nfa = nfaStack.top();
                nfaStack.pop();
                if (c == '?') {
                    // 处理 ?
                    NFA resultNFA = CreateOptionalNFA(nfa);
                    nfaStack.push(resultNFA);
                }
                else if (c == '*') {
                    // 处理 *
                    NFA resultNFA = CreateZeroOrMoreNFA(nfa);
                    nfaStack.push(resultNFA);
                }
            }
            else {
                qDebug() << "正则表达式语法错误：闭包操作没有NFA可用！";
                exit(-1);
            }
            break;
        default:
            // 处理字母字符
            NFA nfa = CreateBasicNFA(c); // 创建基本的字符NFA
            nfaStack.push(nfa);
            break;
        }

    }

    // 处理栈中剩余的运算符
    while (!opStack.empty())
    {
        char op = opStack.top();
        opStack.pop();

        if (op == '|' || op == '@')
        {
            // 处理并构建运算符 | 和 .
            if (nfaStack.size() < 2)
            {
                qDebug() << "正则表达式语法错误：不足以处理运算符 " << op << "！";
                exit(-1);
            }

            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();

            if (op == '|')
            {
                // 创建新的 NFA，表示 nfa1 | nfa2
                NFA resultNFA = CreateUnionNFA(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
            else if (op == '@')
            {
                // 创建新的 NFA，表示 nfa1 . nfa2
                NFA resultNFA = CreateConcatenationNFA(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
        }
        else
        {
            qDebug() << "正则表达式语法错误：未知的运算符 " << op << "！";
            exit(-1);
        }
    }

    // 最终的NFA图在nfaStack的顶部
    NFA result = nfaStack.top();
    qDebug() << "NFA图构建完毕" << endl;

    createNFAStatusTable(result);
    qDebug() << "状态转换表构建完毕" << endl;

    return result;
}

/*============================NFA转DFA==================================*/
//map<int, nfaNode> nfaStates;

// dfa节点
struct dfaNode
{
    string flag; // 是否包含终态（+）或初态（-）
    set<int> nfaStates; // 该DFA状态包含的NFA状态的集合
    map<char, set<int>> transitions; // 字符到下一状态的映射
    dfaNode() {
        flag = "";
    }
};
// dfa状态去重集
set<set<int>> dfaStatusSet;

// dfa最终结果
vector<dfaNode> dfaTable;

//下面用于DFA最小化
// dfa终态集合
set<int> dfaEndStatusSet;
// dfa非终态集合
set<int> dfaNotEndStatusSet;
// set对应序号MAP
map<set<int>, int> dfa2numberMap;
int startStaus;

// 判断是否含有初态终态，含有则返回对应字符串
string setHasStartOrEnd(set<int>& statusSet)
{
    string result = "";
    for (const int& element : startNFAstatus) {
        if (statusSet.count(element) > 0) {
            result += "-";
        }
    }

    for (const int& element : endNFAstatus) {
        if (statusSet.count(element) > 0) {
            result += "+";
        }
    }

    return result;
}

set<int> epsilonClosure(int id)
{
    set<int> eResult{ id };
    stack<int> stack;
    stack.push(id);

    while (!stack.empty())
    {
        int current = stack.top();
        stack.pop();

        set<int> eClosure = statusTable[current].m[EPSILON];
        for (auto t : eClosure)
        {
            if (eResult.find(t) == eResult.end())
            {
                eResult.insert(t);
                stack.push(t);
            }
        }
    }

    return eResult;
}

set<int> otherCharClosure(int id, char ch)
{
    set<int> otherResult{};
    set<int> processed;
    stack<int> stack;
    stack.push(id);

    while (!stack.empty())
    {
        int current = stack.top();
        stack.pop();

        if (processed.find(current) != processed.end())
            continue;

        processed.insert(current);

        set<int> otherClosure = statusTable[current].m[ch];
        for (auto o : otherClosure)
        {
            auto tmp = epsilonClosure(o);
            otherResult.insert(tmp.begin(), tmp.end());
            stack.push(o);
        }
    }

    return otherResult;
}



// DFA debug输出函数
void printDfaTable(const vector<dfaNode>& dfaTable) {
    for (size_t i = 0; i < dfaTable.size(); ++i) {
        qDebug() << "DFA Node " << i << " - Flag: " << QString::fromStdString(dfaTable[i].flag);
        qDebug() << "NFA States: ";
        for (const int state : dfaTable[i].nfaStates) {
            qDebug() << state;
        }
        qDebug() << "Transitions: ";
        for (const auto& transition : dfaTable[i].transitions) {
            qDebug() << "  Input: " << transition.first;
            qDebug() << "  Next States: ";
            for (const int nextState : transition.second) {
                qDebug() << nextState;
            }
        }
        qDebug() << "---------------------";
    }
}

void NFA2DFA(NFA& nfa)
{
    int dfaStatusCount = 1;
    auto start = nfa.start; // 获得NFA图的起始位置
    auto startId = start->id;   // 获得起始编号
    dfaNode startDFANode;
    startDFANode.nfaStates = epsilonClosure(startId); // 初始闭包
    startDFANode.flag = setHasStartOrEnd(startDFANode.nfaStates); // 判断初态终态
    deque<set<int>> newStatus{};
    dfa2numberMap[startDFANode.nfaStates] = dfaStatusCount;
    startStaus = dfaStatusCount;
    if (setHasStartOrEnd(startDFANode.nfaStates).find("+") != string::npos) {
        dfaEndStatusSet.insert(dfaStatusCount++);
    }
    else
    {
        dfaNotEndStatusSet.insert(dfaStatusCount++);
    }
    // 对每个字符进行遍历
    for (auto ch : dfaCharSet)
    {
        set<int> thisChClosure{};
        for (auto c : startDFANode.nfaStates)
        {
            set<int> tmp = otherCharClosure(c, ch);
            thisChClosure.insert(tmp.begin(), tmp.end());
        }
        if (thisChClosure.empty())  // 如果这个闭包是空集没必要继续下去了
        {
            continue;
        }
        int presize = dfaStatusSet.size();
        dfaStatusSet.insert(thisChClosure);
        int lastsize = dfaStatusSet.size();
        // 不管一不一样都是该节点这个字符的状态
        startDFANode.transitions[ch] = thisChClosure;
        // 如果大小不一样，证明是新状态
        if (lastsize > presize)
        {
            dfa2numberMap[thisChClosure] = dfaStatusCount;
            newStatus.push_back(thisChClosure);
            if (setHasStartOrEnd(thisChClosure).find("+") != string::npos) {
                dfaEndStatusSet.insert(dfaStatusCount++);
            }
            else
            {
                dfaNotEndStatusSet.insert(dfaStatusCount++);
            }

        }

    }
    dfaTable.push_back(startDFANode);

    // 对后面的新状态进行不停遍历
    while (!newStatus.empty())
    {
        // 拿出一个新状态
        set<int> ns = newStatus.front();
        newStatus.pop_front();
        dfaNode DFANode;
        DFANode.nfaStates = ns;  // 该节点状态集合
        DFANode.flag = setHasStartOrEnd(ns);

        for (auto ch : dfaCharSet)
        {

            set<int> thisChClosure{};
            for (auto c : ns)
            {
                set<int> tmp = otherCharClosure(c, ch);
                thisChClosure.insert(tmp.begin(), tmp.end());
            }
            if (thisChClosure.empty())  // 如果这个闭包是空集没必要继续下去了
            {
                continue;
            }
            int presize = dfaStatusSet.size();
            dfaStatusSet.insert(thisChClosure);
            int lastsize = dfaStatusSet.size();
            // 不管一不一样都是该节点这个字符的状态
            DFANode.transitions[ch] = thisChClosure;
            // 如果大小不一样，证明是新状态
            if (lastsize > presize)
            {
                dfa2numberMap[thisChClosure] = dfaStatusCount;
                newStatus.push_back(thisChClosure);
                if (setHasStartOrEnd(thisChClosure).find("+") != string::npos) {
                    dfaEndStatusSet.insert(dfaStatusCount++);
                }
                else
                {
                    dfaNotEndStatusSet.insert(dfaStatusCount++);
                }

            }

        }
        dfaTable.push_back(DFANode);

    }

    // dfa debug
    // printDfaTable(dfaTable);
}

/*============================DFA最小化==================================*/
/*
// dfa终态集合
set<int> dfaEndStatusSet;
// dfa非终态集合
set<int> dfaNotEndStatusSet;
// set对应序号MAP
map<set<int>, int> dfa2numberMap;
*/

// 判断是否含有初态终态，含有则返回对应字符串
string minSetHasStartOrEnd(set<int>& statusSet)
{
    string result = "";
    if (statusSet.count(startStaus) > 0) {
        result += "-";
    }

    for (const int& element : dfaEndStatusSet) {
        if (statusSet.count(element) > 0) {
            result += "+";
            break;  // 可能会有多个终态同时包含，我们只要一个
        }
    }

    return result;
}

// dfa最小化节点
struct dfaMinNode
{
    string flag; // 是否包含终态（+）或初态（-）
    int id;
    map<char, int> transitions; // 字符到下一状态的映射
    dfaMinNode() {
        flag = "";
    }
};

vector<dfaMinNode> dfaMinTable;

// 用于分割集合
vector<set<int>> divideVector;
// 存下标
map<int, int> dfaMinMap;


// 根据字符 ch 将状态集合 node 分成两个子集合
void splitSet(int i, char ch)
{
    set<int> result;
    auto& node = divideVector[i];
    int s = -2;

    for (auto state : node)
    {
        int thisNum;
        if (dfaTable[state - 1].transitions.find(ch) == dfaTable[state - 1].transitions.end())
        {
            thisNum = -1; // 空集
        }
        else
        {
            // 根据字符 ch 找到下一个状态
            int next_state = dfa2numberMap[dfaTable[state - 1].transitions[ch]];
            thisNum = dfaMinMap[next_state];    // 这个状态的下标是多少
        }

        if (s == -2)    // 初始下标
        {
            s = thisNum;
        }
        else if (thisNum != s)   // 如果下标不同，就是有问题，需要分出来
        {
            result.insert(state);
        }
    }

    // 删除要删除的元素
    for (int state : result) {
        node.erase(state);
    }

    // 都遍历完了，如果result不是空，证明有新的，加入vector中
    if (!result.empty())
    {
        divideVector.push_back(result);
        // 同时更新下标
        for (auto a : result)
        {
            dfaMinMap[a] = divideVector.size() - 1;
        }
    }

}

void DFAminimize()
{
    divideVector.clear();
    dfaMinMap.clear();

    // 存入非终态、终态集合
    if (dfaNotEndStatusSet.size() != 0)
    {
        divideVector.push_back(dfaNotEndStatusSet);
    }
    // 初始化map
    for (auto t : dfaNotEndStatusSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    divideVector.push_back(dfaEndStatusSet);

    for (auto t : dfaEndStatusSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    // 当flag为1时，一直循环
    int continueFlag = 1;

    while (continueFlag)
    {
        continueFlag = 0;
        int size1 = divideVector.size();

        for (int i = 0; i < size1; i++)
        {

            // 逐个字符尝试分割状态集合
            for (char ch : dfaCharSet)
            {
                splitSet(i, ch);
            }
        }
        int size2 = divideVector.size();
        if (size2 > size1)
        {
            continueFlag = 1;
        }
    }

    for (int dfaMinCount = 0; dfaMinCount < divideVector.size(); dfaMinCount++)
    {
        auto& v = divideVector[dfaMinCount];
        dfaMinNode d;
        d.flag = minSetHasStartOrEnd(v);
        d.id = dfaMinCount;
        // 逐个字符
        for (char ch : dfaCharSet)
        {
            if (v.size() == 0)
            {
                d.transitions[ch] = -1;   // 空集特殊判断
                continue;
            }
            int i = *(v.begin()); // 拿一个出来
            if (dfaTable[i - 1].transitions.find(ch) == dfaTable[i - 1].transitions.end())
            {
                d.transitions[ch] = -1;   // 空集特殊判断
                continue;
            }
            int next_state = dfa2numberMap[dfaTable[i - 1].transitions[ch]];
            int thisNum = dfaMinMap[next_state];    // 这个状态下标
            d.transitions[ch] = thisNum;
        }
        dfaMinTable.push_back(d);
    }

    // 输出 dfaMinTable
    for (const dfaMinNode& node : dfaMinTable) {
        qDebug() << "State " << node.id << ":";
        qDebug() << "Flag: " << QString::fromStdString(node.flag);

        for (const auto& entry : node.transitions) {
            qDebug() << entry.first << " -> " << entry.second;
        }
    }

    qDebug() << "DFA最小化完成！";
}

bool genLexCase(QList<QString> tmpList, QString& codeStr, int idx, bool flag)
{
    bool rFlag = false;
    for (int i = 0; i < tmpList.size(); i++)
    {
        QString tmpKey = tmpList[i];
        char ch = tmpKey.toUtf8().constData()[0];
        if (dfaMinTable[idx].transitions[ch] == -1) {
            continue;
        }
        //字母情况
        if (tmpKey == QString((char)2))
        {
            for (int j = 0; j < 26; j++)
            {
                codeStr += "\t\t\tcase \'" + QString(char('a' + j)) + "\':\n";
                codeStr += "\t\t\tcase \'" + QString(char('A' + j)) + "\':\n";
            }
            codeStr.chop(1);//去掉末尾字符
            if (flag)codeStr += "isIdentifier = true; ";
        }
        else if (tmpKey == QString((char)1))
        {
            //数字情况
            for (int j = 0; j < 10; j++)
            {
                codeStr += "\t\t\tcase \'" + QString::number(j) + "\':\n";
            }
            codeStr.chop(1);
            if (flag)codeStr += "isDigit = true; ";
        }
        else if (tmpKey == "~")
        {
            rFlag = true;
            continue;
        }
        else {
            QString keyRes = tmpKey;
            if (m1.find(ch) != m1.end()) {
                keyRes = QString::fromStdString(trim(m1[ch]));
            }
            codeStr += "\t\t\tcase \'" + keyRes + "\':";
        }

        if (flag) {
            codeStr += "state = " + QString::number(dfaMinTable[idx].transitions[ch]) + "; ";
        }
        codeStr += "break;\n";
    }
    return rFlag;
}

QString generateLexer(QString filePath)
{
    QString lexCode;

    //库函数
    lexCode += 
        "#include<stdio.h>\n"
        "#include<stdlib.h>\n"
        "#include<string.h>\n"
        "#include<stdbool.h>\n";

    //关键字Set
    lexCode += "const char* keywordSet[" + QString::number((int)keyWords.size()) + QString::fromStdString("] = {");
    for (const auto& tmp : keyWords)
    {
        lexCode += QString::fromStdString("\"" + tmp + "\",");
    }
    lexCode.chop(1);
    lexCode += " };\n";

    //符号映射map
    lexCode += "static struct\n";
    lexCode += "{\n";
    lexCode += "\tconst char* str;\n";
    lexCode += "\tconst char* value;\n";
    lexCode += "} reservedWords[" + QString::number((int)opMap.size()) + QString::fromStdString("] = {");
    for (const auto& tmp : opMap)
    {
        // 拿到第一个字符
        char first = tmp.first[0];
        string keyRes = tmp.first;
        // 去判断是不是特殊处理的
        if (m1.find(first) != m1.end()) {
            // 是的话，要取second值展示
            keyRes = trim(m1[first]);
        }
        lexCode += QString::fromStdString("{\"" + keyRes + "\",\"" + tmp.second + "\"},");
    }
    lexCode.chop(1);
    lexCode += " };\n";

    // 一些函数
    lexCode +=
        "void concat(char str[], char tmp) {\n"
        "\tsize_t len = strlen(str);\n"
        "\n"
        "\tstr[len] = tmp;\n"
        "\n"
        "\tstr[len + 1] = '\\0';\n"
        "}\n"
        "\n"
        "bool findKeyWord(const char* str) {\n"
        "\tint i = 0;\n"
        "\tfor (i = 0; i < "+ QString::number((int)keyWords.size()) + "; i++) {\n"
        "\t\tif (strcmp(str,keywordSet[i]) == 0) {\n"
        "\t\t\treturn true;\n"
        "\t\t}\n"
        "\t}\n"
        "\treturn false;\n"
        "}\n"
        "\n"
        "const char* getValue(const char* str) {\n"
        "\tint i = 0;\n"
        "\tfor (i = 0; i < " + QString::number((int)opMap.size()) + "; ++i) {\n"
        "\t\tif (strcmp(reservedWords[i].str, str) == 0) {\n"
        "\t\t\treturn reservedWords[i].value;\n"
        "\t\t}\n"
        "\t}\n"
        "\treturn NULL;\n"
        "}\n";


    //生成分析代码
    lexCode += "void coding(FILE* input_fp,FILE* output_fp) {\n"
        "\tchar tmp = fgetc(input_fp);\n";
    if (isLowerCase) {
        lexCode += "\tif (isupper(tmp)) {\n"
            "\t\ttmp = tolower(tmp);\n"
            "\t}\n";
    }
        
    lexCode += "\tif (tmp == ' ' || tmp == '\\n' || tmp == '\\t'){\n"
        "\t\treturn;\n"
        "\t}\n"
        "\tungetc(tmp, input_fp);\n"
        "\tint state = 0;\n"
        "\tbool flag = false;\n"
        "\tbool isIdentifier = false;\n"
        "\tbool isDigit = false;\n"
        "\tbool isAnnotation = false;\n"
        "\tchar value[1024];\n"
        "\tvalue[0] = '\\0';\n"
        "\twhile (!flag) {\n"
        "\t\ttmp = fgetc(input_fp);\n"
        "\t\tswitch (state) {\n";

    for (int i = 0; i < dfaMinTable.size(); i++)
    {
        if (dfaMinTable[i].transitions.size()) {
            lexCode += "\t\tcase " + QString::number(dfaMinTable[i].id) + ": {\n";
            lexCode += "\t\t\tswitch (tmp) {\n";
            QList<QString> tmpList;
            for (const auto& pair : dfaMinTable[i].transitions) {
                tmpList.append(QString(pair.first));
            }
            if (genLexCase(tmpList, lexCode, i, 1))
                lexCode += "\t\t\tdefault:state = " + QString::number(dfaMinTable[i].transitions['~']) + "; isAnnotation = true; break;\n";
            lexCode += "\t\t\t}\n";
            lexCode += "\t\t\tbreak;\n";
            lexCode += "\t\t}\n";
        }
    }
    lexCode += "\t\t}\n";
    lexCode += "\t\tconcat(value, tmp);\n";


    // 终态集合
    QList<int> stateList;
    for (const dfaMinNode& node : dfaMinTable) {
        if (node.flag.find("+") != string::npos) {
            stateList.append(node.id);
        }
    }

    lexCode += "\t\tif (";
    for (int i = 0; i < stateList.size(); i++)
    {
        //要提前读一个字符判断是不是真的到终态，因为到了终态不一定是真正的终态
        int num = stateList[i];
        if (i)lexCode += "\t\telse if (";
        lexCode += "state ==" + QString::number(num) + ") {\n";
        lexCode += "\t\t\ttmp = fgetc(input_fp);\n";
        lexCode += "\t\t\tswitch (tmp) {\n";
        QList<QString> tmpList;
        for (const auto& pair : dfaMinTable[i].transitions) {
            tmpList.append(QString(pair.first));
        }
        genLexCase(tmpList, lexCode, num, 0);
        lexCode += "\t\t\tdefault: {\n";
        lexCode += "\t\t\t\tflag=true;\n";
        if (tmpList.contains("letter"))
            lexCode += "\t\t\t\tisIdentifier = true;\n";
        lexCode += "\t\t\t}\n"
            "\t\t\t}\n"
            "\t\t\tungetc(tmp, input_fp);\n"
            "\t\t}\n";
    }
    lexCode += "\t}\n";

    //为适配解码增加Keyword:前缀，数字Digit:前缀，ID:标识符前缀 Operaotr:专用符号

    lexCode += 
        "\tif (findKeyWord(value)) {\n"
        "\t\tfprintf(output_fp, \"%s:%s \\n\", value, value);\n"
        "\t\tprintf(\"%s:%s \\n\", value, value);\n"
        "\t\treturn;\n"
        "\t}\n"
        "\tif (isIdentifier) {\n"
        "\t\tfprintf(output_fp, \"ID:%s \\n\", value);\n"
        "\t\tprintf(\"ID:%s \\n\", value);\n"
        "\treturn;\n"
        "\t}\n"
        "\tif (isDigit) {\n"
        "\t\tfprintf(output_fp, \"NUMBER:%s \\n\", value);\n"
        "\t\tprintf(\"NUMBER:%s \\n\", value);\n"
        "\t\treturn;\n"
        "\t}\n"
        "\tif (!isAnnotation) {\n"
        "\t\tfprintf(output_fp, \"%s:%s \\n\", getValue(value), value);\n"
        "\t\tprintf(\"%s:%s \\n\", getValue(value), value);\n"
        "\t}\n"
        "};\n";

    //主函数
    QFileInfo fileInfo(filePath);
    QString t_tmpFilePath = fileInfo.path();
    lexCode += "int main(int argc, char* argv[]) {\n"
        "\tFILE* input_fp = fopen(\"" + filePath + "/_sample.tny\", \"r\");\n"
        "\tif (input_fp == NULL) {\n"
        "\t\tprintf(\"Failed to open input file\");\n"
        "\t\treturn 1;\n"
        "\t}\n";

    lexCode += 
        "\tFILE* output_fp = fopen(\"" + filePath + "/output.lex" + "\", \"w\");\n"
        "\tif (output_fp == NULL) {\n"
        "\t\tprintf(\"Failed to open output file\");\n"
        "\t\tfclose(input_fp);\n"
        "\t\treturn 1;\n"
        "\t}\n";

    lexCode += "\tchar c;\n"
        "\twhile ((c=fgetc(input_fp)) != EOF) {\n"
        "\t\tungetc(c, input_fp);\n"
        "\t\tcoding(input_fp, output_fp);\n"
        "\t}\n"
        "\tfprintf(output_fp, \"EOF:EOF\");\n"
        "\tprintf(\"EOF:EOF\");\n"
        "\tfclose(input_fp);\n"
        "\tfclose(output_fp);\n"
        "\treturn 0;\n"
        "}";

    return lexCode;
}


/*
* @brief 初始化函数
* 用于清空全局变量
*/
void init() {
    // 全局变量清空
    keyWords.clear();
    finalRegex.clear();
    regexLine->clear();
    commentSymbol->clear();
    nodeCount = 0;
    nfaCharSet.clear();
    dfaCharSet.clear();
    statusTable.clear();
    insertionOrder.clear();
    startNFAstatus.clear();
    endNFAstatus.clear();
    dfaStatusSet.clear();
    dfaEndStatusSet.clear();
    dfaNotEndStatusSet.clear();
    dfaMinTable.clear();
    divideVector.clear();
    dfaMinMap.clear();
    dfaTable.clear();
    nfaCharSet.insert(EPSILON); // 放入epsilon
}

/*
* @brief 开始分析按钮
*/
void Widget::on_pushButton_clicked(){
    // 清空全局变量
    init();

    // 拿到所有的正则表达式
    QString allRegex = ui->plainTextEdit_2->toPlainText();

    isLowerCase = ui->checkBox->isChecked();
    qDebug() <<"是否区分大小写："<< isLowerCase;

    string result = handleAllRegex(allRegex, isLowerCase);
    // 如果字符串不为空就是报错了，退出
    if (!result.empty()) {
        QMessageBox::critical(this, "错误信息", QString::fromStdString(result));
        return;
    }

    //正则表达式转换成NFA图
    NFA nfa = regex2NFA(finalRegex);

    // NFA转DFA
    NFA2DFA(nfa);

    DFAminimize();

    QMessageBox::about(this, "提示", "分析成功！请点击其余按钮查看结果！");
}

/*
* @brief 生成NFA按钮
*/
void Widget::on_pushButton_4_clicked()
{
    ui->tableWidget->clearContents(); // 清除表格中的数据
    ui->tableWidget->setRowCount(0); // 清除所有行
    ui->tableWidget->setColumnCount(0); // 清除所有列
    // 设置列数
    int n = 2 + nfaCharSet.size(); // 默认两列：Flag 和 ID
    ui->tableWidget->setColumnCount(n);

    // 字符和第X列存起来对应
    map<char, int> headerCharNum;

    // 设置表头
    QStringList headerLabels;
    headerLabels << "标志" << "ID";
    int headerCount = 3;
    for (const auto& ch : nfaCharSet) {
        if (m1.find(ch) != m1.end()) {
            headerLabels << QString::fromStdString(trim(m1[ch]));
        }
        else {
            if (ch == '~') {
                char tt = commentSymbol[1][0];
                string res = commentSymbol[1];
                if (m1.find(tt) != m1.end()) {
                    res = trim(m1[tt]);
                }
                headerLabels << QString::fromStdString("非" + res);
            }
            else {
                headerLabels << QString(ch);
            }
        }
        
        headerCharNum[ch] = headerCount++;
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    // 设置行数
    int rowCount = statusTable.size();
    ui->tableWidget->setRowCount(rowCount);

    // 填充数据
    int row = 0;
    for (auto id : insertionOrder) {
        const statusTableNode& node = statusTable[id];

        // Flag 列
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(node.flag)));

        // ID 列
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(node.id)));

        // TransitionChar 列
        int col = 2;
        for (const auto& transitionEntry : node.m) {
            string resutlt = set2string(transitionEntry.second);

            // 放到指定列数据
            ui->tableWidget->setItem(row, headerCharNum[transitionEntry.first] - 1, new QTableWidgetItem(QString::fromStdString(resutlt)));
            col++;
        }

        row++;
    }

    // 调整列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 显示表格
    ui->tableWidget->show();
}

/*
* @brief 生成DFA按钮
*/
void Widget::on_pushButton_3_clicked()
{
    ui->tableWidget->clearContents(); // 清除表格中的数据
    ui->tableWidget->setRowCount(0); // 清除所有行
    ui->tableWidget->setColumnCount(0); // 清除所有列

    // 设置列数
    int n = 2 + dfaCharSet.size(); // 默认两列：Flag 和 状态集合
    ui->tableWidget->setColumnCount(n);

    // 字符和第X列存起来对应
    map<char, int> headerCharNum;

    // 设置表头
    QStringList headerLabels;
    headerLabels << "标志" << "状态集合";
    int headerCount = 3;
    for (const auto& ch : dfaCharSet) {
        if (m1.find(ch) != m1.end()) {
            headerLabels << QString::fromStdString(trim(m1[ch]));
        }
        else {
            if (ch == '~') {
                char tt = commentSymbol[1][0];
                string res = commentSymbol[1];
                if (m1.find(tt) != m1.end()) {
                    res = trim(m1[tt]);
                }
                headerLabels << QString::fromStdString("非" + res);
            }
            else {
                headerLabels << QString(ch);
            }
        }

        headerCharNum[ch] = headerCount++;
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    // 设置行数
    int rowCount = dfaTable.size();
    ui->tableWidget->setRowCount(rowCount);

    // 填充数据
    int row = 0;
    for (auto& dfaNode : dfaTable) {

        // Flag 列
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(dfaNode.flag)));

        // 状态集合 列
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString("{" + set2string(dfaNode.nfaStates) + "}")));

        // 状态转换 列
        int col = 2;
        for (const auto& transitionEntry : dfaNode.transitions) {
            string re = set2string(transitionEntry.second);

            // 放到指定列数据
            ui->tableWidget->setItem(row, headerCharNum[transitionEntry.first] - 1, new QTableWidgetItem(QString::fromStdString("{" + re + "}")));
            col++;
        }

        row++;
    }

    // 调整列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 显示表格
    ui->tableWidget->show();
}

/*
* @brief DFA最小化按钮
*/
void Widget::on_pushButton_5_clicked()
{
    ui->tableWidget->clearContents(); // 清除表格中的数据
    ui->tableWidget->setRowCount(0); // 清除所有行
    ui->tableWidget->setColumnCount(0); // 清除所有列

    // 设置列数
    int n = 2 + dfaCharSet.size(); // 默认两列：Flag 和 状态集合
    ui->tableWidget->setColumnCount(n);

    // 字符和第X列存起来对应
    map<char, int> headerCharNum;

    // 设置表头
    QStringList headerLabels;
    headerLabels << "标志" << "ID";
    int headerCount = 3;
    for (const auto& ch : dfaCharSet) {
        if (m1.find(ch) != m1.end()) {
            headerLabels << QString::fromStdString(trim(m1[ch]));
        }
        else {
            if (ch == '~') {
                char tt = commentSymbol[1][0];
                string res = commentSymbol[1];
                if (m1.find(tt) != m1.end()) {
                    res = trim(m1[tt]);
                }
                headerLabels << QString::fromStdString("非" + res);
            }
            else {
                headerLabels << QString(ch);
            }
        }

        headerCharNum[ch] = headerCount++;
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    // 设置行数
    int rowCount = dfaMinTable.size();
    ui->tableWidget->setRowCount(rowCount);

    // 填充数据
    int row = 0;
    for (auto& dfaNode : dfaMinTable) {

        // Flag 列
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(dfaNode.flag)));

        // 状态集合 列
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(dfaNode.id)));

        // 状态转换 列
        int col = 2;
        for (const auto& transitionEntry : dfaNode.transitions) {
            // 放到指定列数据
            ui->tableWidget->setItem(row, headerCharNum[transitionEntry.first] - 1, new QTableWidgetItem(transitionEntry.second == -1 ? QString::fromStdString("") : QString::number(transitionEntry.second)));
            col++;
        }

        row++;
    }

    // 调整列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 显示表格
    ui->tableWidget->show();
}

/*
* @brief 查看规则按钮
*/
void Widget::on_pushButton_9_clicked()
{
    QString message = R"(
* 第一行：keyword，多个用 | 进行拼接成正则表达式
* 第二行：除了注释符号的专用符号的正则表达式，用|连接，如果与正则表达式符号冲突（包括：+、|、（、）、*、？、[、]、~），用\进行包围，如：\+\
* 第三行：第二行符号对应的名称，用|分隔
* 第四行：标识符的正则表达式，可以用\letter\表示所有字母，\num\表示所有数字
* 第五行：数字的正则表达式
* 第六行：注释的正则表达式，用~表示除结束符号的任意字符，如{~}
注意：词法分析程序生成时，请将sample.tny存放到选择的文件夹中。
)";

    QMessageBox::information(this, "输入规则", message);
}

/*
* @brief LEX生成按钮
*/
void Widget::on_pushButton_2_clicked()
{
    QString srcFilePath;
    QString t_filePath=QFileDialog::getExistingDirectory(this,"选择词法分析程序生成路径，并将sample.tny存放在该路径",QDir::currentPath());
    if(t_filePath.isEmpty())
        return;
    else
        srcFilePath=t_filePath;

    qDebug() << "生成词法分析程序...";
    QString res = generateLexer(srcFilePath);//调用主函数
    qDebug() << "词法分析程序生成完成...";

    /*==========显示处理=================*/
    //切换表格
    //ui->tabWidget_Graph->setCurrentIndex(4);
    //显示词法分析程序
    //NDFAG.printLexer(ui->plainTextEdit_Lexer);

    /*==========文件处理=================*/
    QFile tgtFile(srcFilePath+"/_lexer.c");
    if(!tgtFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
    {
        QMessageBox::warning(NULL, "文件", "文件打开/写入失败");
        return;
    }
    QTextStream outputFile(&tgtFile);
    //QString tgStr=ui->plainTextEdit_Lexer->toPlainText();
    outputFile<< res;
    tgtFile.close();

    ui->tableWidget->hide();
    ui->plainTextEdit->show();

    ui->plainTextEdit->setPlainText(res);
}

void Widget::on_pushButton_6_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath(), tr("文本文件 (*.txt);;所有文件 (*.*)"));

    if (!filePath.isEmpty())
    {
        ifstream inputFile;
        QTextCodec* code = QTextCodec::codecForName("GB2312");

        string selectedFile = code->fromUnicode(filePath.toStdString().c_str()).data();
        inputFile.open(selectedFile.c_str(), ios::in);


        //        cout<<filePath.toStdString();
        //        ifstream inputFile(filePath.toStdString());
        if (!inputFile) {
            QMessageBox::critical(this, "错误信息", "导入错误！无法打开文件，请检查路径和文件是否被占用！");
            cerr << "Error opening file." << endl;
        }
        // 读取文件内容并显示在 plainTextEdit_2
        stringstream buffer;
        buffer << inputFile.rdbuf();
        QString fileContents = QString::fromStdString(buffer.str());
        ui->plainTextEdit_2->setPlainText(fileContents);
    }

}

void Widget::on_pushButton_7_clicked()
{
    // 保存结果到文本文件
    QString saveFilePath = QFileDialog::getSaveFileName(this, tr("保存结果文件"), QDir::homePath(), tr("文本文件 (*.txt)"));
    if (!saveFilePath.isEmpty() && !ui->plainTextEdit_2->toPlainText().isEmpty()) {
        QFile outputFile(saveFilePath);
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&outputFile);
            stream << ui->plainTextEdit_2->toPlainText();
            outputFile.close();
            QMessageBox::about(this, "提示", "导出成功！");
        }
    }
    else if (ui->plainTextEdit_2->toPlainText().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("输入框为空，请重试！"));
    }
}

void Widget::on_pushButton_8_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath(), tr("LEX文件 (*.lex)"));

    if (!filePath.isEmpty())
    {
        ifstream inputFile;
        QTextCodec* code = QTextCodec::codecForName("GB2312");

        string selectedFile = code->fromUnicode(filePath.toStdString().c_str()).data();
        inputFile.open(selectedFile.c_str(), ios::in);


        //        cout<<filePath.toStdString();
        //        ifstream inputFile(filePath.toStdString());
        if (!inputFile) {
            QMessageBox::critical(this, "错误信息", "导入错误！无法打开文件，请检查路径和文件是否被占用！");
            cerr << "Error opening file." << endl;
        }
        // 读取文件内容并显示在 plainTextEdit_2
        stringstream buffer;
        buffer << inputFile.rdbuf();
        QString fileContents = QString::fromStdString(buffer.str());
        ui->tableWidget->hide();
        ui->plainTextEdit->show();
        ui->plainTextEdit->setPlainText(fileContents);
    }
}

