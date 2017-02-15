
#include "RingFIFO.hpp"

constexpr uint8_t RingFIFO::EMPTY;

RingFIFO::RingFIFO(uint8_t size) :
  m_buffer(nullptr),
  m_size(size+1),
  m_head(0),
  m_tail(0)
{
  // Handle if size has rolled over
  if (size == UINT8_MAX) {
    m_size = UINT8_MAX;
  }
  m_buffer = new uint8_t[m_size];
}

RingFIFO::~RingFIFO()
{
  delete[] m_buffer;
}

bool RingFIFO::is_empty(void)
{
  return m_head == m_tail;
}

bool RingFIFO::is_full(void)
{
  return (m_tail + 1) % m_size == m_head;
}

uint8_t RingFIFO::get_size(void)
{
  if (m_tail >= m_head) {
    return m_tail - m_head;
  } else {
    return (m_size - m_head) + m_tail;
  }
}

uint8_t RingFIFO::get(uint8_t index)
{
  if (index > get_size()) {
    return EMPTY;
  }
  uint8_t wrapped_index = (m_head + 1 + index) % m_size;
  return m_buffer[wrapped_index];
}

uint8_t RingFIFO::front(void)
{
  if (is_empty() == true) {
    return EMPTY;
  }
  uint8_t index = (m_head + 1) % m_size;
  return m_buffer[index];
}

uint8_t RingFIFO::pop(void)
{
  if (is_empty() == true) {
    return EMPTY;
  }
  m_head = (m_head + 1) % m_size;
  return m_buffer[m_head];
}

void RingFIFO::push(uint8_t val)
{
  if (is_full() == true || val == EMPTY) {
    return;
  }
  uint8_t index = (m_tail + 1) % m_size;
  m_buffer[index] = val;
  m_tail = index;
}
