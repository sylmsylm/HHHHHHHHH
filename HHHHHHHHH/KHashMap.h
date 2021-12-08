#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#define NULL 0

template<class _Ty>
struct node {
    _Ty     value;
    node*   next;

    node() {
        next = NULL;
    }

    node(const _Ty& _value) : value(_value){
        next = NULL;
    }
};

template <class _Ty1, class _Ty2>
struct pair {
    using first_type  = _Ty1;
    using second_type = _Ty2;

    first_type  first;
    second_type second;

    pair() {}

    pair(const pair& Other) {
        first   = Other.first;
        second  = Other.second;
    }

    pair(const _Ty1& _first, const _Ty2& _second) {
        first = _first;
        second = _second;
    }
};

template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
class KHashMap {
public:
    using hasher            = _Hasher;
    using key_type          = _Kty;
    using mapped_type       = _Ty;
    using key_equal         = _Keyeq;
    using value_type        = pair<_Kty, _Ty>;

    KHashMap() {
        m_nBucketSize = m_nValueCount = 0;
        m_ppList = NULL;
    }
    ~KHashMap() {}

    KHashMap(const KHashMap&) = delete;

    KHashMap& operator=(const KHashMap& o) {
        Clear();
        if (!_Resize(o.m_nBucketSize)) {
            return *this;
        }
        m_nValueCount = 0;
        for (int i = 0; i < o.m_nBucketSize; ++i) {
            for (node<value_type>* p = o.m_ppList[i]; p; p = p->next) {
                node<value_type>* tmp = new node<value_type>(value_type(p->value));
                if (!tmp) {
                    return *this;
                }
                tmp->next = m_ppList[i], m_ppList[i] = tmp, m_nValueCount++;
            }
        }
        std::cout << m_nValueCount << ' ' << o.m_nValueCount << ' ' << m_nBucketSize << ' ' << o.m_nBucketSize << '\n';
        return *this;
    }

    KHashMap(const KHashMap&& _o) {
        *this = std::move(_o);
        return this;
    }

    BOOL Init(int n) {
        m_nBucketSize = _GetN(n);
        m_ppList = (node<value_type>**)calloc(m_nBucketSize, sizeof(node<value_type>*));
        return m_ppList ? TRUE : FALSE;
    }

    BOOL Insert(const key_type& Key, const mapped_type& Value) {
        if (!_Resize(m_nValueCount + 1)) return FALSE;
        size_t x = hash(Key) & (m_nBucketSize - 1ull);
        for (node<value_type>* p = m_ppList[x]; p; p = p->next) {
            if (equals(p->value.first, Key)) {
                return FALSE;
            }
        }
        node<value_type>* p = new node<value_type>(value_type(Key, Value));
        return p ? p->next = m_ppList[x], m_ppList[x] = p, ++m_nValueCount, TRUE : FALSE;
    }

    BOOL Modify(const key_type& Key, const mapped_type& Value) {
        size_t x = hash(Key) & (m_nBucketSize - 1ull);
        for (node<value_type>* p = m_ppList[x]; p; p = p->next) {
            if (equals(p->value.first, Key)) {
                return p->value.second = Value, TRUE;
            }
        }
        return FALSE;
    }

    BOOL Delete(const key_type& Key) {
        size_t x = hash(Key) & (m_nBucketSize - 1ull);
        node<value_type>* pre = m_ppList[x];
        for (node<value_type>* p = m_ppList[x]; p; pre = p, p = p->next) {
            if (equals(p->value.first, Key)) {
                p == m_ppList[x] ? m_ppList[x] = p->next : pre->next = p->next;
                return delete(p), p = NULL, TRUE;
            }
        }
        return FALSE;
    }

    mapped_type* GetValue(const key_type& Key) {
        size_t x = hash(Key) & (m_nBucketSize - 1ull);
        for (node<value_type>* p = m_ppList[x]; p; p = p->next) {
            if (equals(p->value.first, Key)) {
                return &(p->value.second);
            }
        }
        return NULL;
    }

    void Clear() {
        for (int i = 0; i < m_nBucketSize; ++i) {
            for (node<value_type>* p = m_ppList[i]; p;) {
                node<value_type>* tmp = p->next;
                delete(p), p = tmp;
            }
            m_ppList[i] = NULL;
        }
        m_nValueCount = 0;
    }

    void UnInit() {
        Clear();
        if (m_ppList) {
            free(m_ppList);
            m_ppList = NULL;
        }
    }

private:
    int _GetN(int n) {
        int cnt = 0, t = n;
        while (t) {
            t >>= 1, ++cnt;
        }
        return (1 << (cnt - 1)) == n ? n : 1 << cnt;
    }

    BOOL _Resize(int n) {
        if (!m_ppList || !m_nBucketSize) {
            if (!Init(n)) return FALSE;
        }
        int old = m_nBucketSize;
        m_nBucketSize = _GetN(n);
        if (m_nBucketSize == old) {
            return TRUE;
        }
        if (m_nBucketSize > old) {
            node<value_type>** ppold = m_ppList;
            ppold = (node<value_type>**)realloc(ppold, m_nBucketSize * sizeof(node<value_type>*));
            if (!ppold) {
                return FALSE;
            }
            m_ppList = ppold;
            for (int i = old; i < m_nBucketSize; ++i) {
                m_ppList[i] = NULL;
            }
        }
        for (int i = 0; i < old; ++i) {
            node<value_type>* f = m_ppList[i];
            node<value_type>* tmp = NULL;
            node<value_type>* pre = m_ppList[i];
            while (f) {
                size_t x = hash(f->value.first) & (m_nBucketSize - 1ull);
                if (x == i) pre = f, f = f->next;
                else {
                    tmp = f->next;
                    f == m_ppList[i] ? m_ppList[i] = tmp : pre->next = f->next;
                    f->next = m_ppList[x];
                    m_ppList[x] = f;
                    f = tmp;
                }
            }
        }
        if (m_nBucketSize < old) {
            node<value_type>** ppold = m_ppList;
            ppold = (node<value_type>**)realloc(ppold, m_nBucketSize * sizeof(node<value_type>*));
            if (!ppold) {
                return FALSE;
            }
            m_ppList = ppold;
        }
        return TRUE;
    }

    hasher hash;
    key_equal equals;

    int                         m_nBucketSize;
    int                         m_nValueCount;
    node<value_type>**          m_ppList;
};

#endif
