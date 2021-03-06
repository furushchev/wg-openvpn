#ifndef WG_PING_H__
#define WG_PING_H__

struct __attribute__((__packed__)) udpmon_pkt
{
  char magic[4];
  double sent;
  double echoed;
  uint32_t seqnum;
  char padding[4096];
};

double wg_ping_gettime()
{
#if POSIX_TIMERS > 0
  struct timespec curtime;
  clock_gettime(CLOCK_REALTIME, &curtime);
  return (uint64_t)(curtime.tv_sec) + (uint64_t)(curtime.tv_nsec) * 1e-9;
#else
  struct timeval timeofday;
  gettimeofday(&timeofday,NULL);
  return (uint64_t)(timeofday.tv_sec) + (uint64_t)(timeofday.tv_usec) * 1e-6;
#endif
}

void is_wg_ping_pkt(struct context *c)
{
  struct buffer *buf = &c->c2.buf;
  char udpmonitor_magic[] = { 0xEF, 0x41, 0xc6, 0x35 };
  char *in_data = BPTR(buf);
  struct udpmon_pkt *pkt = (struct udpmon_pkt *) in_data;
  char *after_echoed = (char *) (&pkt->echoed + 1);
  int minsize = after_echoed - in_data;
  int len = BLEN(buf);

  if (len < sizeof(udpmonitor_magic) || memcmp(udpmonitor_magic, in_data, sizeof(udpmonitor_magic)))
    return;

  if (len >= minsize)
    pkt->echoed = wg_ping_gettime();

  link_socket_write_udp(c->c2.link_socket, buf, &c->c2.from);

  buf->len = 0;
}

#endif // WG_PING_H__
