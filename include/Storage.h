#ifndef STORAGE_H
#define STORAGE_H

template <typename T>
class Storage
{
private:
    T m_data[100];
    int m_count;

public:
    Storage();
    bool add(const T& item);
    bool removeById(int id);
    T* findById(int id);
    const T* findById(int id) const;
    T* getAll();
    const T* getAll() const;
    int size() const;
    void clear();
};

template <typename T>
Storage<T>::Storage()
{
    m_count = 0;
}

template <typename T>
bool Storage<T>::add(const T& item)
{
    if (m_count >= 100)
    {
        return false;
    }

    m_data[m_count] = item;
    m_count++;
    return true;
}

template <typename T>
bool Storage<T>::removeById(int id)
{
    int index = -1;
    for (int i = 0; i < m_count; i++)
    {
        if (m_data[i].getId() == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        return false;
    }

    for (int i = index; i < m_count - 1; i++)
    {
        m_data[i] = m_data[i + 1];
    }

    m_count--;
    return true;
}

template <typename T>
T* Storage<T>::findById(int id)
{
    for (int i = 0; i < m_count; i++)
    {
        if (m_data[i].getId() == id)
        {
            return &m_data[i];
        }
    }
    return nullptr;
}

template <typename T>
const T* Storage<T>::findById(int id) const
{
    for (int i = 0; i < m_count; i++)
    {
        if (m_data[i].getId() == id)
        {
            return &m_data[i];
        }
    }
    return nullptr;
}

template <typename T>
T* Storage<T>::getAll()
{
    return m_data;
}

template <typename T>
const T* Storage<T>::getAll() const
{
    return m_data;
}

template <typename T>
int Storage<T>::size() const
{
    return m_count;
}

template <typename T>
void Storage<T>::clear()
{
    m_count = 0;
}

#endif