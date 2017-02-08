
#ifndef _RINGFIFO_HPP_
#define _RINGFIFO_HPP_

#include <stdint.h>
#include <stdbool.h>

class RingFIFO
{
  public:
    RingFIFO();
    RingFIFO(uint8_t size);
    virtual ~RingFIFO() { }

    bool is_empty(void);
    bool is_full(void);

    uint8_t get_size(void);
    uint8_t get(uint8_t index);
    uint8_t front(void);

    uint8_t pop(void);
    void push(uint8_t val);

    static const uint8_t EMPTY;

  private:
    uint8_t m_buffer[6];
    const uint16_t m_size;
    uint8_t m_head;
    uint8_t m_tail;
};

#endif /* _RINGFIFO_HPP_ */

