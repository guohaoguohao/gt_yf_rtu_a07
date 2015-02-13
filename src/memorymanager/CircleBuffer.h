/*
 * CircleBuffer.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_MEMORYMANAGER_CIRCLEBUFFER_H_
#define SRC_MEMORYMANAGER_CIRCLEBUFFER_H_

#include <atomic>
#include <cstdbool>
#include <exception>
#include <memory>
#include <vector>

namespace memorymanager {

/*
 * 循環內存類,模板參數爲值類型，如int,short等，因此在內部方法中採取直接拷貝的方式而非傳遞引用
 * 描述：此類用來存儲採集卡(ADIO)从各类传感器讀取的數據，在init時申請一個固定大小區域內存(20000bytes)，在讀取時寫入數據，
 * 並修改寫入指針，在讀取時修改讀取指針。
 *
 */
template <typename T>
class CircleBuffer {
 public:
  typedef T value_type;

  CircleBuffer();
  virtual ~CircleBuffer();
  /**
   * @brief 内存大小由参数size控制，size_per_write控制采集卡(AD/IO)每次写入数据块的大小
   * @return 内存创建是否成功
   */ bool Init(const unsigned long size, const unsigned long size_per_write);
  /**
   * @brief 将一个类型为T的数据放入內存中
   * @details
   * 將一个类型为T的数据放入end所指向的內存中，并移动end到下一個写入位置
   * 此时end有可能到到达内存终点，移动时要注意移动到的內存收地址是否越界
   */
  void Write(T const c);
  /**
   * @brief 將size长度的数据，从c位置开始的字符放入內存中
   * @details 循环size调用Write(T const c);
   */
  void Write(const T* c, const unsigned long size);

  /**
   * @brief 获取内存容量
   * @return 内存容量
   */
  unsigned long getCapacity() const { return capacity; }

  unsigned long getValidCapacity() const { return valid_capacity; }

  unsigned long getSizePerWrite() const { return size_per_write; }

  /**
   * @brief 获取内存当前的写入位置
   * @return 当前内存写入位置
   */
  unsigned long getWrite() const { return writepos; }

  /*×
   * 获取写入数据类型占据的空间大小
   * */
  int getSizeofTypeT() const { return sizeof(T); }

  /**
   * #brief 获取内存的循环计数
   * @return 循环计数
   */
  int getCycleCount() const { return cyclecount; }

  std::shared_ptr<std::vector<T>> getData() const { return buffer; }

  void UpdateWrite(const unsigned long new_writepos) {
    if (new_writepos >= valid_capacity) {
      writepos = 0;
      ++cyclecount;
    } else {
      writepos = new_writepos;
    }
  }

  T operator[](unsigned long index);

  void Reset();

  //  SAMPLING_CARD_TYPES getCardtype() const {
  //    return cardtype;
  //  }

  //  void setCardtype(SAMPLING_CARD_TYPES cardtype) {
  //    this->cardtype = cardtype;
  //  }

 private:
  /*
   * 永远指向內存收地址，不变
   */
  std::shared_ptr<std::vector<T>> buffer;
  /*
   * 指向能写入的位置（初始状态时该位置上没有数据）
   */
  std::atomic<unsigned long> writepos;
  /*
   * 容量
   */
  unsigned long capacity;
  unsigned short unitsize;
  /**
   * 是采集卡每次写入块大小的整数倍，应在创建内存时设置.
   */
  unsigned long valid_capacity;
  /**
   * 采集卡每次写入的块大小.
   */
  unsigned long size_per_write;
  /**
   * 循环写入计数
   */
  std::atomic<int> cyclecount;

  //  SAMPLING_CARD_TYPES cardtype;
};

template <typename T>
CircleBuffer<T>::CircleBuffer()
:valid_capacity(0),
 size_per_write(0),
 writepos(0),
 capacity(0),
 unitsize(sizeof(T)){
}

template <typename T>
CircleBuffer<T>::~CircleBuffer() {}

template <typename T>
bool CircleBuffer<T>::Init(const unsigned long size,
                           const unsigned long size_per_write) {
  capacity = size;
  valid_capacity =
      static_cast<unsigned long>(size / size_per_write) * size_per_write;
  this->size_per_write = size_per_write;
  try {
    buffer = std::make_shared<std::vector<T>>(capacity);
  }
  catch (const std::exception& e) {
    // todo log error here
    return false;
  }
  writepos = 0;
  cyclecount = 0;
  return true;
}

template <typename T>
void CircleBuffer<T>::Write(const T c) {
  (*buffer)[writepos] = c;
  if ((writepos + 1) >= capacity) {
    writepos = 0;
    ++cyclecount;
  } else {
    ++writepos;
  }
}

template <typename T>
void CircleBuffer<T>::Write(const T* c, const unsigned long size) {
  for (unsigned long var = 0; var < size; ++var) {
    Write(*(c + var));
  }
}

template <typename T>
T CircleBuffer<T>::operator[](unsigned long index) {
  return (*buffer)[index];
}

template <typename T>
void CircleBuffer<T>::Reset() {
  this->writepos = 0;
  this->cyclecount = 0;
}

inline static unsigned long CALCSIZE(unsigned long readpos,
                                     unsigned long writepos,
                                     unsigned long size);

inline static void READPOSINDENT(unsigned long& readpos, unsigned long step,
                                 unsigned long size);

inline unsigned long CALCSIZE(unsigned long readpos, unsigned long writepos,
                              unsigned long size) {
  if (readpos == writepos) {
    return 0;
  } else if (readpos < writepos) {
    return (writepos - readpos);
  } else {
    return (size - (readpos - writepos));
  }
}

inline void READPOSINDENT(unsigned long& readpos, unsigned long step,
                          unsigned long size) {
  if ((readpos + step) <= (size - 1)) {
    readpos += step;
  } else {
    readpos = step - (size - readpos);
  }
}

inline void READPOSROLLBACK(unsigned long& readpos, unsigned long step,
                            unsigned long size) {
  if (readpos >= step) {
    readpos -= step;
  } else {
    readpos = size - step + readpos;
  }
}

} /* namespace memorymanager */

#endif /* SRC_MEMORYMANAGER_CIRCLEBUFFER_H_ */
