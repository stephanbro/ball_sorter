
#ifndef _RINGFIFO_HPP_
#define _RINGFIFO_HPP_

#include <stdint.h>
#include <stdbool.h>

class RingFIFO
{
  public:
    RingFIFO(uint8_t size = DEFAULT_SIZE);
    ~RingFIFO();

    bool is_empty(void);
    bool is_full(void);

    uint8_t get_size(void);
    uint8_t get(uint8_t index);
    uint8_t front(void);

    uint8_t pop(void);
    void push(uint8_t val);

    static constexpr uint8_t EMPTY = UINT8_MAX;

  private:
    uint8_t *m_buffer;
    uint8_t m_size;
    uint8_t m_head;
    uint8_t m_tail;
    static constexpr uint8_t DEFAULT_SIZE = 5;
};

#endif /* _RINGFIFO_HPP_ */

