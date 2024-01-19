#ifndef SINGLETON_H
#define SINGLETON_H

namespace tdm {

template <typename T>
class Singleton
{
  private:
    inline static T m_root;
    inline static T &m_ref = m_root;

  public:
    T &ref(void) { return m_ref; }

    T &set(T &other)
    {
        m_ref = other;
        return ref();
    }

    T &reset(void)
    {
        m_ref = m_root;
        return ref();
    }

    T *operator->(void) { return &m_ref; }
};

}; // namespace tdm

#endif /* SINGLETON_H */
