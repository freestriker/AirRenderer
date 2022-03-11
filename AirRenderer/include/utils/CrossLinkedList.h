#pragma once
class CrossLinkedNode
{
public:
	CrossLinkedNode* left;
	CrossLinkedNode* right;
	CrossLinkedNode* top;
	CrossLinkedNode* down;
	CrossLinkedNode()
	{
		left = nullptr;
		right = nullptr;
		top = nullptr;
		down = nullptr;
	}
	virtual ~CrossLinkedNode()
	{
		left = nullptr;
		right = nullptr;
		top = nullptr;
		down = nullptr;
	}
	virtual void OnRemove()
	{
	};
	virtual void OnAdd()
	{
	};
};
class CrossLinkedNodeRowItertor
{
private:
	CrossLinkedNode* curNode;
public:
	CrossLinkedNodeRowItertor(CrossLinkedNode* firstNode)
	{
		curNode = firstNode;
	}
	~CrossLinkedNodeRowItertor()
	{
		curNode = nullptr;
	}
	bool IsVaild()
	{
		return curNode != nullptr;
	}
	CrossLinkedNodeRowItertor operator++(int)
	{
		curNode = curNode->right;
		return *this;
	}
	CrossLinkedNodeRowItertor operator++()
	{
		curNode = curNode->right;
		return *this;
	}
	template<typename T>
	T* Node() { return static_cast<T*>(curNode); }

};
class CrossLinkedRowHead: public CrossLinkedNode
{
private:
	CrossLinkedNode* end;
public:
	CrossLinkedRowHead():CrossLinkedNode()
	{
		this->end = nullptr;
	}
	virtual ~CrossLinkedRowHead()
	{
		this->end = nullptr;
	}
	void Init()
	{
		this->end = this;
	}
	template<typename T>
	void AddNode(T* node)
	{
		static_assert(std::is_base_of< CrossLinkedNode, T>::value, "AddNode can only add the sub class of CrossLinkedNode.");
		CrossLinkedNode* newNode = static_cast<CrossLinkedNode*>(node);
		newNode->right = nullptr;
		end->right = newNode;
		newNode->left = end;

		end = newNode;
		newNode->OnAdd();
	}
	template<typename T>
	void RemoveNode(T* node)
	{
		static_assert(std::is_base_of< CrossLinkedNode, T>::value, "RemoveNode can only remove the sub class of CrossLinkedNode.");
		CrossLinkedNode* removeNode = static_cast<CrossLinkedNode*>(node);
		CrossLinkedNode* start = this;
		while (start != end)
		{
			if (start->right == removeNode)
			{
				if (start->right == end)
				{
					start->right = nullptr;
					end = start;
					removeNode->left = nullptr;
				}
				else
				{
					start->right = removeNode->right;
					removeNode->right->left = start;
					removeNode->left = nullptr;
					removeNode->right = nullptr;
				}
				removeNode->OnRemove();
				return;
			}
			start = start->right;
		}
	}
	CrossLinkedNodeRowItertor GetItertor()
	{
		return CrossLinkedNodeRowItertor(this->right);
	}
	//CrossLinkedNode* RemoveNode(int index)
	//{
	//	CrossLinkedNode* start = this;
	//	int i = -1;
	//	while (start != end)
	//	{
	//		if (++i == index)
	//		{
	//			CrossLinkedNode* removeNode = start->right;
	//			if (removeNode == end)
	//			{
	//				start->right = nullptr;
	//				end = start;
	//				removeNode->left = nullptr;
	//			}
	//			else
	//			{
	//				start->right = removeNode->right;
	//				removeNode->right->left = start;
	//				removeNode->left = nullptr;
	//				removeNode->right = nullptr;
	//			}
	//			return removeNode;
	//		}
	//		start = start->right;
	//	}
	//	return nullptr;
	//}
};
class CrossLinkedNodeColItertor
{
private:
	CrossLinkedNode* curNode;
public:
	CrossLinkedNodeColItertor(CrossLinkedNode* firstNode)
	{
		curNode = firstNode;
	}
	~CrossLinkedNodeColItertor()
	{
		curNode = nullptr;
	}
	bool IsVaild()
	{
		return curNode != nullptr;
	}
	CrossLinkedNodeColItertor operator++(int)
	{
		curNode = curNode->down;
		return *this;
	}
	CrossLinkedNodeColItertor operator++()
	{
		curNode = curNode->down;
		return *this;
	}
	template<typename T>
	T* Node() { return static_cast<T*>(curNode); }

};
class CrossLinkedColHead: public CrossLinkedNode
{
private:
	CrossLinkedNode* end;
public:
	CrossLinkedColHead():CrossLinkedNode()
	{
		end = this;
	}
	virtual ~CrossLinkedColHead()
	{
		end = nullptr;
	}
	template<typename T>
	void AddNode(T* node)
	{
		static_assert(std::is_base_of< CrossLinkedNode, T>::value, "AddNode can only add the sub class of CrossLinkedNode.");
		CrossLinkedNode* newNode = static_cast<CrossLinkedNode*>(node);
		newNode->down = nullptr;
		end->down = newNode;
		newNode->top = end;

		end = newNode;
		newNode->OnAdd();
	}
	template<typename T>
	void RemoveNode(T* node)
	{
		static_assert(std::is_base_of< CrossLinkedNode, T>::value, "RemoveNode can only remove the sub class of CrossLinkedNode.");
		CrossLinkedNode* removeNode = static_cast<CrossLinkedNode*>(node);
		CrossLinkedNode* start = this;
		while (start != end)
		{
			if (start->down == removeNode)
			{
				if (start->down == end)
				{
					start->down = nullptr;
					end = start;
					removeNode->top = nullptr;
				}
				else
				{
					start->down = removeNode->down;
					removeNode->down->top = start;
					removeNode->top = nullptr;
					removeNode->down = nullptr;
				}
				removeNode->OnRemove();
				return;
			}
			start = start->down;
		}
	}
	CrossLinkedNodeColItertor GetItertor()
	{
		return CrossLinkedNodeColItertor(this->down);
	}
};