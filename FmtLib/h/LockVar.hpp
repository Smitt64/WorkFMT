#ifndef LOCKVAR_HPP
#define LOCKVAR_HPP

template<class T>
class LockedVar
{
public:
    LockedVar() :
        m_isLocked(false)
    {

    }

    void lock()
    {
        m_isLocked = true;
    }

    void unlock()
    {
        m_isLocked = false;
    }

    operator T()
    {
        return m_var;
    }

    LockedVar &operator=(const T &value)
    {
        if (!m_isLocked)
            m_var = value;

        return *this;
    }

private:
    bool m_isLocked;
    T m_var;
};

#endif // LOCKVAR_HPP
