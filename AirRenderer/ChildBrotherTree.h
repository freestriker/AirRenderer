#pragma once
template<class T>
class ChildBrotherTree
{
public:
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
    ChildBrotherTree<T>::AddChild(newChild);
    return newChild;
}
template<class T>
void ChildBrotherTree<T>::AddChild(T* child)
{
    static_cast<ChildBrotherTree<T>*>(child)->parent = static_cast<T*>(this);
    if (this->child)
    {
        static_cast<ChildBrotherTree<T>*>(child)->AddBrother(child);
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

