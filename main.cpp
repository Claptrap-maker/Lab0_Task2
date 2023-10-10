#include <iostream>
#include<fstream>
#include <string>
#include <cctype>
#include <sstream>
#include <cmath>

template <typename T>
class Node {
public:
    Node* next;
    T info;
    explicit Node(T info = T(), Node* next = nullptr) {
        this->info = info;
        this->next = next;
    }
};

template <typename T>
class SinglyLinkedList {
protected:
    int size;
    Node<T>* head;
public:
    SinglyLinkedList();

    int GetSize();

    void PushFront(T value);

    virtual T Pop();

    virtual T Top() = 0;

    void RemoveAll();

    bool IsEmpty();

    SinglyLinkedList(const SinglyLinkedList<T>& l);

    SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& l);

    ~SinglyLinkedList();
};


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(){
    size = 0;
    head = nullptr;
}

template <typename T>
int SinglyLinkedList<T>::GetSize() {
    return size;
}

template <typename T>
void SinglyLinkedList<T>::PushFront(T info) {
    if (head == nullptr) {
        head = new Node<T>(info);
    }
    else {
        Node<T>* tempNode = head;
        head = new Node<T>(info, tempNode);
    }
    size++;
}

template <typename T>
T SinglyLinkedList<T>::Pop() {
    if (this->IsEmpty()) {
        std::cout << "Stack is empty" << std::endl;
        return 0;
    }
    else {
        Node<T>* tempNode = this->head;
        T info = head->info;
        head = head->next;
        delete tempNode;
        size--;
        return info;
    }
}

template <typename T>
void SinglyLinkedList<T>::RemoveAll() {
    while (size != 0) {
        Pop();
    }
}

template <typename T>
bool SinglyLinkedList<T>::IsEmpty() {
    return head == nullptr;
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& l){
    size = l.size;
    head = nullptr;
    Node<T>* n1 = l.head;
    Node<T>* n2 = nullptr;
    while (n1 != nullptr) {
        if (head == nullptr) {
            head = new Node<T>(n1->info);
            n2 = head;
        }
        else {
            n2->next = new Node<T>(n1->info);
            n2 = n2->next;
        }
        n1 = n1->next;
    }
}

template<typename T>
SinglyLinkedList<T> &SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& l) {
    if (this == &l) {
        return *this;
    }
    RemoveAll();
    size = l.size;
    head = nullptr;
    Node<T>* n1 = l.head;
    Node<T>* n2 = nullptr;
    while (n1 != nullptr) {
        if (head == nullptr) {
            head = new Node<T>(n1->info);
            n2 = head;
        }
        else {
            n2->next = new Node<T>(n1->info);
            n2 = n2->next;
        }
        n1 = n1->next;
    }
    return *this;
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList(){
    if (IsEmpty()) {
        delete head;
    }
    else {
        RemoveAll();
    }
}

template<typename T>
class Stack : public SinglyLinkedList<T> {
public:
    Stack(): SinglyLinkedList<T>(){}

    T Top() override;

    Stack(const Stack<T>& l): SinglyLinkedList<T>(l){}

    Stack& operator = (const Stack& l);

    ~Stack();
};

template <typename T>
T Stack<T>::Top() {
    if (this->IsEmpty()){
        return 0;
    } else {
        return this->head->info;
    }
}

template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &l) {
    SinglyLinkedList<T>::operator=(l);
}

template <typename T>
Stack<T>::~Stack() {
    SinglyLinkedList<T>::RemoveAll();
}

int GetPriority(char ch) {
    switch (ch) {
        case '(': return 1;
        case '+': case '-': return 2;
        case '*': case '/': case '%': return 3;
        case '^': return 4;
        default: return 0;
    }
}

int Count(std::string& strOut) {
    Stack<int> resultStack;
    std::string tempString{};
    int value, num1, num2, res;
    for (size_t i = 0; i < strOut.size(); i++) {
        while ((strOut[i] != ' ') && (isdigit(strOut[i]))) {
            tempString += strOut[i];
            i++;
        }
        if (!tempString.empty()) {
            value = stoi(tempString);
            resultStack.PushFront(value);
            tempString.clear();
        }
        if (strOut[i] != ' ') {
            num1 = resultStack.Pop();
            num2 = resultStack.Pop();
            switch (strOut[i]) {
                case '+': res = num1 + num2; break;
                case '-': res = num2 - num1; break;
                case '*': res = num1 * num2; break;
                case '/': res = num2 / num1; break;
                case '%': res = num2 % num1; break;
                case '^': res = pow(num2, num1); break;
                default: throw (std::string)"Error";
            }
            resultStack.PushFront(res);
        }

    }
    return resultStack.Pop();
}

