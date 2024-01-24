#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

namespace tdm {

template <typename T>
class Singleton
{
  private:
    inline static T m_root;
    inline static T *m_ptr = &m_root;

  public:
    T *ptr(void) { return m_ptr; }

    Singleton &set(std::shared_ptr<T> other)
    {
        m_ptr = other.get();
        return *this;
    }

    Singleton &reset(void)
    {
        m_ptr = &m_root;
        return *this;
    }

    T *operator->(void) { return m_ptr; }
};

}; // namespace tdm

#endif /* SINGLETON_H */
