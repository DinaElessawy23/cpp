#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;


enum gateTypes
{
    AND_t,
    OR_t,
    NAND_t,
    NOR_t,
    XOR_t,
    XNOR_t,
    NOT_t
};


class Node
{
private:
    string name;
    bool value;

public:
    Node()
    {
        this->name = "A";
        this->value = 0;
    }

    Node(string name, bool value)
    {
        this->name = name;
        this->value = value;
    }

    void setName(string name)
    {
        this->name = name;
    }

    void setValue(bool value)
    {
        this->value = value;
    }

    string getName(void)
    {
        return this->name;
    }

    bool getValue(void)
    {
        return this->value;
    }

    void AND(Node n1, Node n2)
    {
        this->value = n1.getValue() & n2.getValue();
    }
    void OR(Node n1, Node n2)
    {
        this->value = n1.getValue() | n2.getValue();
    }
    void XOR(Node n1, Node n2)
    {
        this->value = n1.getValue() ^ n2.getValue();
    }



    friend ostream& operator << (ostream& os, const Node& node);
};

ostream& operator << (ostream& Dina, const Node& node)
{
    Dina << node.name << ": " << node.value << endl;
    return Dina;
}

class Gate
{
protected:
    Node *input_a;
    Node *input_b;
    Node *output;

public:
    Gate()
    {
        // (input_a)->setName("input_a");
        // (input_b)->setName("input_b");
        // (output)->setName("output");
    }
    Gate(Node *input_a, Node *input_b, Node *output)
    {
        this->input_a = input_a;

        this->input_b = input_b;

        this->output = output;
    }

    void setInput_a(Node *input_a)
    {
        this->input_a = input_a;
    }

    void setInput_b(Node *input_b)
    {
        this->input_b = input_b;
    }

    void setOutput(Node *output)
    {
        this->output = output;
    }

    Node* getInput_a(void)
    {
        return this->input_a;
    }

    Node* getInput_b(void)
    {
        return this->input_b;
    }

    Node* getOutput(void)
    {
        return this->output;
    }

    virtual bool simulateGate()
    {
        return 0;
    }
    
};

class AND :public Gate
{
public:
    AND() :Gate()
    {}
    AND(Node *input_a, Node *input_b, Node *output) :Gate(input_a, input_b, output)
    {}
    bool simulateGate()
    {   
        this->output->AND(*(this->input_a), *(this->input_b));
        return this->output->getValue();
    }
};
class OR :public Gate
{
public:
    OR() :Gate()
    {}
    OR(Node *input_a, Node *input_b, Node *output) :Gate(input_a, input_b, output)
    {}
    bool simulateGate()
    {
        this->output->OR(*(this->input_a), *(this->input_b));
        // cout<<"val1 "<<this->input_a->getValue()<<endl;
        // cout<<"val1 "<<this->input_b->getValue()<<endl;
        return this->output->getValue();
    }
};

class XOR :public Gate
{
public:
    XOR() :Gate()
    {}
    XOR(Node *input_a, Node *input_b, Node *output) :Gate(input_a, input_b, output)
    {}
    bool simulateGate()
    {
        this->output->XOR(*(this->input_a), *(this->input_b));
        return this->output->getValue();
    }
};

class NAND :public Gate
{
public:
    NAND() :Gate()
    {}
    NAND(Node *input_a, Node *input_b, Node *output) :Gate(input_a, input_b, output)
    {}
    bool simulateGate()
    {
        this->output->AND(*(this->input_a), *(this->input_b));
        this->output->setValue(!(this->output->getValue()));
        return this->output->getValue();
    }
};

class NOR :public Gate
{
public:
    NOR() :Gate()
    {}
    NOR(Node *input_a, Node *input_b, Node *output) :Gate(input_a, input_b, output)
    {}
    bool simulateGate()
    {
        this->output->OR(*(this->input_a), *(this->input_b));
        this->output->setValue(!(this->output->getValue()));
        return this->output->getValue();
    }
};
class XNOR :public Gate
{
public:
    XNOR() :Gate()
    {}
    XNOR(Node *input_a, Node *input_b, Node *output) :Gate(input_a, input_b, output)
    {}
    bool simulateGate()
    {
        this->output->XOR(*(this->input_a), *(this->input_b));
        this->output->setValue(!(this->output->getValue()));
        return this->output->getValue();
    }

};
class NOT :public Gate
{
public:
    NOT() :Gate()
    {}
    NOT(Node *input_a, Node *input_b, Node *output) :Gate(input_a, input_b, output)
    {}
    bool simulateGate()
    {
        this->output->setValue(!(this->input_a->getValue()));
        return this->output->getValue();
    }
};

class Simulator
{

private:
    Node* n;
    Simulator() {}
    Simulator(const Simulator& other) = delete;

public:
    vector<Gate*> GateContainer;
    vector<Node*> NodeContainer;

    static Simulator& creatObj()
    {
        static Simulator object;
        return object;
    }

    void postGate(Gate* gate)
    {
        GateContainer.push_back(gate);
    }

    Node* postNode(Node* node)
    {
        Node* result;
        if(FindNode(node->getName())==nullptr)
        {
            NodeContainer.push_back(node);
            result=node;
        }
        else
        {
            result=FindNode(node->getName());
        }
        return result;
    }
    

