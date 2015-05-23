-Ipthread
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/in.h>

#define  DELAY  100

typedef unsigned char __u8;
typedef unsigned short __u16;

struct arp_pkt {

    __u16  hw_format;
    __u16  prot_format;
    __u8   hw_len;
    __u8   prot_len;
    __u16  opcode;
    __u8  hw_sender[ETH_ALEN];
    __u8  ip_sender[4];
    __u8  hw_target[ETH_ALEN];
    __u8  ip_target[4];
};

void die(char *fmt, ...)
{
     va_list ap;
     va_start(ap, fmt);

     vfprintf(stderr, fmt, ap);
     fputc('\n', stderr);

     va_end(ap);
     exit(-1);
}

void * get_reply(void *arg)
{
     struct arp_pkt pack;
     int sock = *(int *)arg;
     struct sockaddr_ll dlink;
     int size = sizeof(struct sockaddr_ll);

     while(1) {

          if(recvfrom(sock, &pack, sizeof(pack), 0, (struct sockaddr *)&dlink, &size) < 0)
              die("recvfrom() : %s");

          if(pack.opcode == htons(ARPOP_REPLY))

                printf("%d.%d.%d.%d -> %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
                       pack.ip_sender[0], pack.ip_sender[1],
                       pack.ip_sender[2], pack.ip_sender[3],
                       pack.hw_sender[0], pack.hw_sender[1], pack.hw_sender[2],
                       pack.hw_sender[3], pack.hw_sender[4], pack.hw_sender[5]);
     }
     pthread_exit(NULL);
}

int main(int argc, char **argv)
{
     pthread_t t;
     int i, j, sock;
     struct ifreq ifr;
     unsigned long addr;
     struct arp_pkt pack;
     struct sockaddr_ll dlink;

     if(argc != 2) die("%s <interface>", argv[0]);

     sock = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
     if(sock < 0) die("socket() : %s", strerror(errno));

     /* popoliamo la struttura da passare alla sendto */
     dlink.sll_family = AF_PACKET;
     dlink.sll_protocol = htons(ETH_P_ARP);
     dlink.sll_halen = ETH_ALEN;
     /* inviamo in broadcast */
     memset(dlink.sll_addr, 0xff, ETH_ALEN);

     /* ricaviamo l'interface index e lo scriviamo nella struttura */
     strncpy((char *)ifr.ifr_name, argv[1], IFNAMSIZ);
     if(ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
          die("ioctl(SIOCGIFINDEX) : %s", strerror(errno));
     dlink.sll_ifindex = ifr.ifr_ifindex;

     /* bindiamo il socket all'interfaccia */
     if(bind(sock, (struct sockaddr *)&dlink, sizeof(dlink)) < 0)
         die("bind() : %s", strerror(errno));

     /* riempiamo l'header arp */
     pack.hw_format = htons(ARPHRD_ETHER);
     pack.prot_format = htons(ETH_P_IP);
     pack.hw_len = ETH_ALEN;
     pack.prot_len = 4;
     pack.opcode = htons(ARPOP_REQUEST);

     /* ricaviamo l'indirizzo IPv4 bindato all'interfaccia */
     if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
          die("ioctl(SIOCGIFADDR) : %s", strerror(errno));

     addr = (*((struct sockaddr_in *)&ifr.ifr_addr)).sin_addr.s_addr;
     memcpy(pack.ip_sender, (char *)&addr, 4);

     /* ricaviamo il l'indirizzo fisico MAC */
     if(ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
          die("ioctl(SIOCGIFHWADDR) : %s", strerror(errno));

     memcpy(pack.hw_sender, (char *)ifr.ifr_hwaddr.sa_data, ETH_ALEN);

     /* broadcast */
     memset(pack.hw_target, 0xff, ETH_ALEN);

     /* leggiamo in parallelo le risposte */
     pthread_create(&t, NULL, get_reply, &sock);

     /* inviamo alla subnet 192.168.*.*   */
     pack.ip_target[0]=192;
     pack.ip_target[1]=168;

     for(i = 0; i <= 255; i++) {

          pack.ip_target[2] = i;

          for(j = 0; j <= 255; j++) {

              pack.ip_target[3] = j;

              if(sendto(sock, &pack, sizeof(pack), 0, (struct sockaddr *)&dlink, sizeof(dlink)) < 0)
                  die("sendto() : %s", strerror(errno));

              usleep(DELAY);
          }
     }
     exit(0);
}
