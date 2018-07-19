#include <iostream>
#include <vector>
#define NOTFOUND -1

using namespace std;

class SimpleNode;
class SimpleBST;

ostream & operator << (ostream &stream, SimpleNode &node);
ostream & operator << (ostream &stream, SimpleBST &tree);

class SimpleNode {
public:
	void Print(ostream &stream) {
		if (LChild != nullptr || RChild != nullptr) {
			if (LChild != nullptr) {
				stream << "Ű:" << key << "�� LC�� "<< *LChild;
			}
			else {
				stream << "Ű:" << key << "�� LC�� ����" << endl;
			}
			if (RChild != nullptr) {
				stream << "Ű:" << key << "�� RC�� "<< *RChild;
			}
			else {
				stream << "Ű:" << key << "�� RC�� ����" << endl;
			}
stream << endl;
		}
		else {
			stream << "Ű:" << key << "�� ����" << endl;
		}
	}
private:
	int key;
	SimpleNode* LChild;
	SimpleNode* RChild;
	SimpleNode() {
		initNode();
	}
	SimpleNode(int nodekey) {
		key = nodekey;
		initNode();
	}
	~SimpleNode() {
	}
	bool insert(SimpleNode* v) {
		if (key == v->key) {
			return false;
		}
		SimpleNode** insertDstNodePtr = nullptr;
		bool isSuccess;
		if (key < v->key) {
			insertDstNodePtr = &RChild;
		}
		else if (key > v->key) {
			insertDstNodePtr = &LChild;
		}
		if (*insertDstNodePtr == nullptr) {
			*insertDstNodePtr = v;
			isSuccess = true;
		}
		else {
			isSuccess = (*insertDstNodePtr)->insert(v);
		}
		return isSuccess;
	}
	void initNode() {
		LChild = nullptr;
		RChild = nullptr;
	}
	bool isLeaf() {
		return LChild == nullptr && RChild == nullptr;
	}
	/*
	bool operator == (const SimpleNode& src) { return key == src.key; }
	bool operator < (const SimpleNode& src) { return key < src.key; }
	bool operator > (const SimpleNode& src) { return key > src.key; }*/
	friend class SimpleBST;
};

class SimpleBST {
public:
	SimpleBST() {
		nodes.push_back(nullptr);
	}
	~SimpleBST() {
	}
	bool isEmpty() {
		return nodes[0] == nullptr;
	}
	bool insert(int v) {
		bool isSuccess;
		SimpleNode* nodeToInsert = new SimpleNode(v);
		if (isEmpty()) {
			nodes[0] = nodeToInsert;
			isSuccess = true;
		}
		else {
			bool insertSuccess = nodes[0]->insert(nodeToInsert);
			if (!insertSuccess) {
				delete nodeToInsert;
				isSuccess = false;
			}
			else {
				isSuccess = true;
			}
		}
		return isSuccess;
	}
	SimpleNode* Search(int searchKey) {
		SimpleNode* result = nullptr;
		if (saveDecentPath(searchKey)) {
			result = nodes.back();
		}
		nodesClear();
		return result;
	}
	bool remove(int key) {
		bool isSuccess;
		if (saveDecentPath(key)) {
			SimpleNode* dstNode = nodes.back();
			if (dstNode != nullptr) isSuccess = remove(dstNode);
		}
		else {
			isSuccess = false;
		}		
		nodesClear();
		return isSuccess;
	}
	
	void Print(ostream &stream) {
		if (!isEmpty()) stream << (*nodes[0]);
		else stream << "�� Ʈ����" << endl;
	}
private:
	vector<SimpleNode*> nodes;
	bool remove(SimpleNode* dstNode) {
		if (dstNode == nullptr) {
			return false;
		}
		bool isSuccess = false;
		while (dstNode->LChild != nullptr && dstNode->RChild != nullptr) {
			SimpleNode* leastNodeAtRChild = findLeastNodeAtRChild(dstNode);
			dstNode->key = leastNodeAtRChild->key;
			dstNode = leastNodeAtRChild;
		}
		int level = nodes.size() - 1;
		isSuccess = (level > 0)? parentWork() : rootWork();
		delete dstNode;
		return isSuccess;
	}

	SimpleNode* findLeastNodeAtRChild(SimpleNode* dstNode) {
		SimpleNode* currentNode = dstNode->RChild;
		nodes.push_back(currentNode);
		while (currentNode->LChild != nullptr) {
			currentNode = currentNode->LChild;
			nodes.push_back(currentNode);
		}
		return currentNode;
	}
	void nodesClear() {
		SimpleNode* root = nodes[0];
		nodes.clear();
		nodes.push_back(root);
	}
	bool saveDecentPath(int key) {
		int curLevel = 0;
		SimpleNode* current = nodes[0];
		bool result = false;
		while (current != nullptr) {
			if (key == current->key) {
				result = true;
				break;
			}
			else {
				SimpleNode** dstNode = nullptr;
				if (key < current->key) {
					dstNode = &(current->LChild);
				}
				else if (key > current->key) {
					dstNode = &(current->RChild);
				}
				if (*dstNode != nullptr) {
					current = *dstNode;
					curLevel++;
					nodes.push_back(current);
				}
				else {
					nodesClear();
					break;
				}
			}			
		}
		return result;
	}
	bool parentWork() {
		//�� �Լ��� ���Դٴ� �� ������尡 �����ų� �ڽ��� �ϳ���� �Ҹ���.
		int level = nodes.size() - 1;
		bool isSuccess;
		if (level > 0) {
			SimpleNode* dstNode = nodes[level];
			SimpleNode* parent = nodes[level - 1];			
			SimpleNode** childToUpdatePtr = (parent->LChild == dstNode) ?
				&(parent->LChild) : &(parent->RChild);
			if (dstNode->isLeaf()) {
				*childToUpdatePtr = nullptr;
			}
			else if (dstNode->LChild == nullptr) {
				*childToUpdatePtr = dstNode->RChild;
			}
			else if (dstNode->RChild == nullptr) {
				*childToUpdatePtr = dstNode->LChild;
			}
			else {
				cout << "�׷����� ����. ���� �̻��ϴ�" << endl;
				return false;

			}
			isSuccess = true;
		}
		else {
			isSuccess = false;
		}
		return isSuccess;
	};
	bool rootWork() {
		//�� �Լ��� ���Դٴ� �� ��Ʈ��尡 �����ų� �ڽ��� �ϳ���� �Ҹ���.
		bool isSuccess;
		int level = nodes.size() - 1;		
		if (level == 0) {
			if (nodes[0]->LChild == nullptr) {
				nodes[0] = nodes[0]->RChild;
			}
			else if (nodes[0]->RChild == nullptr) {
				nodes[0] = nodes[0]->LChild;
			}
			else if (nodes[0]->isLeaf()) {
				//�ƹ��͵� ���Ѵ�.
			}
			else {
				cout << "�׷����� ����." << endl;
				return false;
			}
			isSuccess = true;
		}
		else {
			isSuccess = false;
		}
		return isSuccess;
		
	}
};

ostream & operator << (ostream &stream, SimpleNode &node) {
	node.Print(stream);
	return stream;
}
ostream & operator << (ostream &stream, SimpleBST &tree) {
	tree.Print(stream);
	return stream;
}