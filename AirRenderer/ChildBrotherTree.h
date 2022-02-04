#pragma once
template<class T>
class ChildBrotherTree
{

public:
    struct BrotherIterator {
        ChildBrotherTree<T>* node;

    public:
        BrotherIterator(T* node)
        {
            if (node && static_cast<ChildBrotherTree<T>*>(node)->parent && static_cast<ChildBrotherTree<T>*>(static_cast<ChildBrotherTree<T>*>(node)->parent)->child)
            {
                this->node = static_cast<ChildBrotherTree<T>*>(static_cast<ChildBrotherTree<T>*>(node)->parent)->child;
            }
            else
            {
                this->node = nullptr;
            }
        }
        BrotherIterator& operator++() {
            node = node->brother;
            return *this;
        }
        BrotherIterator operator++(int) {
            node = node->brother;
            BrotherIterator ret_val = BrotherIterator(static_cast<T*>(node));
            return ret_val;
        }
        bool operator==(BrotherIterator other) const { return node == other.node; }
        bool operator!=(BrotherIterator other) const { return node != other.node; }
        T operator*() { return *static_cast<T*>(node); }
    };
    struct ChildIterator {
        ChildBrotherTree<T>* node;

    public:
        ChildIterator(T* node)
        {
            if (node)
            {
                this->node = static_cast<ChildBrotherTree<T>*>(node)->child;
            }
            else
            {
                this->node = nullptr;
            }
        }
        ChildIterator& operator++() {
            node = node->brother;
            return *this;
        }
        ChildIterator operator++(int) {
            node = node->brother;
            ChildIterator ret_val = ChildIterator(static_cast<T*>(node));
            return ret_val;
        }
        bool operator==(ChildIterator other) const { return node == other.node; }
        bool operator!=(ChildIterator other) const { return node != other.node; }
        T operator*() { return *static_cast<T*>(node); }
    };

    T* parent;
	T* child;
	T* brother;

	ChildBrotherTree(T* parent, T* child, T* brother);

	ChildBrotherTree(T* parent);
	ChildBrotherTree();

	T* AddChild();
	void AddChild(T* child);
	T* AddBrother();
	void AddBrother(T* brother);

    ChildIterator GetStartChildIterator()
    {
        return ChildIterator(static_cast<T*>(this));
    }
    ChildIterator GetEndChildIterator()
    {
        return ChildIterator(nullptr);
    }
    BrotherIterator GetStartBrotherIterator()
    {
        return BrotherIterator(static_cast<T*>(this));
    }
    BrotherIterator GetEndBrotherIterator()
    {
        return BrotherIterator(nullptr);
    }
};
template<class T>
ChildBrotherTree<T>::ChildBrotherTree() :ChildBrotherTree(nullptr, nullptr, nullptr)
{

}
template<class T>
ChildBrotherTree<T>::ChildBrotherTree(T* parent, T* child, T* brother)
{
    this->parent = parent;
    this->child = child;
    this->brother = brother;
}
template<class T>
ChildBrotherTree<T>::ChildBrotherTree(T* parent) :ChildBrotherTree(parent, nullptr, nullptr)
{

}

template<class T>
T* ChildBrotherTree<T>::AddChild()
{
    T* newChild = new T();
    this->AddChild(newChild);
    return newChild;
}
template<class T>
void ChildBrotherTree<T>::AddChild(T* child)
{
    static_cast<ChildBrotherTree<T>*>(child)->parent = static_cast<T*>(this);
    if (this->child)
    {
        static_cast<ChildBrotherTree<T>*>(this->child)->AddBrother(child);
    }
    else
    {
        this->child = child;
    }
}

template<class T>
T* ChildBrotherTree<T>::AddBrother()
{
    T* newBrother = new T();
    ChildBrotherTree<T>::AddBrother(newBrother);
    return newBrother;
}
template<class T>
void ChildBrotherTree<T>::AddBrother(T* brother)
{
    static_cast<ChildBrotherTree<T>*>(brother)->parent = this->parent;
    ChildBrotherTree<T>* b = this->brother;
    if (b)
    {
        while (b->brother)
        {
            b = b->brother;
        }
        b->brother = brother;
    }
    else
    {
        this->brother = brother;
    }
}

