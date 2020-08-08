class Field {
  public:
    Field(String* p_searchStringPtr, char p_separator) ;
    bool findNext(String& p_field, bool p_toEnd=false) ;

  private:
    String* _searchStringPtr = NULL ;
    char _separator ;
    int _position ;
} ;

Field::Field(String* p_searchStringPtr, char p_separator=' ') {
  this->_searchStringPtr = p_searchStringPtr ;
  this->_separator = p_separator ;
  this->_position = 0 ;
} ;

bool Field::findNext(String& p_field, bool p_toEnd) {
  if (this->_searchStringPtr == NULL || this->_position >= (*this->_searchStringPtr).length()) return false ;
  int l_pos = (*_searchStringPtr).indexOf(_separator,_position) ;
  if ((l_pos == -1) || p_toEnd) {
    p_field = (*(this->_searchStringPtr)).substring(_position) ;
    this->_searchStringPtr = NULL ;
  } else {
    p_field = (*(this->_searchStringPtr)).substring(this->_position,l_pos) ;
    this->_position = l_pos + 1 ;
  }
  return true ;
} ;


template <class T>
class Fifo {
  public:
    Fifo(uint8_t p_size) ;
    ~Fifo(void) ;
    bool isEmpty(void) ;
    bool isFull(void) ;
    bool pushRef(T& p_value, bool p_dropOldest=true) ;
    bool push(T p_value, bool p_dropOldest=true) ;
    bool pull(T& p_value) ;
    bool getAt(uint8_t p_index, T& p_item) ;
    bool setAt(uint8_t p_index, T& p_item) ;
    String dump(String (* p_callback)(T&)) ;
    bool has(bool (*p_callback)(T&)) ;

  private:
    uint8_t _size ;
    T* _fifo ;
    uint8_t _first ;
    uint8_t _last ;
    uint8_t _count ;
} ;


template <class T>
Fifo<T>::Fifo(uint8_t p_size) {
  this->_fifo = (T*) malloc(sizeof(T) * p_size) ;
  this->_size = p_size ;
  this->_first = this->_last = this->_count = 0 ;
} ;


template <class T>
Fifo<T>::~Fifo(void) {
  free(this->_fifo) ;
} ;


template <class T>
bool Fifo<T>::isEmpty(void) {
  return (this->_count == 0) ;
} ;


template <class T>
bool Fifo<T>::isFull(void) {
  return (this->_count >= this->_size) ;
} ;


template <class T>
bool Fifo<T>::pushRef(T& p_value, bool p_dropOldest) {
  if (this->isFull()) {
    if (p_dropOldest)
      this->_first = (this->_first + 1) % this->_size ; // Buffer full, drop first value
    else
      return false ;
  } else
    this->_count++ ;
  this->_fifo[this->_last] = p_value ;
  this->_last = (this->_last + 1) % this->_size ;
  return true ;
} ;


template <class T>
bool Fifo<T>::push(T p_value, bool p_dropOldest) {
  return this->pushRef(p_value,p_dropOldest) ;
} ;


template <class T>
bool Fifo<T>::pull(T& p_value) {
  if (this->isEmpty()) return false ;
  p_value = this->_fifo[this->_first] ;
  this->_first = (this->_first + 1) % this->_size ;
  this->_count-- ;
  return true ;
} ;


template <class T>
bool Fifo<T>::getAt(uint8_t p_rank, T& p_item) {
  if (p_rank >= this->_count) return false ;
  p_item = this->_fifo[(this->_first + p_rank) % this->_size] ;
  return true ;
} ;


template <class T>
bool Fifo<T>::setAt(uint8_t p_rank, T& p_item) {
  if (p_rank >= this->_count) return false ;
  this->_fifo[(this->_first + p_rank) % this->_size] = p_item ;
  return true ;
} ;


template <class T>
String Fifo<T>::dump(String (*p_callback)(T&)) {
  T l_item ;
  String l_dumpstr = "" ;

  for (uint8_t l_rank=0 ; this->getAt(l_rank, l_item) ; l_rank++)
    l_dumpstr += "[" + (p_callback(l_item)) + "] " ;
  if (l_dumpstr == "") l_dumpstr = F("*") ;
  return l_dumpstr ;
} ;


template <class T>
bool Fifo<T>::has(bool (*p_callback)(T&)) {
  T l_item ;

  for (uint8_t l_rank=0 ; this->getAt(l_rank, l_item) ; l_rank++)
    if (p_callback(l_item)) return true ;
  return false ;
} ;



class Body {
  public:
    Body(String* p_body) ;
    bool nextKeyVal(String &p_key, String& p_val) ;

  private:
    Field* _pfield ;
    bool _notRequest ;
} ;


Body::Body(String* p_body) {
  this->_pfield = new Field(p_body) ;
  this->_notRequest = true ;
} ;


bool Body::nextKeyVal(String &p_key, String& p_val) {
  String l_keyval ;

  if (this->_notRequest && this->_pfield->findNext(l_keyval)) {
    Field l_field(&l_keyval,'=') ;
    if (l_field.findNext(p_key)) {
      if (!l_field.findNext(p_val)) { 
        p_val = "" ;
        this->_notRequest = false ;
      }
      return true ;
    }
  }
  return false ;
} ;
