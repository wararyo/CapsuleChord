// 順番を区別しない、ユニークであることが保証される可変長配列
template<class T>
class Set {
  protected:
    T *_data;
    int _count;
    int _size;
    char (*_validator)(T); 
    T (*_initializer)();
    char (*_comparator)(T,T);
  public:
    Set(int size,T (*initializer)(),char (*validator)(T), char (*comparator)(T,T)) {
      _data = new T[size];
      _size = size;
      _count = 0;
      _validator = validator;
      _initializer = initializer;
      _comparator = comparator;
      for(int i = 0;i < _size;i++) _data[i] = _initializer();//初期化
    }
    inline int count();
    inline int size();
    char Add(T item);
    char Contains(T item);
    char Remove(T value);

    class Iterator {
      private:
        int cursor;
        int currentCount;
        Set<T> *parent;
      public:
        Iterator(Set<T> *parent) {
          this->parent = parent;
          cursor = -1;
          currentCount = 0;
        }
        char hasNext();
        T next();
        T *nextPtr();
    };
    Iterator getIterator();
};

template<class T>
inline int Set<T>::count() 
{
  return _count;
}

template<class T>
inline int Set<T>::size() 
{
  return _size;
}

template<class T>
char Set<T>::Add(T item) {
  if(Contains(item)) return 0;
  for(int i = 0;i < _size; i++) {
    if(!_validator(_data[i])) {
      _data[i] = item;
      _count++;
      return 1;
    }
  }
  return 0;
}

template<class T>
char Set<T>::Contains(T value) {
  Set::Iterator iter = getIterator();
  while(iter.hasNext()) {
    T item = iter.next();
    if(_comparator(value,item)) return 1;
  }
  return 0;
}

template<class T>
char Set<T>::Remove(T value) {
  Set<T>::Iterator iter = getIterator();
  while(iter.hasNext()) {
    T *item = iter.nextPtr();
    if(_comparator(value,*item)) {
      *item = _initializer();
      _count--;
      // delete iter;
      return 1;
    }
  }
  // delete iter;
  return 0;
}

template<class T>
typename Set<T>::Iterator Set<T>::getIterator() {
  return Iterator(this);
}

template<class T>
char Set<T>::Iterator::hasNext() {
  return (currentCount < (parent->_count)) ? 1 : 0;
}

template<class T>
T Set<T>::Iterator::next() {
  do {
    cursor++;
    if(cursor == (parent->size() - 1)) break;
  } while(!parent->_validator(parent->_data[cursor]));
  currentCount++;
  return parent->_data[cursor];
}

template<class T>
T *Set<T>::Iterator::nextPtr() {
  do {
    cursor++;
    if(cursor == (parent->size() - 1)) break;
  } while(!parent->_validator(parent->_data[cursor]));
  currentCount++;
  return (parent->_data) + cursor;
}