std::string ReversePolishNotation(const std::string& str) {
    int isOp = 0, np = 0;
    bool isDigit = false;
    Stack<char> operationStack;
    std::string strIn = str;
    std::string strOut{};
    if ((!isdigit(strIn[0])) && strIn[0] != '(') {
        throw (std::string) str + " " + "Expression should start from num or opening bracket";
    }
    for (char ch : strIn) {
        if (isdigit(ch)) {
            strOut += ch;
            isOp = 0;
            isDigit = true;
        }
        else strOut += ' ';
        if (!isDigit) {
            switch (ch) {
                case '(':
                    operationStack.PushFront(ch);
                    ++np;
                    isOp = 0;
                    break;
                case '*': case '/': case '+': case '-': case '^': case '%':
                    if (!isOp) {
                        isOp = 1;
                        while (GetPriority(ch) <= GetPriority(operationStack.Top())) {
                            strOut += operationStack.Pop();
                            strOut += ' ';
                        }
                        if (GetPriority(ch) > GetPriority(operationStack.Top())) {
                            operationStack.PushFront(ch);
                        }
                        break;
                    }
                    else {
                        throw (std::string) str + " " + "Several operations in a row";
                    }

                case ')':
                    if (isOp) {
                        throw (std::string) str + " " + "Opening bracket not found";
                    }
                    else {
                        while ((ch = operationStack.Pop()) != '(' && np > 0) {

                            strOut += ch;
                            strOut += ' ';
                        }
                        --np;
                    }
                    break;

                case ' ':
                    break;

                default: {
                    throw (std::string)str + " " + "Incorrect symbol in string";
                }
            }
        }
        else {
            isDigit = false;
        }
    }
    while (operationStack.GetSize() != 0) {
        strOut += ' ';
        strOut += operationStack.Pop();
    }
    if (np) {
        throw (std::string) str + " " + "Incorrect count of brackets";
    }
    return strOut;
}

int errorFileIndex = 0;

std::string GenerateErrorFileName(std::string& inputFile) {
    std::stringstream fileNameStream;
    fileNameStream << "" << inputFile << ' ' << errorFileIndex++ << ".txt";
    std::string fileName = fileNameStream.str();
    return fileName;
}

void PrintToErrorFile(std::string& exception, int lineCounter, std::string& input, std::string& out) {
    std::ofstream targetFile(out, std::ios::app);
    if (!targetFile.is_open()) {
        std::cerr << "Error creating file " << out << "." << std::endl;
    }
    else {
        targetFile << lineCounter << std::endl;
        targetFile << exception << std::endl;
    }
    targetFile.close();
}

void WorkWithFiles(std::string& input) {
    int lineCounter = 0;
    std::ifstream inputFile(input);
    bool hasException = false;
    std::string strOut{};
    std::string errorFileName = GenerateErrorFileName(input);
    while (!hasException) {
        if (!inputFile.is_open()) {
            hasException = true;
            std::cerr << "Error with opening file\n";
        }
        try {
            std::string line;
            while (std::getline(inputFile, line)) {
                try {
                    strOut = ReversePolishNotation(line);
                    std::cout << "Given string: " << line << std::endl;
                    std::cout << "Postfix form: " << strOut << std::endl;
                    std::cout << "Result: " << Count(strOut) << std::endl;
                }
                catch (std::string exception) {
                    PrintToErrorFile(exception, lineCounter, input, errorFileName);
                }
                lineCounter++;
            }
            hasException = true;
        }
        catch (std::string ex) {
            std::cerr << "hasException: " << ex << '\n';
            hasException = true;
        }
    }
    inputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Enter the name of file...";
    } else {
        for (size_t i = 1; i < argc; i++){
            std::string filename = argv[i];
            WorkWithFiles(filename);
        }
    }
}