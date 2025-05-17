#pragma once
#include <memory>

template<typename T>
class weak_ptr {
	public:
	    explicit weak_ptr(std::weak_ptr<T> ptr) : m_Ptr(ptr) {};
	    weak_ptr() = default;

	    T* operator->() {
	        m_LockedPtr = m_Ptr.lock();
	        return m_LockedPtr.get();
	    }

	    explicit operator bool() const {
	        return !m_Ptr.expired();
	    }

	private:
	    std::weak_ptr<T> m_Ptr;
	    mutable std::shared_ptr<T> m_LockedPtr;

	};