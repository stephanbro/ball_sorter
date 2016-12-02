
#include "RingFIFO.hpp"
#include <stdio.h>

RingFIFO::RingFIFO(uint8_t size) :
  m_buffer{},
  m_size(size+1),
  m_head(0),
  m_tail(0)
{
  printf("Made buffer of size %d\n", size);
}

bool RingFIFO::is_empty(void) {
  return m_head == m_tail;
}

bool RingFIFO::is_full(void) {
  return (m_tail + 1) % m_size == m_head;
}

uint8_t RingFIFO::get_size(void) {
  if (m_tail >= m_head) {
    return m_tail - m_head;
  } else {
    return (uint16_t)(m_tail + UINT8_MAX) - m_head;
  }
}

uint8_t RingFIFO::get(uint8_t index) {
  if (index > get_size()) {
    return UINT8_MAX;
  }
  uint8_t wrapped_index = (index + m_head) % m_size;
  return m_buffer[wrapped_index];
}

uint8_t RingFIFO::pop(void) {
  if (is_empty() == true) {
    printf("Poping off empty\n");
    return UINT8_MAX;
  }
  m_head = (m_head + 1) % m_size;
  uint8_t output = m_buffer[m_head];
  printf("Popping off %d from index %d\n", output, m_head);
  return output;
}

void RingFIFO::push(uint8_t val) {
  if (is_full() == true || val == UINT8_MAX) {
    printf("Pushing onto full\n");
    return;
  }
  m_tail = (m_tail + 1) % m_size;
  m_buffer[m_tail] = val;
  printf("Adding %d to index %d\n", val, m_tail);
}
