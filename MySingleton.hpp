#include <memory>
#include <mutex>

template<typename _SingletonCandidate>
class CSingleton;

// Singleton candidate tag
template<typename _SingletonCandidate>
class CSingletonTag
{
    friend _SingletonCandidate;
    friend class CSingleton<_SingletonCandidate>;
    private:
        enum TYPE
        {
            SINGLE
        };

};

// Utility class to create singletons.
template < typename _SingletonCandidate>
class CSingleton
{
public:

    //type to access the singleton candidate
    typedef std::shared_ptr<_SingletonCandidate> Type_T;
    //type to store the singleton candidate instance
    typedef std::weak_ptr<_SingletonCandidate>   InstanceType_T;

private:
    // Instance of singleton candidate
    static InstanceType_T m_pSingleInstance;
    // mutex for thread safety
    static std::once_flag m_mutexCreate;

    // Non copyable instance
    CSingleton();
    CSingleton(const CSingleton&);
    CSingleton& operator=(const CSingleton&);

protected:

    // The class can be instantiated by passing the secret tag
    CSingleton(typename CSingletonTag<_SingletonCandidate>::TYPE tag)
    {

    }

public:
    //Create singleton instance
    template<typename... Args>
    static Type_T GetInstance(Args... args)
    {
        if(m_pSingleInstance.use_count() == 0)
        {
            std::call_once(m_mutexCreate,
                            [&, args...](){
                                    m_pSingleInstance = std::make_shared<_SingletonCandidate>(CSingletonTag<_SingletonCandidate>::SINGLE, args...);
                                }
                           );
        }
        // return shared pointer
        return m_pSingleInstance.lock();
    }

};

// definition of the static member to store the singleton instance
template<typename _SingletonCandidate>
typename CSingleton< _SingletonCandidate>::InstanceType_T
CSingleton< _SingletonCandidate>::m_pSingleInstance;

// definition of the static member mutex
template<typename _SingletonCandidate>
std::once_flag CSingleton< _SingletonCandidate>::m_mutexCreate;
