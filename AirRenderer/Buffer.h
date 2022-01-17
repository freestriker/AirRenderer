#pragma once
template<class T> 
class Buffer
{
public:
	int dimensionX;
	int dimensionY;
	int dimensionZ;
	T* data = NULL;

	void SetData(T data, int x);
	void SetData(T data, int x, int y);
	void SetData(T data, int x, int y, int z);
	T GetData(int x);
	T GetData(int x, int y);
	T GetData(int x, int y, int z);

	Buffer(int x);
	Buffer(int x, int y);
	Buffer(int x, int y, int z);

	int GetIndex(int x);
	int GetIndex(int x, int y);
	int GetIndex(int x, int y, int z);

	int GetDataLength();
};

template <class T>
void Buffer<T>::SetData(T data, int x)
{
	this->SetData(data, x, 0, 0);
}
template <class T>
void Buffer<T>::SetData(T data, int x, int y)
{
	this->SetData(data, x, y, 0);
}
template <class T>
void Buffer<T>::SetData(T data, int x, int y, int z)
{
	this->data[this->GetIndex(x, y, z)] = data;
}
template <class T>
T Buffer<T>::GetData(int x)
{
	return this->GetData(x, 0, 0);
}
template <class T>
T Buffer<T>::GetData(int x, int y)
{
	return this->GetData(x, y, 0);
}
template <class T>
T Buffer<T>::GetData(int x, int y, int z)
{
	return this->data[this->GetIndex(x, y, z)];
}

template <class T>
int Buffer<T>::GetIndex(int x)
{
	return x;
}
template <class T>
int Buffer<T>::GetIndex(int x, int y)
{
	return this->dimensionX * y + x;
}
template <class T>
int Buffer<T>::GetIndex(int x, int y, int z)
{
	int m = this->dimensionX * this->dimensionY * z + this->dimensionX * y + x;
	return m;
}

template <class T>
Buffer<T>::Buffer(int x) : Buffer(x, 1, 1)
{

}
template <class T>
Buffer<T>::Buffer(int x, int y) : Buffer(x, y, 1)
{

}
template <class T>
Buffer<T>::Buffer(int x, int y, int z)
{
	this->data = new T[x * y * z]();
	this->dimensionX = x;
	this->dimensionY = y;
	this->dimensionZ = z;
}
template <class T>
int Buffer<T>::GetDataLength()
{
	return dimensionX * dimensionY * dimensionZ;
}