    Node* FindNode(string name)
    {
        Node* result = nullptr;
        vector<Node*>::iterator iter;

        for (iter = NodeContainer.begin(); iter != NodeContainer.end(); ++iter)
        {
            if (name == (*iter)->getName())
            {
                result = (*iter);
            }
        }
        return result;
    }

    void startSimulate()
    {
        vector<Gate*>::iterator iter;

        for (iter = GateContainer.begin(); iter != GateContainer.end(); ++iter)
        {
            bool number= (*iter)->simulateGate();
            n = FindNode(((*iter)->getOutput())->getName());
           ((*iter)->getOutput())->setValue(number);
            n->setValue(number);

        }

    }
};

class GateGenerator
{
private:
    string input;
    Simulator& simulator = Simulator::creatObj();
    Gate* gateBase;
    Node* n;

    int stringlen(string str)  
    {  
        int length = 0;  
        
        for (int i = 0; str[i] != '\0'; i++)  
        {  
            length++;  
              
        }  
        return length;     
    }  
    vector<string> splitString (string str, char seperator)  
    {  
        int currIndex = 0, i = 0;  
        int startIndex = 0, endIndex = 0; 
        
        vector<string> splitedString;
        
        while (i <= stringlen(str))  
        {  
            if (str[i] == seperator || i == stringlen(str))  
            {  
                endIndex = i;  
                string subStr = "";  
                subStr.append(str, startIndex, endIndex - startIndex);  
                splitedString.push_back(subStr);  
                currIndex += 1;  
                startIndex = endIndex + 1;  
            }  
            i++;  
        }  
        return splitedString;
    }  
    Gate* createGate(const gateTypes id)
    {
        if (id == AND_t) 
        {
            return new AND;
        }
        else if (id == OR_t) 
        {
            return new OR;
        }
        else if (id == XOR_t) 
        {
            return new XOR;
        }
        else if (id == NAND_t) 
        {
            return new NAND;
        }
        else if (id == NOR_t) 
        {
            return new NOR;
        }
        else if (id == XNOR_t) 
        {
            return new XNOR;
        }
        else if (id == NOT_t) 
        {
            return new NOT;
        }
        return nullptr;
    }

    Node* createNode()
    {
        return new Node;
    }

    

public:
    void parseInput()
    {
        
        getline(cin, input);

        char sep =' ';
        vector<string> parsedinput;
        parsedinput = splitString(input, sep);
        

        vector<string>::iterator iter;
        iter = parsedinput.begin();
        const char* str=(*iter).c_str();
        iter++;
        
            if (strcmp(str, "AND") == 0)
            {
                gateBase = createGate(AND_t);
                simulator.postGate(gateBase);
                
                n = createNode();
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_a(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_b(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setOutput(n);
                
            }
            else if (strcmp(str, "OR") == 0)
            {
                gateBase = createGate(OR_t);
                simulator.postGate(gateBase);
                
                n = createNode();
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_a(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_b(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setOutput(n);                
            }
            else if (strcmp(str, "XOR") == 0)
            {
                gateBase = createGate(XOR_t);
                simulator.postGate(gateBase);
                
                n = createNode();
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_a(n);
                
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_b(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setOutput(n);                
                
            }
            else if (strcmp(str, "NAND") == 0)
            {
                gateBase = createGate(NAND_t);
                simulator.postGate(gateBase);
                
                n = createNode();
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_a(n);
                
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_b(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setOutput(n);                
            }
            else if (strcmp(str, "NOR") == 0)
            {
                gateBase = createGate(NOR_t);
                simulator.postGate(gateBase);
                
                n = createNode();
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_a(n);
                
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_b(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setOutput(n);                
            }
            else if (strcmp(str, "XNOR") == 0)
            {
                gateBase = createGate(XNOR_t);
                simulator.postGate(gateBase);
                
               n = createNode();
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_a(n);
                
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_b(n);
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setOutput(n);      
            }
            else if (strcmp(str, "NOT") == 0)
            {
                gateBase = createGate(NOT_t);
                simulator.postGate(gateBase);
                
                n = createNode();
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setInput_a(n);
                
                
                n = createNode();
                iter++;
                n->setName(*iter);
                n=simulator.postNode(n);
                gateBase->setOutput(n);                
            }
            
            else if (strcmp(str, "SET") == 0)
            {
                n=simulator.FindNode(*iter);
                iter++;
                bool num = stoi(*iter);
                n->setValue(num);
            }
            else if (strcmp(str, "OUT") == 0)
            {
                const char* str2=(*iter).c_str();
                
                if (strcmp(str2, "ALL") == 0)
                {
                    vector<Node*>::iterator iter2;

                    for (iter2 = simulator.NodeContainer.begin(); iter2 != simulator.NodeContainer.end(); ++iter2)
                    {
                        cout << **iter2;
                    }
                }
                else
                {
                    n = simulator.FindNode(*iter);
                    cout << *n;
                }
            }
            else if (strcmp(str, "SIM") == 0)
            {
                simulator.startSimulate();
            }
        

    }

};



int main()
{
    GateGenerator in;
    
    while (true)
    {
        if (cin.peek() != EOF)
        {
            in.parseInput();
        }

        else
        {
            break;
        }
    }
    
    return 0;
}
